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

    addAndMakeVisible(adsrComponent);
}

Wave5AudioProcessorEditor::~Wave5AudioProcessorEditor()
{
}

//==============================================================================
void Wave5AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void Wave5AudioProcessorEditor::resized()
{
    adsrComponent.setBounds(getLocalBounds());
}
