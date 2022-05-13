/*
  ==============================================================================

    OscPropertiesComponent.h
    Created: 13 May 2022 3:08:15pm
    Author:  Валентин Чекулаев

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GlobalUIConstants.h"

//==============================================================================
/*
*/
class OscPropertiesComponent  : public juce::Component
{
public:
    OscPropertiesComponent(juce::AudioProcessorValueTreeState& apvts, const juce::String& chooseId);
    ~OscPropertiesComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void buttonsClickHandler();

private:
    juce::ComboBox waveChooser;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveChooserAttachment;
    
    juce::ImageButton sineWaveButton;
    juce::ImageButton squareWaveButton;
    juce::ImageButton sawWaveButton;
    juce::ImageButton triangleWaveButton;
    juce::ImageButton noiseWaveButton;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscPropertiesComponent)
};
