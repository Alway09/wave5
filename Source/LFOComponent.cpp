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
LFOComponent::LFOComponent(int lfoNum) : lfoNumber(lfoNum)
{
    //envelope.setEnvelopeAsLFO();
    addAndMakeVisible(envelope);
    
    startTimer(200);
}

LFOComponent::~LFOComponent()
{
}

void LFOComponent::timerCallback(){
    envelope.setEnvelopeAsLFO(voices, lfoNumber);
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
    
    envBounds = juce::Rectangle<int>(x, y, UI::LFO::envWidth, UI::LFO::envHeight);
}


void LFOComponent::resized()
{
    setSizes();
    
    envelope.setBounds(envBounds.reduced(UI::GLOBAL::strokeLineWigthInside, UI::GLOBAL::strokeLineWigthInside));
}
