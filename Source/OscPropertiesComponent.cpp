/*
  ==============================================================================

    OscPropertiesComponent.cpp
    Created: 13 May 2022 3:08:15pm
    Author:  Валентин Чекулаев

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscPropertiesComponent.h"
#include "StringConstants.h"

//==============================================================================
OscPropertiesComponent::OscPropertiesComponent(juce::AudioProcessorValueTreeState& apvts,
                                               const juce::String& chooseId,
                                               const juce::String& gainId)
    : gainSlider("Gain Slider")
{
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts,
                                                                                            gainId,
                                                                                            gainSlider);
    
    waveChooser.addItemList(STR_CONST::ADSR::oscWavesVariants, 1);
    waveChooserAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts,
                                                                                                     chooseId,
                                                                                                     waveChooser);
    
    sineWaveButton.onClick = [this](){ waveChooser.setSelectedId(1); };
    squareWaveButton.onClick = [this](){ waveChooser.setSelectedId(2); };
    sawWaveButton.onClick = [this](){ waveChooser.setSelectedId(3); };
    triangleWaveButton.onClick = [this](){ waveChooser.setSelectedId(4); };
    noiseWaveButton.onClick = [this](){ waveChooser.setSelectedId(5); };
    
    addAndMakeVisible(sineWaveButton);
    addAndMakeVisible(squareWaveButton);
    addAndMakeVisible(sawWaveButton);
    addAndMakeVisible(triangleWaveButton);
    addAndMakeVisible(noiseWaveButton);
    
    addAndMakeVisible(gainSlider);
}

OscPropertiesComponent::~OscPropertiesComponent()
{
}

void OscPropertiesComponent::buttonsClickHandler(){
    
}

void OscPropertiesComponent::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::black);
    g.drawRect(getBounds());
    
    g.drawRect(sineWaveButton.getBounds());
    g.drawRect(squareWaveButton.getBounds());
    g.drawRect(sawWaveButton.getBounds());
    g.drawRect(triangleWaveButton.getBounds());
    g.drawRect(noiseWaveButton.getBounds());
}

void OscPropertiesComponent::resized(){
    auto localBounds = getLocalBounds();
    
    auto buttonBounds = localBounds.removeFromLeft(UI::OSC_PROPERTIES::height)
        .reduced(UI::OSC_PROPERTIES::height - UI::OSC_PROPERTIES::buttonSize,
                 UI::OSC_PROPERTIES::height - UI::OSC_PROPERTIES::buttonSize)
        .withX(UI::GLOBAL::paddingFromStoke);
    
    sineWaveButton.setBounds(buttonBounds);
    
    //buttonBounds = localBounds.removeFromLeft(UI::OSC_PROPERTIES::height);
    buttonBounds.setX(buttonBounds.getX() + buttonBounds.getWidth() + UI::GLOBAL::paddingComponentsInside);
    
    squareWaveButton.setBounds(buttonBounds);
    
    //buttonBounds = localBounds.removeFromLeft(UI::OSC_PROPERTIES::height);
    buttonBounds.setX(buttonBounds.getX() + buttonBounds.getWidth() + UI::GLOBAL::paddingComponentsInside);
    
    sawWaveButton.setBounds(buttonBounds);
    
    //buttonBounds = localBounds.removeFromLeft(UI::OSC_PROPERTIES::height);
    buttonBounds.setX(buttonBounds.getX() + buttonBounds.getWidth() + UI::GLOBAL::paddingComponentsInside);

    triangleWaveButton.setBounds(buttonBounds);
    
    //buttonBounds = localBounds.removeFromLeft(UI::OSC_PROPERTIES::height);
    buttonBounds.setX(buttonBounds.getX() + buttonBounds.getWidth() + UI::GLOBAL::paddingComponentsInside);

    noiseWaveButton.setBounds(buttonBounds);
    
    juce::Rectangle<int> sliderBounds(buttonBounds.getX() + buttonBounds.getWidth() + UI::GLOBAL::paddingComponentsInside,
                                      buttonBounds.getY(),
                                      UI::GLOBAL::sliderComponentWidth,
                                      UI::GLOBAL::sliderComponentHeight);
    
    gainSlider.setBounds(sliderBounds);
    
}
