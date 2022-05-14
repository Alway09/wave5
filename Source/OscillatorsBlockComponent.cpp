/*
  ==============================================================================

    OscillatorsBlockComponent.cpp
    Created: 10 May 2022 11:00:50am
    Author:  Валентин Чекулаев

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscillatorsBlockComponent.h"

//==============================================================================
OscillatorsBlockComponent::OscillatorsBlockComponent(juce::AudioProcessorValueTreeState& apvts) :
    juce::TabbedComponent(juce::TabbedButtonBar::Orientation::TabsAtTop)
{
    addTab("OSC 1", UI::GLOBAL::backColour, new PageComponent(apvts,
                                                              STR_CONST::ADSR::firstAdsrParameters,
                                                              STR_CONST::ADSR::firstOscWaveCoose,
                                                              STR_CONST::ADSR::firstOscGain), true);
    addTab("OSC 2", UI::GLOBAL::backColour, new PageComponent(apvts,
                                                              STR_CONST::ADSR::secondAdsrParameters,
                                                              STR_CONST::ADSR::secondOscWaveCoose,
                                                              STR_CONST::ADSR::secondOscGain), true);
    addTab("OSC 3", UI::GLOBAL::backColour, new PageComponent(apvts,
                                                              STR_CONST::ADSR::thirdAdsrParameters,
                                                              STR_CONST::ADSR::thirdOscWaveCoose,
                                                              STR_CONST::ADSR::thirdOscGain), true);
    
    // for OSC 1
    juce::ToggleButton* toggle = new juce::ToggleButton();
    firstOscStateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            apvts,
            STR_CONST::ADSR::firstOscOn,
            *toggle);
    //toggle->setToggleState(true, juce::NotificationType::dontSendNotification);
    toggle->setBounds(0, 0, UI::OSC_BLOCK::toggleWidth, 10);
    
    getTabbedButtonBar().getTabButton(0)->setExtraComponent(toggle, juce::TabBarButton::ExtraComponentPlacement::afterText);
    
    // for OSC 2
    toggle = new juce::ToggleButton();
    secondOscStateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            apvts,
            STR_CONST::ADSR::secondOscOn,
            *toggle);
    //toggle->setToggleState(true, juce::NotificationType::dontSendNotification);
    toggle->setBounds(0, 0, UI::OSC_BLOCK::toggleWidth, 10);
    
    getTabbedButtonBar().getTabButton(1)->setExtraComponent(toggle, juce::TabBarButton::ExtraComponentPlacement::afterText);
    
    // for OSC 3
    toggle = new juce::ToggleButton();
    thirdOscStateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            apvts,
            STR_CONST::ADSR::thirdOscOn,
            *toggle);
    //toggle->setToggleState(true, juce::NotificationType::dontSendNotification);
    toggle->setBounds(0, 0, UI::OSC_BLOCK::toggleWidth, 10);
    
    getTabbedButtonBar().getTabButton(2)->setExtraComponent(toggle, juce::TabBarButton::ExtraComponentPlacement::afterText);
    
}

OscillatorsBlockComponent::~OscillatorsBlockComponent()
{
}

void OscillatorsBlockComponent::currentTabChanged (int newCurrentTabIndex, const juce::String &newCurrentTabName){
    
}

void OscillatorsBlockComponent::popupMenuClickOnTab (int tabIndex, const juce::String &tabName){
    
}

OscillatorsBlockComponent::PageComponent::PageComponent(juce::AudioProcessorValueTreeState& apvts,
                                                        const juce::StringArray& idList,
                                                        const juce::String& waveChooseId,
                                                        const juce::String& gainId)
: juce::Component(), adsrComponent(apvts, idList), oscPropertiesComponent(apvts, waveChooseId, gainId)
{
    addAndMakeVisible(oscPropertiesComponent);
    addAndMakeVisible(adsrComponent);
}

OscillatorsBlockComponent::PageComponent::~PageComponent(){};


void OscillatorsBlockComponent::PageComponent::paint (juce::Graphics& g){
    //g.drawRect(getLocalBounds());
}

void OscillatorsBlockComponent::PageComponent::resized(){
    auto localBounds = getLocalBounds();
    auto properties = localBounds.removeFromTop(UI::OSC_PROPERTIES::height);
    //properties.setX(UI::GLOBAL::paddingFromStoke);
    /*localBounds.setWidth(localBounds.getWidth() - 2 * UI::GLOBAL::strokeLineWigthInside -
                         2 * UI::GLOBAL::paddingFromStoke);*/
    
    //adsrComponent.setBounds(adsrComponent.getCompBounds());
    //localBounds = adsrComponent.getCompBounds().withX(localBounds.getX()).withY(localBounds.getY());
    //DBG(adsrComponent.getCompBounds().getWidth());
    //DBG(localBounds.getWidth());
    
    oscPropertiesComponent.setBounds(properties);
    adsrComponent.setBounds(localBounds);
}
