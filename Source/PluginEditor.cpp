/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Wave5AudioProcessorEditor::Wave5AudioProcessorEditor (Wave5AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), oscBlock(p.apvts)
{
    setSize (800, 800);

    //addAndMakeVisible(adsrComponent);
    addAndMakeVisible(oscBlock);
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
    //adsrComponent.setBounds(getLocalBounds());
    oscBlock.setBounds(juce::Rectangle<int>(0, 0, UI::OSC_BLOCK::blockWidth, UI::OSC_BLOCK::blockHeight));
}
