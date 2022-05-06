/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Wave5AudioProcessorEditor::Wave5AudioProcessorEditor (Wave5AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    adsrComponent("ADSR", p.apvts)
{
    setSize (800, 800);
    
    DBG("It works!!");

    addAndMakeVisible(adsrComponent);
}

Wave5AudioProcessorEditor::~Wave5AudioProcessorEditor()
{
}

//==============================================================================
void Wave5AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (UI::GLOBAL::backColour);
}

void Wave5AudioProcessorEditor::resized()
{
    adsrComponent.setBounds(getLocalBounds());
}
