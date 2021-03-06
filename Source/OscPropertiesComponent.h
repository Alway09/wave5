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
#include "CustomRotarySlider.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/*
*/
class OscPropertiesComponent  : public juce::Component, public juce::Button::Listener
{
public:
    OscPropertiesComponent(juce::AudioProcessorValueTreeState& apvts,
                           const juce::String& chooseId,
                           const juce::String& gainId,
                           const juce::String& transposeId,
                           const juce::String& panId);
    ~OscPropertiesComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void buttonClicked(juce::Button *) override;
    //virtual void buttonStateChanged (Button *)
    
    void setCustomLookAndFeel(CustomLookAndFeel* lookAndFeel);

private:
    void setSizes();
    
    void prepareButton(juce::ImageButton& button, juce::Image& image, const juce::String& imageName);
    
    juce::ComboBox waveChooser;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveChooserAttachment;
    
    juce::Image sineWaveImage;
    juce::Image squareWaveImage;
    juce::Image sawWaveImage;
    juce::Image triangleWaveImage;
    juce::Image noiseWaveImage;
    
    juce::ImageButton sineWaveButton;
    juce::ImageButton squareWaveButton;
    juce::ImageButton sawWaveButton;
    juce::ImageButton triangleWaveButton;
    juce::ImageButton noiseWaveButton;
    
    CustomRotarySlider gainSlider;
    juce::Label gainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    
    CustomRotarySlider transposeSlider;
    juce::Label transposeLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> transposeAttachment;
    
    CustomRotarySlider panSlider;
    juce::Label panLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> panAttachment;
    
    juce::Rectangle<int> buttonBounds;
    juce::Rectangle<int> sliderBounds;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscPropertiesComponent)
};
