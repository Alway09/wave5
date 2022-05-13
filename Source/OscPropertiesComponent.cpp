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
OscPropertiesComponent::OscPropertiesComponent(juce::AudioProcessorValueTreeState& apvts, const juce::String& chooseId)
{
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
    
    addAndMakeVisible(waveChooser);
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
}

void OscPropertiesComponent::resized(){
    auto localBounds = getLocalBounds();
    
    auto buttonBounds = localBounds.removeFromLeft(UI::OSC_PROPERTIES::height);
    
    sineWaveButton.setBounds(buttonBounds);
    
    buttonBounds = localBounds.removeFromLeft(UI::OSC_PROPERTIES::height);
    
    squareWaveButton.setBounds(buttonBounds);
    
    buttonBounds = localBounds.removeFromLeft(UI::OSC_PROPERTIES::height);
    
    sawWaveButton.setBounds(buttonBounds);
    
    buttonBounds = localBounds.removeFromLeft(UI::OSC_PROPERTIES::height);
    
    triangleWaveButton.setBounds(buttonBounds);
    
    buttonBounds = localBounds.removeFromLeft(UI::OSC_PROPERTIES::height);
    
    noiseWaveButton.setBounds(buttonBounds);
    
    waveChooser.setBounds(localBounds);
}
