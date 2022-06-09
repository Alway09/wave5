/*
  ==============================================================================

    LFOComponent.cpp
    Created: 17 May 2022 12:28:13pm
    Author:  Валентин Чекулаев

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LFOComponent.h"

//==============================================================================
LFOComponent::LFOComponent(LFOData* lfo) : relativeLFO(lfo)
{
    //envelope.setEnvelopeAsLFO();
    envelope.setReltiveLFO(relativeLFO);
    addAndMakeVisible(envelope);
    
    startTimer(200);
}

LFOComponent::~LFOComponent()
{
}

void LFOComponent::timerCallback(){
    envelope.addDot(0, getHeight(), false, true, false);
    envelope.addDot(getWidth(), getHeight(), false, true, false);
    
    stopTimer();
}

void LFOComponent::paint (juce::Graphics& g)
{
    g.setColour(UI::GLOBAL::strokeLineColour);
    g.drawRect(envBounds, UI::GLOBAL::strokeLineWigthInside);
}

void LFOComponent::setSizes(){
    int x = UI::GLOBAL::paddingFromStoke;
    int y = UI::GLOBAL::paddingFromStoke;
    
    envBounds = juce::Rectangle<int>(x, y, UI::LFO_BLOCK::width - 2 * UI::GLOBAL::paddingFromStoke, getHeight() - 2 * UI::GLOBAL::paddingFromStoke);
}


void LFOComponent::resized()
{
    setSizes();
    
    envelope.setBounds(envBounds.reduced(UI::GLOBAL::strokeLineWigthInside, UI::GLOBAL::strokeLineWigthInside));
}
