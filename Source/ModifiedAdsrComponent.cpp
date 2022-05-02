/*
  ==============================================================================

    ModifiedAdsrComponent.cpp
    Created: 2 May 2022 4:40:25pm
    Author:  alway

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ModifiedAdsrComponent.h"

using Params = ModifiedAdsrData::Parameters;

//==============================================================================
ModifiedAdsrComponent::ModifiedAdsrComponent(juce::String adsrName, juce::AudioProcessorValueTreeState& apvts)
    : name(adsrName)
{
    //updateParameters(apvts, idArray);

    prepareSlider(attackSlider, apvts, Params::idList[Params::ParametersIDs::attackTimeSeconds], attackAttachment);
    prepareSlider(decaySlider, apvts, Params::idList[Params::ParametersIDs::decayTimeSeconds], decayAttachment);
    prepareSlider(sustainSlider, apvts, Params::idList[Params::ParametersIDs::sustainLevelNormalised], sustainAttachment);
    prepareSlider(releaseSlider, apvts, Params::idList[Params::ParametersIDs::releaseTimeSeconds], releaseAttachment);
}

ModifiedAdsrComponent::~ModifiedAdsrComponent()
{
}

void ModifiedAdsrComponent::updateParameters(juce::AudioProcessorValueTreeState& apvts, juce::StringArray idArray) {

    /*attackTimeSeconds = apvts.getRawParameterValue(idArray[ModifiedAdsrData::Parameters::ParametersIDs::attackTimeSeconds]);
    decayTimeSeconds = apvts.getRawParameterValue(idArray[ModifiedAdsrData::Parameters::ParametersIDs::decayTimeSeconds]);
    sustainLevel = apvts.getRawParameterValue(idArray[ModifiedAdsrData::Parameters::ParametersIDs::sustainLevelNormalised]);
    releaseTimeSeconds = apvts.getRawParameterValue(idArray[ModifiedAdsrData::Parameters::ParametersIDs::releaseTimeSeconds]);*/

    
}

void ModifiedAdsrComponent::prepareSlider(juce::Slider& slider, juce::AudioProcessorValueTreeState& apvts, juce::String paramId, std::unique_ptr<SliderAttachment>& attachment) {
    addAndMakeVisible(slider);

    attachment = std::make_unique<SliderAttachment>(apvts, paramId, slider); 
}

void ModifiedAdsrComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);   // clear the background
}

void ModifiedAdsrComponent::resized()
{
    juce::Rectangle<int> slidersBounds(0, 0, UI::ADSR::sliderAreaWidth, UI::ADSR::sliderAreaHeight);
    juce::Rectangle<int> sliderBounds(0, 0, UI::GLOBAL::sliderComponentWidth, UI::GLOBAL::sliderComponentHeight);

    attackSlider.setBounds(sliderBounds);
    
    sliderBounds.setX(UI::GLOBAL::sliderComponentWidth + UI::GLOBAL::sliderComponentPadding);
    decaySlider.setBounds(sliderBounds);

    sliderBounds.setY(UI::GLOBAL::sliderComponentHeight + UI::GLOBAL::sliderComponentPadding);
    releaseSlider.setBounds(sliderBounds);

    sliderBounds.setX(0);
    sustainSlider.setBounds(sliderBounds);
}
