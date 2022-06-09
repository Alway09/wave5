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
    am_of_parameters = p.allRangedParametersIDs.size();
    //DBG(p.allRangedParametersIDs.size());
    setSize (UI::GLOBAL::pluginWidth, UI::GLOBAL::pluginHeight);
    
    lookAndFeel = new CustomLookAndFeel();
    oscBlock.setCustomLookAndFeel(lookAndFeel);
    lfoBlock.setCustomLookAndFeel(lookAndFeel);
    matrixComponent.setCustomLookAndFeel(lookAndFeel);
    matrixViewport.setLookAndFeel(lookAndFeel);
    
    
    addAndMakeVisible(oscBlock);
    addAndMakeVisible(lfoBlock);
    //addAndMakeVisible(matrixComponent);
    matrixViewport.setSize(UI::MODULATION_MATRIX::width - 2 * UI::GLOBAL::paddingFromStoke - 3*UI::GLOBAL::strokeLineWigthInside,
                           UI::MODULATION_MATRIX::height - 2 * UI::GLOBAL::paddingFromStoke - 2*UI::GLOBAL::strokeLineWigthInside);
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
    g.drawRect(matrixViewport.getBounds().expanded(UI::GLOBAL::strokeLineWigthInside, UI::GLOBAL::strokeLineWigthInside), UI::GLOBAL::strokeLineWigthInside);
    
    juce::Rectangle<int> matrixRect(0, UI::OSC_BLOCK::blockHeight - UI::GLOBAL::strokeLineWigthOutside,
                                    UI::MODULATION_MATRIX::width,
                                    UI::MODULATION_MATRIX::height + UI::GLOBAL::strokeLineWigthOutside);
    
    g.drawRect(matrixRect, UI::GLOBAL::strokeLineWigthOutside);
}

void Wave5AudioProcessorEditor::resized()
{
    //adsrComponent.setBounds(getLocalBounds());
    auto oscBlockBounds = juce::Rectangle<int>(0, 0, UI::OSC_BLOCK::blockWidth, UI::OSC_BLOCK::blockHeight);
    oscBlock.setBounds(oscBlockBounds);
    
    lfoBlock.setBounds(juce::Rectangle<int>(UI::OSC_BLOCK::blockWidth - UI::GLOBAL::strokeLineWigthOutside, 0, UI::LFO_BLOCK::width, UI::LFO_BLOCK::height));
    
    auto matrixBounds = juce::Rectangle<int>(
                    UI::GLOBAL::paddingFromStoke + 3,
                    UI::OSC_BLOCK::blockHeight + UI::GLOBAL::paddingFromStoke,
                    UI::MODULATION_MATRIX::width - 2 * UI::GLOBAL::paddingFromStoke - 3*UI::GLOBAL::strokeLineWigthInside,
                    UI::MODULATION_MATRIX::height - 2 * UI::GLOBAL::paddingFromStoke - 2*UI::GLOBAL::strokeLineWigthInside);

    matrixViewport.setBounds(matrixBounds);
    matrixComponent.setBounds(matrixBounds.withHeight((am_of_parameters + 1) * UI::MODULATION_MATRIX::labelHeight).
                              withWidth(matrixBounds.getWidth() - 8));
}
