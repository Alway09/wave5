/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Wave5AudioProcessorEditor::Wave5AudioProcessorEditor (Wave5AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), oscBlock(p.apvts),
      lfoBlock(p.apvts, &p.LFOvector[0], &p.LFOvector[1], &p.LFOvector[2])
{
    setSize (UI::GLOBAL::pluginWidth, UI::GLOBAL::pluginHeight);
    
    lookAndFeel = new CustomLookAndFeel();
    oscBlock.setCustomLookAndFeel(lookAndFeel);
    lfoBlock.setCustomLookAndFeel(lookAndFeel);
    //addAndMakeVisible(adsrComponent);
    addAndMakeVisible(oscBlock);
    addAndMakeVisible(lfoBlock);
    
    /*for(int i = 0; i < audioProcessor.synth.getNumVoices(); ++i){
        lfoBlock.addVoice(&audioProcessor.modulationMatrix.getVoice(i), i);
    }*/
}

Wave5AudioProcessorEditor::~Wave5AudioProcessorEditor()
{
    delete lookAndFeel;
}

//==============================================================================
void Wave5AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (UI::GLOBAL::backColour);
}

void Wave5AudioProcessorEditor::resized()
{
    //adsrComponent.setBounds(getLocalBounds());
    auto oscBlockBounds = juce::Rectangle<int>(0, 0, UI::OSC_BLOCK::blockWidth, UI::OSC_BLOCK::blockHeight);
    oscBlock.setBounds(oscBlockBounds);
    
    lfoBlock.setBounds(juce::Rectangle<int>(UI::OSC_BLOCK::blockWidth, 0, UI::LFO::blockWidth, UI::LFO::blockHeight));
}
