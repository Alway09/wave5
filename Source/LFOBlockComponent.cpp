/*
  ==============================================================================

    LFOBlockComponent.cpp
    Created: 17 May 2022 12:38:41pm
    Author:  Валентин Чекулаев

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LFOBlockComponent.h"

//==============================================================================
LFOBlockComponent::LFOBlockComponent(juce::AudioProcessorValueTreeState& apvts) :
juce::TabbedComponent(juce::TabbedButtonBar::Orientation::TabsAtTop), pageOne(1), pageTwo(2), pageThree(3)
{
    addTab("LFO 1", UI::GLOBAL::backColour, &pageOne, false);
    addTab("LFO 2", UI::GLOBAL::backColour, &pageTwo, false);
    addTab("LFO 3", UI::GLOBAL::backColour, &pageThree, false);
    
    juce::ToggleButton* toggle = new juce::ToggleButton();
    firstLFOStateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            apvts,
            STR_CONST::LFO::firstLFOOn,
            *toggle);
    toggle->setBounds(0, 0, UI::OSC_BLOCK::toggleWidth, 10);
    
    getTabbedButtonBar().getTabButton(0)->setExtraComponent(toggle, juce::TabBarButton::ExtraComponentPlacement::afterText);
    
    toggle = new juce::ToggleButton();
    secondLFOStateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            apvts,
            STR_CONST::LFO::secondLFOOn,
            *toggle);
    toggle->setBounds(0, 0, UI::OSC_BLOCK::toggleWidth, 10);
    
    getTabbedButtonBar().getTabButton(1)->setExtraComponent(toggle, juce::TabBarButton::ExtraComponentPlacement::afterText);
    
    toggle = new juce::ToggleButton();
    thirdLFOStateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            apvts,
            STR_CONST::LFO::thirdLFOOn,
            *toggle);
    toggle->setBounds(0, 0, UI::OSC_BLOCK::toggleWidth, 10);
    
    getTabbedButtonBar().getTabButton(2)->setExtraComponent(toggle, juce::TabBarButton::ExtraComponentPlacement::afterText);
}

LFOBlockComponent::~LFOBlockComponent()
{
}

void LFOBlockComponent::currentTabChanged (int newCurrentTabIndex, const juce::String &newCurrentTabName){
    
}

void LFOBlockComponent::popupMenuClickOnTab (int tabIndex, const juce::String &tabName){
    
}

LFOBlockComponent::PageComponent::PageComponent(int lfoNum) : lfo(lfoNum)
{
    addAndMakeVisible(lfo);
}

LFOBlockComponent::PageComponent::~PageComponent(){
    
}

void LFOBlockComponent::PageComponent::paint (juce::Graphics& g){
    //g.setColour(UI::GLOBAL::strokeLineColour);
    //g.drawRect(lfo.getBounds(), UI::GLOBAL::strokeLineWigthInside);
}

void LFOBlockComponent::PageComponent::resized(){
    lfo.setBounds(juce::Rectangle<int>(0, 0, UI::LFO::envWidth, UI::LFO::envHeight));
}
