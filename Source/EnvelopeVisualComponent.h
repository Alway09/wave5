#pragma once

#include <JuceHeader.h>
#include "GlobalUIConstants.h"
#include "SynthVoice.h"

// left ID, right ID and line between them
using LinesVectorElement = std::pair<std::pair<int, int>, juce::Line<float>*>;
using LinesVector = std::vector<LinesVectorElement>;

using APVTS = juce::AudioProcessorValueTreeState;

//==============================================================================


class EnvelopeVisualComponent  : public juce::Component, public juce::Timer, public APVTS::Listener
{
private:
    class MovingDot : public juce::Component
    {
    public:
        MovingDot(int ID, int leftID, int rightID, bool makeRemovible,
                  bool withConstantX, bool withConstantY);
        ~MovingDot() {};

        void paint(juce::Graphics&) override;
        void resized() override;

        void mouseEnter(const juce::MouseEvent& event) override;
        void mouseExit(const juce::MouseEvent& event) override;

        int getId() const { return dotId; }
        int getLeftId() const { return leftDotId; }
        int getRightId() const { return rightDotId; }
        juce::Point<int> getCentrePosition() const;

        void setLeftId(int ID) { leftDotId = ID; }
        void setRightId(int ID) { rightDotId = ID; }
        void setCentrePosition(juce::Point<int> p);
        
        bool removable() const { return isRemovable; }

    private:
        bool isRemovable;
        bool mouseIsOn;
        bool xIsConstant;
        bool yIsConstant;
        bool positionInitialized = false;
        
        int dotId;
        int leftDotId;
        int rightDotId;

        juce::Rectangle<int> dotBounds;
    };
    
    struct ADSRParameters{
        double attackTimeSeconds = 1.0;
        double decayTimeSeconds = 1.0;
        double sustainLevel = 1.0; // 0.0 to 1.0
        double releaseTimeSeconds = 1.0;
        
        int attackDotId = 0;
        int decayDotId = 0;
        int sustainDotId = 0;
        int releaseDotId = 0;
        
        APVTS* apvts;
        
        /* in this list MUST be
           0 index - attack
           1 index - decay
           2 index - sustain
           3 index - release
         */
        juce::StringArray idList;
        //juce::String adsrStateId;
                        
        int currentPosition = 0;
    };
    
    struct LFOParameters{
        std::map<int, std::atomic<SynthVoice*>* > voices;
        int lfoNumber = 0; // 1 2 or 3
        //unsigned int periodsIdCounter = 0;
    };
    
public:
    EnvelopeVisualComponent();
    ~EnvelopeVisualComponent() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;

    // add and remove dots handler
    void mouseDoubleClick(const juce::MouseEvent& event) override;
    
    // dots move handler
    void mouseDrag(const juce::MouseEvent& event) override;

    int addDot(int x, int y, bool removable, bool withConstantX, bool withConstantY);
    void removeDot(int ID);
    
    // sets envelope as ADSR
    void setEnvelopeAsADSR(APVTS* apvtsListenTo, juce::StringArray paramsIDsListenTo);
    
    void setEnvelopeAsLFO(std::map<int, std::atomic<SynthVoice*>* >& voices, int lfoNumber);
    
    // calling when attack, decay, sustain, or release value changed
    // sets ADSR dots
    //void sliderValueChanged (juce::Slider *slider) override;
    void parameterChanged(const juce::String &parameterID, float newValue) override;
    
    // starts evnelope period
    void startEnvelope();
    
    // stops envelope period
    void stopEnvelope();
    
private:
    // return dot index in dotsVector member
    int getDotIndex(int ID);
    
    // return pointer on nearest left dot, if it exist
    MovingDot* getLeftDot(int x, int y);
    
    // return pointer on nearest left dot, if it exist
    MovingDot* getRightDot(int x, int y);
    
    // the handler to keep the dot inside the component
    juce::Point<int> stayPointInsideComponent(MovingDot* dot, juce::Point<int> eventPos);
    
    // the handler, observing order of dots
    juce::Point<int> complyOrder(MovingDot* dot, juce::Point<int> eventPos);
    
    void addLine(EnvelopeVisualComponent::MovingDot const * leftDot,
                 EnvelopeVisualComponent::MovingDot const * rightDot);
    void removeLine(int leftId, int rightId);
    void updateLine(int leftId, int rightId);
    juce::Line<float>* lineBetween(int leftId, int rightId);
    LinesVector::iterator getLineElement(juce::Line<float>*);

    //for ADSR
    void setupAdsr();
    void updateAdsr(int movingDotId, bool updateFromHost = true);
    void updateApvtsValues(int ID);
    void timerCallback() override;
    bool isADSR = false;
    bool adsrIsSettedUp = false;
    
    ADSRParameters* adsrParams = nullptr;
    
    LFOParameters* lfoParams = nullptr;
    bool isLFO = false;
    bool lfoIsSettedIp = false;
    
    int dotsIdCounted = 1;
    double widthInSeconds = 6.0;

    std::vector<MovingDot*> dotsVector;
    LinesVector linesVector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeVisualComponent)
};
