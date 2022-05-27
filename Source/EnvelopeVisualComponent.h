#pragma once

#include <JuceHeader.h>
#include "GlobalUIConstants.h"
#include "SynthVoice.h"

// left ID, right ID and line between them
using LinesVectorElement = std::pair<std::pair<int, int>, juce::Line<float>*>;
using LinesVector = std::vector<LinesVectorElement>;

using APVTS = juce::AudioProcessorValueTreeState;

//==============================================================================


class EnvelopeVisualComponent  : public juce::Component, public APVTS::Listener
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
    
public:
    EnvelopeVisualComponent();
    ~EnvelopeVisualComponent() override;
    
    enum class DotRelationDirection{ Vertical, Horizontal };
    
    void paint (juce::Graphics&) override;
    void resized() override;

    // add and remove dots handler
    void mouseDoubleClick(const juce::MouseEvent& event) override;
    
    // dots move handler
    void mouseDrag(const juce::MouseEvent& event) override;

    int addDot(int x, int y, bool removable, bool withConstantX, bool withConstantY);
    void removeDot(int ID);
    
    void setDotRelativeToParameter(const juce::String& paramId, int dotId, DotRelationDirection direction);
    
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    
    void setReltiveLFO(LFOData* lfo){ relativeLFO = lfo; }
    
    void setAPVTS(APVTS* tree){ apvts = tree; }
    
private:
    // return dot index in dotsVector member
    int getDotIndex(int ID);
    
    void moveRightDots(MovingDot* dot, int deltaX);
    
    void updateRelativeValue(MovingDot* dot, juce::Point<int> dotPreviousePosition);
    
    // return pointer on nearest left dot, if it exist
    MovingDot* getLeftDot(int x, int y);
    
    // return pointer on nearest left dot, if it exist
    MovingDot* getRightDot(int x, int y);
    
    // the handler to keep the dot inside the component
    juce::Point<int> stayPointInsideComponent(juce::Point<int> eventPos);
    
    // the handler, observing order of dots
    juce::Point<int> complyOrder(MovingDot* dot, juce::Point<int> eventPos);
    
    void addLine(EnvelopeVisualComponent::MovingDot const * leftDot,
                 EnvelopeVisualComponent::MovingDot const * rightDot);
    void removeLine(int leftId, int rightId);
    void updateLine(int leftId, int rightId);
    juce::Line<float>* lineBetween(int leftId, int rightId);
    LinesVector::iterator getLineElement(juce::Line<float>*);

    APVTS* apvts = nullptr;
    
    LFOData* relativeLFO = nullptr;
    
    std::map<juce::String, std::pair<int, DotRelationDirection>> dotsRelativeToParameters;
    
    int dotsIdCounted = 1;
    float widthFactor = 10.f;

    std::vector<MovingDot*> dotsVector;
    LinesVector linesVector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeVisualComponent)
};
