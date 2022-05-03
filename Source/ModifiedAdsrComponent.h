/*
  ==============================================================================

    ModifiedAdsrComponent.h
    Created: 2 May 2022 4:40:25pm
    Author:  alway

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ModifiedAdsrData.h"
#include "CustomRotarySlider.h"
#include "GlobalUIConstants.h"
#include "EnvelopeVisualComponent.h"

//==============================================================================

using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

class ModifiedAdsrComponent  : public juce::Component
{
public:
    ModifiedAdsrComponent(juce::String adsrName, juce::AudioProcessorValueTreeState& adsr);
    ~ModifiedAdsrComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setSizes();

    void prepareSlider(juce::Slider& slider, juce::AudioProcessorValueTreeState& adsrAPVTS, juce::String paramId, std::unique_ptr<SliderAttachment>& attachment);

private:
    juce::String name;

    EnvelopeVisualComponent envelope;

    CustomRotarySlider attackSlider;
    CustomRotarySlider decaySlider;
    CustomRotarySlider sustainSlider;
    CustomRotarySlider releaseSlider;

    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;

    juce::Rectangle<int> slidersAreaBounds;
    juce::Rectangle<int> sliderBounds;
    juce::Rectangle<int> envelopeBounds;
    
    float widthInSeconds = 5.0f;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModifiedAdsrComponent)
};
