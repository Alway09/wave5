/*
  ==============================================================================

    LFOPropertiesComponent.h
    Created: 28 May 2022 4:11:49pm
    Author:  Валентин Чекулаев

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LFOPropertiesComponent  : public juce::Component
{
public:
    LFOPropertiesComponent();
    ~LFOPropertiesComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOPropertiesComponent)
};
