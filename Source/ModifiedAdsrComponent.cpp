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
                                             const juce::StringArray& _idList)
    : juce::Component(),
    attackSlider("Attack"), decaySlider("Decay"), sustainSlider("Sustain"), releaseSlider("Release"),
    idList(_idList)
{
    //updateParameters(apvts, idArray);

    prepareSlider(attackSlider, apvts, idList[0], attackAttachment, attackLabel);
    prepareSlider(decaySlider, apvts, idList[1], decayAttachment, decayLabel);
    prepareSlider(sustainSlider, apvts, idList[2], sustainAttachment, sustainLabel);
    prepareSlider(releaseSlider, apvts, idList[3], releaseAttachment, releaseLabel);
    
    //envelope.setEnvelopeAsADSR(&apvts, idList);
    
    /*attackSlider.setValue(1.0);
    decaySlider.setValue(1.0);
    sustainSlider.setValue(1.0);
    releaseSlider.setValue(1.0);*/

    envelope.setAPVTS(&apvts);
    addAndMakeVisible(envelope);
    
    startTimer(200);
}

ModifiedAdsrComponent::~ModifiedAdsrComponent()
{
}

void ModifiedAdsrComponent::timerCallback(){
    
    envelope.addDot(0, envelope.getHeight(), false, true, true);
    int decayDot = envelope.addDot(1, 0, false, false, true);
    int sustainDot = envelope.addDot(2, 0, false, false, false);
    int releaseDot = envelope.addDot(3, envelope.getHeight(), false, false, true);
    
    envelope.setDotRelativeToParameter(idList[0],
                                       decayDot,
                                       EnvelopeVisualComponent::DotRelationDirection::Horizontal);
    envelope.setDotRelativeToParameter(idList[1],
                                       sustainDot,
                                       EnvelopeVisualComponent::DotRelationDirection::Horizontal);
    envelope.setDotRelativeToParameter(idList[2],
                                       sustainDot,
                                       EnvelopeVisualComponent::DotRelationDirection::Vertical);
    envelope.setDotRelativeToParameter(idList[3],
                                       releaseDot,
                                       EnvelopeVisualComponent::DotRelationDirection::Horizontal);
    
    //envelope.setp
    
    attackSlider.setValue(0.0);
    decaySlider.setValue(1.0);
    sustainSlider.setValue(1.0);
    releaseSlider.setValue(0.1);
    
    stopTimer();
}

void ModifiedAdsrComponent::prepareSlider(juce::Slider& slider,
                                          juce::AudioProcessorValueTreeState& apvts,
                                          juce::String paramId, std::unique_ptr<SliderAttachment>& attachment,
                                          juce::Label& sliderLabel)
{
    addAndMakeVisible(slider);
    addAndMakeVisible(sliderLabel);
    
    sliderLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
    sliderLabel.setJustificationType(juce::Justification::centred);
    sliderLabel.setText(slider.getName(), juce::NotificationType::dontSendNotification);

    attachment = std::make_unique<SliderAttachment>(apvts, paramId, slider); 
}

void ModifiedAdsrComponent::paint (juce::Graphics& g)
{
    g.fillAll (UI::GLOBAL::backColour);

    g.setColour(UI::GLOBAL::strokeLineColour);
    g.drawRect(slidersAreaBounds, UI::GLOBAL::strokeLineWigthInside);
    g.drawRect(envelopeBounds, UI::GLOBAL::strokeLineWigthInside);

    //g.drawRect(attackSlider.getBounds().expanded(3, 3));
    //g.drawRect(decaySlider.getBounds());
    
    //g.drawRect(getLocalBounds(), UI::GLOBAL::strokeLineWigthInside);
}

void ModifiedAdsrComponent::setSizes() {
    
    int width = UI::GLOBAL::sliderComponentWidth;
    int height = UI::GLOBAL::sliderComponentHeight + UI::GLOBAL::sliderLabelHeight;
    
    sliderBounds = juce::Rectangle<int>(0, 0, width, height);
    
    width = UI::GLOBAL::paddingFromStoke +
            sliderBounds.getWidth() +
            UI::GLOBAL::sliderComponentPadding +
            sliderBounds.getWidth() +
            UI::GLOBAL::paddingFromStoke;
    
    height = UI::GLOBAL::paddingFromStoke +
             sliderBounds.getHeight() +
             UI::GLOBAL::sliderComponentPadding +
             sliderBounds.getHeight() +
             UI::GLOBAL::paddingFromStoke;
    
    slidersAreaBounds = juce::Rectangle<int>(UI::GLOBAL::paddingFromStoke,
                                             UI::GLOBAL::paddingFromStoke,
                                             width,
                                             height);
    
    width = UI::OSC_BLOCK::blockWidth -
            UI::GLOBAL::paddingFromStoke -
            slidersAreaBounds.getWidth() -
            UI::GLOBAL::paddingComponentsInside -
            UI::GLOBAL::paddingFromStoke;
    
    height = slidersAreaBounds.getHeight();
    
    int x = slidersAreaBounds.getX() +
            slidersAreaBounds.getWidth() +
    UI::GLOBAL::paddingComponentsInside - UI::GLOBAL::strokeLineWigthInside;;
    
    int y = slidersAreaBounds.getY();

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

void ModifiedAdsrComponent::setCustomLookAndFeel(CustomLookAndFeel* lookAndFeel){
    attackSlider.setLookAndFeel(lookAndFeel);
    decaySlider.setLookAndFeel(lookAndFeel);
    sustainSlider.setLookAndFeel(lookAndFeel);
    releaseSlider.setLookAndFeel(lookAndFeel);
}

void ModifiedAdsrComponent::resized()
{
    setSizes();
    
    juce::Rectangle<int> sliderBoundsLocal = sliderBounds.withX(slidersAreaBounds.getX() + UI::GLOBAL::paddingFromStoke)
                                                         .withY(slidersAreaBounds.getY() + UI::GLOBAL::paddingFromStoke);
    
    auto boundsCopy = sliderBoundsLocal;
    attackSlider.setBounds(boundsCopy.removeFromTop(UI::GLOBAL::sliderComponentHeight));
    attackLabel.setBounds(boundsCopy);
    
    sliderBoundsLocal.setX(sliderBoundsLocal.getX() + sliderBounds.getWidth() + UI::GLOBAL::sliderComponentPadding);
    
    boundsCopy = sliderBoundsLocal;
    decaySlider.setBounds(boundsCopy.removeFromTop(UI::GLOBAL::sliderComponentHeight));
    decayLabel.setBounds(boundsCopy);

    sliderBoundsLocal.setY(sliderBoundsLocal.getY() + sliderBounds.getHeight() + UI::GLOBAL::sliderComponentPadding);
    
    boundsCopy = sliderBoundsLocal;
    releaseSlider.setBounds(boundsCopy.removeFromTop(UI::GLOBAL::sliderComponentHeight));
    releaseLabel.setBounds(boundsCopy);

    sliderBoundsLocal.setX(slidersAreaBounds.getY() + UI::GLOBAL::paddingFromStoke);
    
    boundsCopy = sliderBoundsLocal;
    sustainSlider.setBounds(boundsCopy.removeFromTop(UI::GLOBAL::sliderComponentHeight));
    sustainLabel.setBounds(boundsCopy);

    envelope.setBounds(envelopeBounds.reduced(UI::GLOBAL::strokeLineWigthInside));
}
