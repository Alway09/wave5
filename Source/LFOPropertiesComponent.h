#pragma once

#include <JuceHeader.h>
#include "CustomRotarySlider.h"
#include"CustomLookAndFeel.h"
#include "StringConstants.h"

using APVTS = juce::AudioProcessorValueTreeState;

//==============================================================================
/*
*/
class LFOPropertiesComponent  : public juce::Component, public juce::ToggleButton::Listener
{
public:
    LFOPropertiesComponent(APVTS& apvts,
                           const juce::String& rateControllerId,
                           const juce::String& workingModeId,
                           const juce::String& rateModeId);
    ~LFOPropertiesComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void setCustomLookAndFeel(CustomLookAndFeel* lookAndFeel){
        trigToggle.setLookAndFeel(lookAndFeel);
        envToggle.setLookAndFeel(lookAndFeel);
        offToggle.setLookAndFeel(lookAndFeel);
        bpmToggle.setLookAndFeel(lookAndFeel);
        hzToggle.setLookAndFeel(lookAndFeel);
        rateSlider.setLookAndFeel(lookAndFeel);
    }
    
    void buttonClicked(juce::Button *) override;
    //void buttonStateChanged(juce::Button *) override;
    
    void setSizes();

private:
    juce::ToggleButton trigToggle;
    juce::Label trigLabel;
    juce::ToggleButton envToggle;
    juce::Label envLabel;
    juce::ToggleButton offToggle;
    juce::Label offLabel;
    
    juce::ToggleButton bpmToggle;
    juce::Label bpmLabel;
    juce::ToggleButton hzToggle;
    juce::Label hzLabel;
    
    CustomRotarySlider rateSlider;
    juce::Label rateLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateAttachment;
    
    juce::ComboBox workModeChooser;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> workModeAttachment;
    
    juce::ComboBox rateModeChooser;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> rateModeAttachment;
    
    juce::Rectangle<int> toggleBounds;
    juce::Rectangle<int> sliderBounds;
    juce::Rectangle<int> localBounds;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOPropertiesComponent)
};
