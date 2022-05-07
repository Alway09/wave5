/*
  ==============================================================================

    EnvelopeVisualComponent.h
    Created: 3 May 2022 1:22:38pm
    Author:  alway

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GlobalUIConstants.h"

// left ID, right ID and line between them
using LinesVectorElement = std::pair<std::pair<int, int>, juce::Line<float>*>;
using LinesVector = std::vector<LinesVectorElement>;


//==============================================================================


class EnvelopeVisualComponent  : public juce::Component, public juce::Timer, public juce::Slider::Listener
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
    EnvelopeVisualComponent(bool isAdsr = false);
    ~EnvelopeVisualComponent() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;

    void mouseDoubleClick(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;

    int addDot(int x, int y, bool removable, bool withConstantX, bool withConstantY);
    void removeDot(int ID);
    
    void addLine(EnvelopeVisualComponent::MovingDot const * leftDot,
                 EnvelopeVisualComponent::MovingDot const * rightDot);
    void removeLine(int leftId, int rightId);
    void updateLine(int leftId, int rightId);
    
    void sliderValueChanged (juce::Slider *slider) override;
    
private:
    int getDotIndex(int ID);
    juce::Line<float>* lineBetween(int leftId, int rightId);
    LinesVector::iterator getLineElement(juce::Line<float>*);

    juce::Point<int> stayPointInsideComponent(MovingDot* dot, juce::Point<int> eventPos);
    juce::Point<int> complyOrder(MovingDot* dot, juce::Point<int> eventPos);
    
    MovingDot* getLeftDot(int x, int y);
    MovingDot* getRightDot(int x, int y);
    
    int dotsIdCounted = 1;
    
    //for ADSR
    void setupAdsr();
    void updateAdsr();
    void timerCallback() override{ setupAdsr(); updateAdsr(); stopTimer(); };
    bool isADSR;
    
    double attackTimeSeconds = 1.0;
    double decayTimeSeconds = 1.0;
    double sustainLevel = 1.0; // 0.0 to 1.0
    double releaseTimeSeconds = 1.0;
    
    int attackDotId;
    int decayDotId;
    int sustainDotId;
    int releaseDotId;
    
    double widthInSeconds = 5.0;

    std::vector<MovingDot*> dotsVector;
    LinesVector linesVector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeVisualComponent)
};
