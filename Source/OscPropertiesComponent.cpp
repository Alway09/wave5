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
#include "BinaryData.h"

//==============================================================================
OscPropertiesComponent::OscPropertiesComponent(juce::AudioProcessorValueTreeState& apvts,
                                               const juce::String& chooseId,
                                               const juce::String& gainId,
                                               const juce::String& transposeId,
                                               const juce::String& panId)
    : gainSlider("Gain"), transposeSlider("Trans"), panSlider("Pan")
{
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts,
                                                                                            gainId,
                                                                                            gainSlider);
    
    transposeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts,
                                                                                                 transposeId,
                                                                                                 transposeSlider);
    
    panAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts,
                                                                                           panId,
                                                                                           panSlider);
    
    waveChooser.addItemList(STR_CONST::ADSR::oscWavesVariants, 1);
    waveChooserAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts,
                                                                                                     chooseId,
                                                                                                     waveChooser);
    
    sineWaveButton.onClick = [this](){ waveChooser.setSelectedId(1); };
    squareWaveButton.onClick = [this](){ waveChooser.setSelectedId(2); };
    sawWaveButton.onClick = [this](){ waveChooser.setSelectedId(3); };
    triangleWaveButton.onClick = [this](){ waveChooser.setSelectedId(4); };
    noiseWaveButton.onClick = [this](){ waveChooser.setSelectedId(5); };
    
    prepareButton(sineWaveButton, sineWaveImage, "/Users/alvvay/Documents/JUCE Projects/wave5/Images/Sine Wave.png");
    prepareButton(squareWaveButton, squareWaveImage, "/Users/alvvay/Documents/JUCE Projects/wave5/Images/Square Wave.png");
    prepareButton(sawWaveButton, sawWaveImage, "/Users/alvvay/Documents/JUCE Projects/wave5/Images/Saw Wave.png");
    prepareButton(triangleWaveButton, triangleWaveImage, "/Users/alvvay/Documents/JUCE Projects/wave5/Images/Triange Wave.png");
    prepareButton(noiseWaveButton, noiseWaveImage, "/Users/alvvay/Documents/JUCE Projects/wave5/Images/Noise Wave.png");
    
    gainLabel.setText(gainSlider.getName(), juce::NotificationType::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    
    transposeLabel.setText(transposeSlider.getName(), juce::NotificationType::dontSendNotification);
    transposeLabel.setJustificationType(juce::Justification::centred);
    transposeLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    
    panLabel.setText(panSlider.getName(), juce::NotificationType::dontSendNotification);
    panLabel.setJustificationType(juce::Justification::centred);
    panLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    
    gainSlider.setTextValueSuffix(" dB");
    transposeSlider.setTextValueSuffix(" st");
    
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(gainLabel);
    
    addAndMakeVisible(transposeSlider);
    addAndMakeVisible(transposeLabel);
    
    addAndMakeVisible(panSlider);
    addAndMakeVisible(panLabel);
}

OscPropertiesComponent::~OscPropertiesComponent()
{
    
}

void OscPropertiesComponent::prepareButton(juce::ImageButton& button, juce::Image& image, const juce::String& imageName)
{
    image = juce::ImageFileFormat::loadFrom(juce::File(imageName));
    
    //button.setToggleable(true);
    
    button.setImages(false, true, true,
                     image, 1.f, juce::Colours::transparentWhite,
                     image, 0.6f, juce::Colours::transparentWhite,
                     image, 0.3f, juce::Colours::transparentWhite);
    
    addAndMakeVisible(button);
}

void OscPropertiesComponent::buttonClicked(juce::Button * button){
    
}

void OscPropertiesComponent::setCustomLookAndFeel(CustomLookAndFeel* lookAndFeel){
    gainSlider.setLookAndFeel(lookAndFeel);
    transposeSlider.setLookAndFeel(lookAndFeel);
}

void OscPropertiesComponent::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::black);
    g.drawRect(getBounds().reduced(UI::GLOBAL::paddingFromStoke), UI::GLOBAL::strokeLineWigthInside);
    
    //if(sineWaveButton.getToggleState())
        g.drawRect(sineWaveButton.getBounds());
    
    g.drawRect(squareWaveButton.getBounds());
    g.drawRect(sawWaveButton.getBounds());
    g.drawRect(triangleWaveButton.getBounds());
    g.drawRect(noiseWaveButton.getBounds());
    
    //g.drawRect(gainSlider.getBounds().expanded(3, 3));
}

void OscPropertiesComponent::setSizes(){
    int x = UI::GLOBAL::paddingFromStoke * 2;
    int y = UI::OSC_PROPERTIES::height / 2 - UI::OSC_PROPERTIES::buttonSize / 2;
    int width = UI::OSC_PROPERTIES::buttonSize;
    int height = width;
    
    buttonBounds = juce::Rectangle<int>(x,y, width, height);
    
    x = buttonBounds.getX() +
        buttonBounds.getWidth() * 5 +
        UI::GLOBAL::paddingComponentsInside * 5;
    y = getHeight() / 2 - (UI::GLOBAL::sliderComponentHeight + UI::GLOBAL::sliderLabelHeight) / 2;
    width = UI::GLOBAL::sliderComponentWidth;
    height = UI::GLOBAL::sliderComponentHeight + UI::GLOBAL::sliderLabelHeight;
    
    sliderBounds = juce::Rectangle<int>(x, y, width, height);
}

void OscPropertiesComponent::resized(){
    
    setSizes();
    
    /*juce::Rectangle<int> buttonBounds(UI::GLOBAL::paddingFromStoke,
                                      UI::OSC_PROPERTIES::height / 2 - UI::OSC_PROPERTIES::buttonSize / 2,
                                      UI::OSC_PROPERTIES::buttonSize, UI::OSC_PROPERTIES::buttonSize);*/
    auto localButtonBounds = buttonBounds;
    
    sineWaveButton.setBounds(localButtonBounds);
    
    //buttonBounds = localBounds.removeFromLeft(UI::OSC_PROPERTIES::height);
    localButtonBounds.setX(localButtonBounds.getX() + buttonBounds.getWidth() + UI::GLOBAL::paddingComponentsInside);
    
    squareWaveButton.setBounds(localButtonBounds);
    
    //buttonBounds = localBounds.removeFromLeft(UI::OSC_PROPERTIES::height);
    localButtonBounds.setX(localButtonBounds.getX() + buttonBounds.getWidth() + UI::GLOBAL::paddingComponentsInside);
    
    sawWaveButton.setBounds(localButtonBounds);
    
    //buttonBounds = localBounds.removeFromLeft(UI::OSC_PROPERTIES::height);
    localButtonBounds.setX(localButtonBounds.getX() + buttonBounds.getWidth() + UI::GLOBAL::paddingComponentsInside);

    triangleWaveButton.setBounds(localButtonBounds);
    
    //buttonBounds = localBounds.removeFromLeft(UI::OSC_PROPERTIES::height);
    localButtonBounds.setX(localButtonBounds.getX() + buttonBounds.getWidth() + UI::GLOBAL::paddingComponentsInside);

    noiseWaveButton.setBounds(localButtonBounds);
    
    /*juce::Rectangle<int> sliderBounds(buttonBounds.getX() + buttonBounds.getWidth() + UI::GLOBAL::paddingComponentsInside,
                                      getHeight() / 2 - (UI::GLOBAL::sliderComponentHeight + UI::GLOBAL::sliderLabelHeight) / 2,
                                      UI::GLOBAL::sliderComponentWidth,
                                      UI::GLOBAL::sliderComponentHeight + UI::GLOBAL::sliderLabelHeight);*/
    
    auto localSliderBounds = sliderBounds;
    gainSlider.setBounds(localSliderBounds.removeFromTop(UI::GLOBAL::sliderComponentHeight));
    gainLabel.setBounds(localSliderBounds);
    
    localSliderBounds = sliderBounds;
    localSliderBounds.setX(localSliderBounds.getX() + localSliderBounds.getWidth() + UI::GLOBAL::paddingComponentsInside);
    transposeSlider.setBounds(localSliderBounds.removeFromTop(UI::GLOBAL::sliderComponentHeight));
    transposeLabel.setBounds(localSliderBounds);

    localSliderBounds = sliderBounds;
    localSliderBounds.setX(localSliderBounds.getX() + 2*localSliderBounds.getWidth() + 2*UI::GLOBAL::paddingComponentsInside);
    panSlider.setBounds(localSliderBounds.removeFromTop(UI::GLOBAL::sliderComponentHeight));
    panLabel.setBounds(localSliderBounds);
}
