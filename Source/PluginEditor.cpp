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
      lfoBlock(p.apvts, &p.LFOvector[0], &p.LFOvector[1], &p.LFOvector[2]),
      matrixComponent(p.allRangedParametersIDs, p.modulationMatrixAtomic)
{
    //DBG(p.allRangedParametersIDs.size());
    setSize (UI::GLOBAL::pluginWidth, UI::GLOBAL::pluginHeight);
    
    lookAndFeel = new CustomLookAndFeel();
    oscBlock.setCustomLookAndFeel(lookAndFeel);
    lfoBlock.setCustomLookAndFeel(lookAndFeel);
    matrixComponent.setCustomLookAndFeel(lookAndFeel);
    
    addAndMakeVisible(oscBlock);
    addAndMakeVisible(lfoBlock);
    //addAndMakeVisible(matrixComponent);
    matrixViewport.setSize(UI::MODULATION_MATRIX::width, UI::MODULATION_MATRIX::height);
    matrixViewport.setViewedComponent(&matrixComponent, false);
    addAndMakeVisible(matrixViewport);
}

Wave5AudioProcessorEditor::~Wave5AudioProcessorEditor()
{
    delete lookAndFeel;
}

//==============================================================================
void Wave5AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (UI::GLOBAL::backColour);
    g.drawRect(matrixViewport.getBounds());
}

void Wave5AudioProcessorEditor::resized()
{
    //adsrComponent.setBounds(getLocalBounds());
    auto oscBlockBounds = juce::Rectangle<int>(0, 0, UI::OSC_BLOCK::blockWidth, UI::OSC_BLOCK::blockHeight);
    oscBlock.setBounds(oscBlockBounds);
    
    lfoBlock.setBounds(juce::Rectangle<int>(UI::OSC_BLOCK::blockWidth, 0, UI::LFO_BLOCK::width, UI::LFO_BLOCK::height));
    
    auto matrixRectangle = juce::Rectangle<int>(0, UI::OSC_BLOCK::blockHeight,
                                                UI::MODULATION_MATRIX::width,
                                                UI::MODULATION_MATRIX::height);
    //matrixViewport.setSize(matrixRectangle.getWidth(), matrixRectangle.getHeight());
    //DBG(matrixViewport.getMaximumVisibleWidth());
    matrixViewport.setBounds(matrixRectangle);
    matrixComponent.setBounds(matrixRectangle.withHeight(700));
}
