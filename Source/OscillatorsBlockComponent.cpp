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
    juce::TabbedComponent(juce::TabbedButtonBar::Orientation::TabsAtTop),
    firstPageComponent(apvts,
                       STR_CONST::ADSR::firstAdsrParameters,
                       STR_CONST::ADSR::firstOscWaveCoose,
                       STR_CONST::ADSR::firstOscGain),
    secondPageComponent(apvts,
                        STR_CONST::ADSR::secondAdsrParameters,
                        STR_CONST::ADSR::secondOscWaveCoose,
                        STR_CONST::ADSR::secondOscGain),
    thirdPageComponent(apvts,
                       STR_CONST::ADSR::thirdAdsrParameters,
                       STR_CONST::ADSR::thirdOscWaveCoose,
                       STR_CONST::ADSR::thirdOscGain)
{
    addTab("OSC 1", UI::GLOBAL::backColour, &firstPageComponent, true);
    addTab("OSC 2", UI::GLOBAL::backColour, &secondPageComponent, true);
    addTab("OSC 3", UI::GLOBAL::backColour, &thirdPageComponent, true);
    
    // for OSC 1
    firstToggle = new juce::ToggleButton();
    firstOscStateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            apvts,
            STR_CONST::ADSR::firstOscOn,
            *firstToggle);
    firstToggle->setBounds(0, 0, UI::OSC_BLOCK::toggleWidth, UI::OSC_BLOCK::toggleHeight);
    getTabbedButtonBar().getTabButton(0)->setExtraComponent(firstToggle, juce::TabBarButton::ExtraComponentPlacement::afterText);
    
    // for OSC 2
    secondToggle = new juce::ToggleButton();
    secondOscStateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            apvts,
            STR_CONST::ADSR::secondOscOn,
            *secondToggle);
    secondToggle->setBounds(0, 0, UI::OSC_BLOCK::toggleWidth, UI::OSC_BLOCK::toggleHeight);
    
    getTabbedButtonBar().getTabButton(1)->setExtraComponent(secondToggle, juce::TabBarButton::ExtraComponentPlacement::afterText);
    
    // for OSC 3
    thirdToggle = new juce::ToggleButton();
    thirdOscStateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            apvts,
            STR_CONST::ADSR::thirdOscOn,
            *thirdToggle);
    
    thirdToggle->setBounds(0, 0, UI::OSC_BLOCK::toggleWidth, UI::OSC_BLOCK::toggleHeight);
    
    getTabbedButtonBar().getTabButton(2)->setExtraComponent(thirdToggle, juce::TabBarButton::ExtraComponentPlacement::afterText);
    
}

OscillatorsBlockComponent::~OscillatorsBlockComponent()
{
    //delete firstToggle;
}

void OscillatorsBlockComponent::setCustomLookAndFeel(CustomLookAndFeel* lookAndFeel){
    firstPageComponent.setCustomLookAndFeel(lookAndFeel);
    secondPageComponent.setCustomLookAndFeel(lookAndFeel);
    thirdPageComponent.setCustomLookAndFeel(lookAndFeel);
    
    firstToggle->setLookAndFeel(lookAndFeel);
    secondToggle->setLookAndFeel(lookAndFeel);
    thirdToggle->setLookAndFeel(lookAndFeel);
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
    oscPropertiesComponent.setBounds(properties);
    adsrComponent.setBounds(localBounds);
}

void OscillatorsBlockComponent::PageComponent::setCustomLookAndFeel(CustomLookAndFeel* lookAndFeel){
    adsrComponent.setCustomLookAndFeel(lookAndFeel);
    oscPropertiesComponent.setCustomLookAndFeel(lookAndFeel);
}
