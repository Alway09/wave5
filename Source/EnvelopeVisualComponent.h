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
    MovedDot(int ID, bool doubleClickAllow);
    ~MovedDot() {};

    void paint(juce::Graphics&) override;
    void resized() override;

    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;
    //void mouseDoubleClick(const juce::MouseEvent& event) override;

    int getId() const { return dotId; }
    //void setAreaCentre(int x, int y);
    void updateBounds();

private:
    bool doubleClickAllowed;
    bool mouseIsOn;
    int dotId;

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

private:
    //bool doubleClickAllowed;
    int dotsAmount = 0;

    std::vector<MovedDot*> dotsVector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeVisualComponent)
};
