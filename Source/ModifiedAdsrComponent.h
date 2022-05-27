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
#include "StringConstants.h"

//==============================================================================

using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

class ModifiedAdsrComponent  : public juce::Component, juce::Timer
{
public:
    ModifiedAdsrComponent(juce::AudioProcessorValueTreeState& adsr, const juce::StringArray& idList);
    ~ModifiedAdsrComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setSizes();
    
    void timerCallback() override;
    
    void setCustomLookAndFeel(CustomLookAndFeel* lookAndFeel);
    
    juce::Rectangle<int> getCompBounds() const;

    void prepareSlider(juce::Slider& slider,
                       juce::AudioProcessorValueTreeState& adsrAPVTS,
                       juce::String paramId,
                       std::unique_ptr<SliderAttachment>& attachment,
                       juce::Label& sliderLabel);

private:
    //juce::String name;
    
    //juce::AudioProcessorValueTreeState* a

    EnvelopeVisualComponent envelope;

    CustomRotarySlider attackSlider;
    CustomRotarySlider decaySlider;
    CustomRotarySlider sustainSlider;
    CustomRotarySlider releaseSlider;
    
    juce::Label attackLabel;
    juce::Label decayLabel;
    juce::Label sustainLabel;
    juce::Label releaseLabel;

    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;

    juce::Rectangle<int> slidersAreaBounds;
    juce::Rectangle<int> sliderBounds;
    juce::Rectangle<int> envelopeBounds;
    
    juce::StringArray idList;
    
    //float widthInSeconds = 5.0f;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModifiedAdsrComponent)
};
