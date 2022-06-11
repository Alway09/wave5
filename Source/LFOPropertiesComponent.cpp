#include <JuceHeader.h>
#include "LFOPropertiesComponent.h"

//==============================================================================
LFOPropertiesComponent::LFOPropertiesComponent(APVTS& apvts,
                                               const juce::String& rateHzControllerId,
                                               const juce::String& rateBpmControllerId,
                                               const juce::String& workingModeId,
                                               const juce::String& rateModeId)

    : rateSliderHz("Hz")
{
    trigLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    trigLabel.setText("Trig", juce::NotificationType::dontSendNotification);
    
    envLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    envLabel.setText("Env", juce::NotificationType::dontSendNotification);
    
    offLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    offLabel.setText("Off", juce::NotificationType::dontSendNotification);
    
    bpmLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    bpmLabel.setText("Bpm", juce::NotificationType::dontSendNotification);
    
    hzLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    hzLabel.setText("Hz", juce::NotificationType::dontSendNotification);
    
    rateLabelHz.setColour(juce::Label::textColourId, juce::Colours::black);
    rateLabelHz.setJustificationType(juce::Justification::centred);
    rateLabelHz.setText("Hz", juce::NotificationType::dontSendNotification);
    
    bpmChooserLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    bpmChooserLabel.setJustificationType(juce::Justification::centred);
    bpmChooserLabel.setText("Bpm", juce::NotificationType::dontSendNotification);
    
    workModeChooser.addItemList(STR_CONST::LFO::LFOWorkModes, 1);
    workModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts,
                                                                                                  workingModeId,
                                                                                                  workModeChooser);
    
    rateModeChooser.addItemList(STR_CONST::LFO::LFORateModes, 1);
    rateModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts,
                                                                                                  rateModeId,
                                                                                                  rateModeChooser);
    
    rateHzAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts,
                                                                                              rateHzControllerId,
                                                                                              rateSliderHz);
    
    bpmChooser.addItemList(STR_CONST::LFO::LFOBpmRates, 1);
    bpmChooserAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts,
                                                                                                    rateBpmControllerId,
                                                                                                    bpmChooser);
    
    bpmChooser.setColour(juce::ComboBox::textColourId, juce::Colours::black);
    
    trigToggle.addListener(this);
    envToggle.addListener(this);
    offToggle.addListener(this);
    bpmToggle.addListener(this);
    hzToggle.addListener(this);
    
    trigToggle.setToggleState(true, juce::NotificationType::dontSendNotification);
    bpmToggle.setToggleState(true, juce::NotificationType::dontSendNotification);
    
    rateSliderHz.setEnabled(false);
    
    addAndMakeVisible(trigToggle);
    addAndMakeVisible(trigLabel);
    addAndMakeVisible(envToggle);
    addAndMakeVisible(envLabel);
    addAndMakeVisible(offToggle);
    addAndMakeVisible(offLabel);
    addAndMakeVisible(bpmToggle);
    addAndMakeVisible(bpmLabel);
    addAndMakeVisible(hzLabel);
    addAndMakeVisible(hzToggle);
    addAndMakeVisible(rateSliderHz);
    addAndMakeVisible(rateLabelHz);
    addAndMakeVisible(bpmChooser);
    addAndMakeVisible(bpmChooserLabel);
}

LFOPropertiesComponent::~LFOPropertiesComponent()
{
}

void LFOPropertiesComponent::buttonClicked(juce::Button* button){
    int item = 0;
    
    item = button == &trigToggle ? 1 :
           button == &envToggle ? 2 :
           button == &offToggle ? 3 :
           button == &bpmToggle ? 4 :
           button == &hzToggle ? 5 : 0;
    
    
    switch (item) {
        case 1:
            trigToggle.setToggleState(true, juce::dontSendNotification);
            envToggle.setToggleState(false, juce::dontSendNotification);
            offToggle.setToggleState(false, juce::dontSendNotification);
            workModeChooser.setSelectedId(1);
            break;
        case 2:
            trigToggle.setToggleState(false, juce::dontSendNotification);
            envToggle.setToggleState(true, juce::dontSendNotification);
            offToggle.setToggleState(false, juce::dontSendNotification);
            workModeChooser.setSelectedId(2);
            break;
        case 3:
            trigToggle.setToggleState(false, juce::dontSendNotification);
            envToggle.setToggleState(false, juce::dontSendNotification);
            offToggle.setToggleState(true, juce::dontSendNotification);
            workModeChooser.setSelectedId(3);
            break;
        case 4:
            bpmToggle.setToggleState(true, juce::dontSendNotification);
            hzToggle.setToggleState(false, juce::dontSendNotification);
            rateModeChooser.setSelectedId(1);
            bpmChooser.setEnabled(true);
            rateSliderHz.setEnabled(false);
            break;
        case 5:
            bpmToggle.setToggleState(false, juce::dontSendNotification);
            hzToggle.setToggleState(true, juce::dontSendNotification);
            rateModeChooser.setSelectedId(2);
            bpmChooser.setEnabled(false);
            rateSliderHz.setEnabled(true);
            break;

        default:
            break;
    }
}

void LFOPropertiesComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::transparentWhite);   // clear the background
    g.drawRect(localBounds, UI::GLOBAL::strokeLineWigthInside);
}

void LFOPropertiesComponent::setSizes(){
    
    localBounds = juce::Rectangle<int>(UI::GLOBAL::paddingFromStoke,
                                       UI::GLOBAL::paddingFromStoke,
                                       UI::LFO_BLOCK::width - 2 * UI::GLOBAL::paddingFromStoke,
                                       UI::LFO_BLOCK::propHeight - 4 * UI::GLOBAL::paddingFromStoke);
    
    toggleBounds = juce::Rectangle<int>(0, 0,
                                        UI::LFO_PROPERTIES::toggleWidth + UI::LFO_PROPERTIES::labelWidth,
                                        UI::LFO_PROPERTIES::labelHeight);
    
    sliderBounds = juce::Rectangle<int>(0, 0,
                                        UI::GLOBAL::sliderComponentWidth,
                                        UI::GLOBAL::sliderComponentHeight + UI::GLOBAL::sliderLabelHeight);
    
    //toggleBounds = jce::Rectangle<int>(0, 0,)
}

void LFOPropertiesComponent::resized()
{
    setSizes();
    
    toggleBounds.setX(localBounds.getX() + UI::GLOBAL::paddingFromStoke);
    toggleBounds.setY(localBounds.getY() + UI::GLOBAL::paddingFromStoke);
    
    auto localToggleBounds = toggleBounds;
    trigToggle.setBounds(localToggleBounds.removeFromLeft(UI::LFO_PROPERTIES::toggleWidth));
    trigLabel.setBounds(localToggleBounds);
    
    toggleBounds.setY(toggleBounds.getY() + UI::LFO_PROPERTIES::labelHeight);
    
    localToggleBounds = toggleBounds;
    envToggle.setBounds(localToggleBounds.removeFromLeft(UI::LFO_PROPERTIES::toggleWidth));
    envLabel.setBounds(localToggleBounds);
    
    toggleBounds.setY(toggleBounds.getY() + UI::LFO_PROPERTIES::labelHeight);
    
    localToggleBounds = toggleBounds;
    offToggle.setBounds(localToggleBounds.removeFromLeft(UI::LFO_PROPERTIES::toggleWidth));
    offLabel.setBounds(localToggleBounds);
    
    toggleBounds.setX(toggleBounds.getX() + UI::LFO_PROPERTIES::labelWidth + UI::GLOBAL::paddingComponentsInside);
    toggleBounds.setY(localBounds.getY() + UI::GLOBAL::paddingComponentsInside);
    
    localToggleBounds = toggleBounds;
    bpmToggle.setBounds(localToggleBounds.removeFromLeft(UI::LFO_PROPERTIES::toggleWidth));
    bpmLabel.setBounds(localToggleBounds);
    
    toggleBounds.setY(toggleBounds.getY() + UI::LFO_PROPERTIES::labelHeight);
    
    localToggleBounds = toggleBounds;
    hzToggle.setBounds(localToggleBounds.removeFromLeft(UI::LFO_PROPERTIES::toggleWidth));
    hzLabel.setBounds(localToggleBounds);
    
    sliderBounds.setX(toggleBounds.getX() + UI::LFO_PROPERTIES::labelWidth + UI::GLOBAL::paddingComponentsInside);
    sliderBounds.setY(UI::LFO_BLOCK::propHeight / 2 - sliderBounds.getHeight() / 2);
    
    auto localSliderBounds = sliderBounds;
    rateSliderHz.setBounds(localSliderBounds.removeFromTop(UI::GLOBAL::sliderComponentHeight));
    rateLabelHz.setBounds(localSliderBounds);
    
    sliderBounds.setX(toggleBounds.getX() + 2*UI::LFO_PROPERTIES::labelWidth + 2*UI::GLOBAL::paddingComponentsInside);
    sliderBounds.setY(UI::LFO_BLOCK::propHeight / 2 - sliderBounds.getHeight() / 2);
    
    localSliderBounds = sliderBounds;
    localSliderBounds.setWidth(localSliderBounds.getWidth() + 20);
    bpmChooser.setBounds(localSliderBounds.removeFromTop(UI::GLOBAL::sliderComponentHeight).reduced(0, 15));
    bpmChooserLabel.setBounds(localSliderBounds);
    
}
