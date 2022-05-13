/*
  ==============================================================================

    ModifiedAdsrComponent.cpp
    Created: 2 May 2022 4:40:25pm
    Author:  alway

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ModifiedAdsrComponent.h"

//using Params = ModifiedAdsrData::Parameters;

//==============================================================================
ModifiedAdsrComponent::ModifiedAdsrComponent(juce::AudioProcessorValueTreeState& apvts,
                                             const juce::StringArray& idList)
    : juce::Component(),
    attackSlider("Attack"), decaySlider("Decay"), sustainSlider("Sustain"), releaseSlider("Release")
{
    //updateParameters(apvts, idArray);

    prepareSlider(attackSlider, apvts, idList[0], attackAttachment);
    prepareSlider(decaySlider, apvts, idList[1], decayAttachment);
    prepareSlider(sustainSlider, apvts, idList[2], sustainAttachment);
    prepareSlider(releaseSlider, apvts, idList[3], releaseAttachment);
    
    envelope.setEnvelopeAsADSR(&apvts, idList);
    
    attackSlider.setValue(1.0);
    decaySlider.setValue(1.0);
    sustainSlider.setValue(1.0);
    releaseSlider.setValue(1.0);

    addAndMakeVisible(envelope);
}

ModifiedAdsrComponent::~ModifiedAdsrComponent()
{
}

void ModifiedAdsrComponent::prepareSlider(juce::Slider& slider, juce::AudioProcessorValueTreeState& apvts, juce::String paramId, std::unique_ptr<SliderAttachment>& attachment)
{
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
    
    g.drawRect(getLocalBounds());
}

void ModifiedAdsrComponent::setSizes() {
    
    int x = UI::GLOBAL::paddingFromStoke;
    int y = UI::GLOBAL::paddingFromStoke;
    
    int width = UI::GLOBAL::paddingFromStoke +
                UI::GLOBAL::sliderComponentWidth +
                UI::GLOBAL::sliderComponentPadding +
                UI::GLOBAL::sliderComponentWidth +
                UI::GLOBAL::paddingFromStoke;
    
    int height = UI::GLOBAL::paddingFromStoke +
                 UI::GLOBAL::sliderComponentHeight +
                 UI::GLOBAL::sliderComponentPadding +
                 UI::GLOBAL::sliderComponentHeight +
                 UI::GLOBAL::paddingFromStoke;
    
    slidersAreaBounds = juce::Rectangle<int>(x, y, width, height);

    width = UI::GLOBAL::sliderComponentWidth;
    height = UI::GLOBAL::sliderComponentHeight;
    
    sliderBounds = juce::Rectangle<int>(0, 0, width, height);
    
    x = UI::GLOBAL::paddingFromStoke +
        slidersAreaBounds.getWidth() +
        UI::GLOBAL::paddingComponentsInside;
    
    width = UI::OSC_BLOCK::blockWidth -
            UI::GLOBAL::paddingFromStoke -
            slidersAreaBounds.getWidth() -
            UI::GLOBAL::paddingComponentsInside -
            UI::GLOBAL::paddingFromStoke;
    
    height = slidersAreaBounds.getHeight();

    envelopeBounds = juce::Rectangle<int>(x, y, width, height);
}

juce::Rectangle<int> ModifiedAdsrComponent::getCompBounds() const{
    juce::Rectangle<int> bounds;
    
    bounds.setX(0);
    bounds.setY(0);
    
    bounds.setWidth(UI::GLOBAL::paddingFromStoke +
                    slidersAreaBounds.getWidth() +
                    UI::GLOBAL::paddingComponentsInside +
                    envelopeBounds.getWidth() +
                    UI::GLOBAL::paddingFromStoke);
    
    bounds.setHeight(slidersAreaBounds.getHeight());
    
    return bounds;
}

void ModifiedAdsrComponent::resized()
{
    setSizes();
    
    juce::Rectangle<int> sliderBoundsLocal =
        sliderBounds.withX(slidersAreaBounds.getX() + UI::GLOBAL::paddingFromStoke).
        withY(slidersAreaBounds.getY() + UI::GLOBAL::paddingFromStoke);

    attackSlider.setBounds(sliderBoundsLocal);
    
    sliderBoundsLocal.setX(sliderBoundsLocal.getX() + UI::GLOBAL::sliderComponentWidth + UI::GLOBAL::sliderComponentPadding);
    decaySlider.setBounds(sliderBoundsLocal);

    sliderBoundsLocal.setY(sliderBoundsLocal.getY() + UI::GLOBAL::sliderComponentHeight + UI::GLOBAL::sliderComponentPadding);
    releaseSlider.setBounds(sliderBoundsLocal);

    sliderBoundsLocal.setX(slidersAreaBounds.getX() + UI::GLOBAL::paddingFromStoke);
    sustainSlider.setBounds(sliderBoundsLocal);

    envelope.setBounds(envelopeBounds.reduced(UI::GLOBAL::strokeLineWigthInside));
}
