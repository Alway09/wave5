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
    : juce::Component(), name(adsrName), envelope(true),
    attackSlider("Attack"), decaySlider("Decay"), sustainSlider("Sustain"), releaseSlider("Release")
{
    //updateParameters(apvts, idArray);

    prepareSlider(attackSlider, apvts, Params::idList[Params::ParametersIDs::attackTimeSeconds], attackAttachment);
    prepareSlider(decaySlider, apvts, Params::idList[Params::ParametersIDs::decayTimeSeconds], decayAttachment);
    prepareSlider(sustainSlider, apvts, Params::idList[Params::ParametersIDs::sustainLevelNormalised], sustainAttachment);
    prepareSlider(releaseSlider, apvts, Params::idList[Params::ParametersIDs::releaseTimeSeconds], releaseAttachment);
    
    attackSlider.addListener(&envelope);
    decaySlider.addListener(&envelope);
    sustainSlider.addListener(&envelope);
    releaseSlider.addListener(&envelope);
    
    attackSlider.setValue(1.0);
    decaySlider.setValue(1.0);
    sustainSlider.setValue(1.0);
    releaseSlider.setValue(1.0);

    addAndMakeVisible(envelope);
}

ModifiedAdsrComponent::~ModifiedAdsrComponent()
{
}

void ModifiedAdsrComponent::prepareSlider(juce::Slider& slider, juce::AudioProcessorValueTreeState& apvts, juce::String paramId, std::unique_ptr<SliderAttachment>& attachment) {
    addAndMakeVisible(slider);

    attachment = std::make_unique<SliderAttachment>(apvts, paramId, slider); 
}

void ModifiedAdsrComponent::paint (juce::Graphics& g)
{
    g.fillAll (UI::GLOBAL::backColour);

    g.setColour(UI::GLOBAL::strokeLineColour);
    g.drawRect(slidersAreaBounds, UI::GLOBAL::strokeLineWigthInside);
    g.drawRect(envelopeBounds, UI::GLOBAL::strokeLineWigthInside);

    g.drawRect(attackSlider.getBounds());
    g.drawRect(decaySlider.getBounds());
}

void ModifiedAdsrComponent::setSizes() {
    //slidersAreaBounds = juce::Rectangle<int>(0, 0, UI::ADSR::sliderAreaWidth, UI::ADSR::sliderAreaHeight);
    slidersAreaBounds = juce::Rectangle<int>(0, 0,
        UI::GLOBAL::paddingFromStoke + UI::GLOBAL::sliderComponentPadding + UI::GLOBAL::sliderComponentWidth * 2 + UI::GLOBAL::paddingFromStoke,
        UI::GLOBAL::paddingFromStoke + UI::GLOBAL::sliderComponentPadding + UI::GLOBAL::sliderComponentHeight * 2 + UI::GLOBAL::paddingFromStoke);

    sliderBounds = juce::Rectangle<int>(0, 0, UI::GLOBAL::sliderComponentWidth, UI::GLOBAL::sliderComponentHeight);

    envelopeBounds = juce::Rectangle<int>(slidersAreaBounds.getWidth() + UI::GLOBAL::paddingComponentsInside, 0, UI::ADSR::envelopeWidth, slidersAreaBounds.getHeight());
}

void ModifiedAdsrComponent::resized()
{
    setSizes();

    juce::Rectangle<int> slidersAreaBoundsLocal = slidersAreaBounds.reduced(UI::GLOBAL::paddingFromStoke, UI::GLOBAL::paddingFromStoke);
    juce::Rectangle<int> sliderBoundsLocal = sliderBounds.withX(slidersAreaBoundsLocal.getX()).withY(slidersAreaBoundsLocal.getY());

    attackSlider.setBounds(sliderBoundsLocal);
    
    sliderBoundsLocal.setX(sliderBoundsLocal.getX() + UI::GLOBAL::sliderComponentWidth + UI::GLOBAL::sliderComponentPadding);
    decaySlider.setBounds(sliderBoundsLocal);

    sliderBoundsLocal.setY(sliderBoundsLocal.getY() + UI::GLOBAL::sliderComponentHeight + UI::GLOBAL::sliderComponentPadding);
    releaseSlider.setBounds(sliderBoundsLocal);

    sliderBoundsLocal.setX(slidersAreaBoundsLocal.getX());
    sustainSlider.setBounds(sliderBoundsLocal);

    envelope.setBounds(envelopeBounds.reduced(UI::GLOBAL::strokeLineWigthInside));
}
