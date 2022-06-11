/*
  ==============================================================================

    EqualizerComponent.h
    Created: 10 Jun 2022 3:39:04pm
    Author:  Валентин Чекулаев

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GlobalUIConstants.h"
#include "CustomRotarySlider.h"

//==============================================================================
/*
*/

class EqualizerComponent  : public juce::Component
{
public:
    EqualizerComponent(juce::AudioProcessorValueTreeState& tree);
    ~EqualizerComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void setSizes();

private:
    
    juce::Rectangle<int> slidersAreaRect;
    juce::Rectangle<int> sliderRect;
    juce::Rectangle<int> toggleRect;
    
    juce::ToggleButton lowCutBypass;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lowCutBypassAttachment;
    juce::ToggleButton peakBypass;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> peakBypassAttachment;
    juce::ToggleButton highCutBypass;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> highCutAttachment;
    
    juce::Label lowCutLabel;
    CustomRotarySlider lowCutFreqSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowCutFreqAttachment;
    juce::Label lowCutFreqLabel;
    juce::ComboBox lowCutSlopeChooser;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lowCutSlopeAttachment;
    juce::Label lowCutSlopeLable;
    
    juce::Label highCutLabel;
    CustomRotarySlider highCutFreqSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highCutFreqAttachment;
    juce::Label highCutFreqLabel;
    juce::ComboBox highCutSlopeChooser;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> highCutSlopeAttahment;
    juce::Label highCutSlopeLable;
    
    juce::Label peakLabel;
    CustomRotarySlider peakFreqSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> peakFreqAttachment;
    juce::Label peakFreqLabel;
    CustomRotarySlider peakQSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> peakQAttachment;
    juce::Label peakQLabel;
    CustomRotarySlider peakGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> peakGainAttachment;
    juce::Label peakGainLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EqualizerComponent)
};
