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


class EnvelopeVisualComponent  : public juce::Component
{
private:
    class MovingDot : public juce::Component
    {
    public:
        MovingDot(int ID, int leftID, int rightID, bool doubleClickAllow);
        ~MovingDot() {};

        void paint(juce::Graphics&) override;
        void resized() override;

        void mouseEnter(const juce::MouseEvent& event) override;
        void mouseExit(const juce::MouseEvent& event) override;

        int getId() const { return dotId; }
        int getLeftId() const { return leftDotId; }
        int getRightId() const { return rightDotId; }
        juce::Point<int> getCentrePosition() const;
        void updateBounds();

        void setLeftId(int ID) { leftDotId = ID; }
        void setRightId(int ID) { rightDotId = ID; }

    private:
        bool doubleClickAllowed;
        bool mouseIsOn;
        int dotId;
        int leftDotId;
        int rightDotId;

        juce::Rectangle<int> dotBounds;
    };
    
public:
    EnvelopeVisualComponent();
    ~EnvelopeVisualComponent() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;

    void mouseDoubleClick(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;

    void addDot(int x, int y);
    void removeDot(int ID);
    
    void addLine(EnvelopeVisualComponent::MovingDot const * leftDot,
                 EnvelopeVisualComponent::MovingDot const * rightDot);
    void removeLine(int leftId, int righttId);
    
private:
    int getDotIndex(int ID);
    juce::Line<float>* lineBetween(int leftId, int rightId);
    LinesVector::iterator getLineElement(juce::Line<float>*);

    juce::Point<int> stayPointInsideComponent(MovingDot* dot, juce::Point<int> eventPos);
    juce::Point<int> complyOrder(MovingDot* dot, juce::Point<int> eventPos);
    
    MovingDot* getLeftDot(int x, int y);
    MovingDot* getRightDot(int x, int y);
    
    int dotsIdCounted = 1;

    std::vector<MovingDot*> dotsVector;
    LinesVector linesVector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeVisualComponent)
};
