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

//==============================================================================
class MovedDot : public juce::Component
{
public:
    MovedDot(int ID, int leftID, int rightID, bool doubleClickAllow);
    ~MovedDot() {};

    void paint(juce::Graphics&) override;
    void resized() override;

    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;
    //void mouseDoubleClick(const juce::MouseEvent& event) override;

    int getId() const { return dotId; }
    int getLeftId() const { return leftDotId; }
    int getRightId() const { return rightDotId; }
    //juce::Point<int> getCentre() const { return localBounds.getCentre(); }
    void updateBounds();

    void setLeftId(int ID) { leftDotId = ID; }
    void setRightId(int ID) { rightDotId = ID; }

private:
    bool doubleClickAllowed;
    bool mouseIsOn;
    int dotId;
    int leftDotId;
    int rightDotId;

    juce::Rectangle<int> localBounds;
    juce::Rectangle<int> dotBounds;
    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeVisualComponent);
};

class EnvelopeVisualComponent  : public juce::Component
{
public:
    EnvelopeVisualComponent();
    ~EnvelopeVisualComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void mouseDoubleClick(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;

    void addDot(int x, int y);
    void removeDot(int ID);
    int getDotIndex(int ID);

    juce::Point<int> stayPointInsideComponent(MovedDot* dot, juce::Point<int> eventPos);
    juce::Point<int> complyOrder(MovedDot* dot, juce::Point<int> eventPos);
private:
    //bool doubleClickAllowed;
    int dotsIdConted = 1;

    std::vector<MovedDot*> dotsVector;
    //std::vector<int> dotsIDs;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeVisualComponent)
};
