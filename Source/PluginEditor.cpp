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
      matrixComponent(p.allRangedParametersIDs, p.modulationMatrixAtomic),
      equalizer(p.apvts), globalVolumeSlider("Vol")
{
    am_of_parameters = p.allRangedParametersIDs.size();
    //DBG(p.allRangedParametersIDs.size());
    setSize (UI::GLOBAL::pluginWidth, UI::GLOBAL::pluginHeight);
    
    globalVolumeLabel.setText("Vol", juce::NotificationType::dontSendNotification);
    globalVolumeLabel.setJustificationType(juce::Justification::centred);
    globalVolumeLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    
    glideLabel.setText("Glide", juce::NotificationType::dontSendNotification);
    glideLabel.setJustificationType(juce::Justification::centred);
    glideLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    
    lookAndFeel = new CustomLookAndFeel();
    oscBlock.setCustomLookAndFeel(lookAndFeel);
    lfoBlock.setCustomLookAndFeel(lookAndFeel);
    matrixComponent.setCustomLookAndFeel(lookAndFeel);
    matrixViewport.setLookAndFeel(lookAndFeel);
    globalVolumeSlider.setLookAndFeel(lookAndFeel);
    glideSlider.setLookAndFeel(lookAndFeel);
    glideSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 40, 20);
    
    glideAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.apvts,
                                                                                             STR_CONST::glideTime,
                                                                                             glideSlider);
    
    globalVolumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.apvts,
                                                                                                    STR_CONST::globalVolume,
                                                                                                    globalVolumeSlider);
    
    addAndMakeVisible(oscBlock);
    addAndMakeVisible(lfoBlock);
    //addAndMakeVisible(matrixComponent);
    matrixViewport.setSize(UI::MODULATION_MATRIX::width - 2 * UI::GLOBAL::paddingFromStoke - 3*UI::GLOBAL::strokeLineWigthInside,
                           UI::MODULATION_MATRIX::height - 2 * UI::GLOBAL::paddingFromStoke - 2*UI::GLOBAL::strokeLineWigthInside);
    matrixViewport.setViewedComponent(&matrixComponent, false);
    addAndMakeVisible(matrixViewport);
    
    equalizer.setLookAndFeel(lookAndFeel);
    addAndMakeVisible(equalizer);
    addAndMakeVisible(globalVolumeSlider);
    addAndMakeVisible(globalVolumeLabel);
    addAndMakeVisible(glideSlider);
    addAndMakeVisible(glideLabel);
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
    
    g.drawRect(globalPropBounds, UI::GLOBAL::strokeLineWigthOutside);
}

void Wave5AudioProcessorEditor::resized()
{
    //adsrComponent.setBounds(getLocalBounds());
    auto oscBlockBounds = juce::Rectangle<int>(0, 0, UI::OSC_BLOCK::blockWidth, UI::OSC_BLOCK::blockHeight);
    oscBlock.setBounds(oscBlockBounds);
    
    auto lfoBlockBounds = juce::Rectangle<int>(UI::OSC_BLOCK::blockWidth - UI::GLOBAL::strokeLineWigthOutside, 0,
                                               UI::LFO_BLOCK::width, UI::LFO_BLOCK::height);
    
    lfoBlock.setBounds(lfoBlockBounds);
    
    auto matrixBounds = juce::Rectangle<int>(
                    UI::GLOBAL::paddingFromStoke + 3,
                    UI::OSC_BLOCK::blockHeight + UI::GLOBAL::paddingFromStoke,
                    UI::MODULATION_MATRIX::width - 2 * UI::GLOBAL::paddingFromStoke - 3*UI::GLOBAL::strokeLineWigthInside,
                    UI::MODULATION_MATRIX::height - 2 * UI::GLOBAL::paddingFromStoke - 2*UI::GLOBAL::strokeLineWigthInside);

    matrixViewport.setBounds(matrixBounds);
    matrixComponent.setBounds(matrixBounds.withHeight((am_of_parameters + 1) * UI::MODULATION_MATRIX::labelHeight).
                              withWidth(matrixBounds.getWidth() - 8));
    
    auto eqBounds = juce::Rectangle<int>(1+UI::GLOBAL::paddingFromStoke + matrixBounds.getX() + matrixBounds.getWidth(),
                                         lfoBlockBounds.getY() + lfoBlockBounds.getHeight() - UI::GLOBAL::strokeLineWigthInside,
                                         UI::EQ::width, UI::EQ::height);
    
    equalizer.setBounds(eqBounds);
    
    globalPropBounds = juce::Rectangle<int>(eqBounds.getX(),
                                            eqBounds.getY() + eqBounds.getHeight() - UI::GLOBAL::strokeLineWigthOutside,
                                            eqBounds.getWidth(), 84);
    
    auto gBoundsLocal = globalPropBounds;
    auto r = gBoundsLocal.removeFromRight(70).withX(gBoundsLocal.getX() +
                                                   UI::GLOBAL::paddingFromStoke).withY(gBoundsLocal.getY() + UI::GLOBAL::paddingFromStoke);
    globalVolumeSlider.setBounds(r.removeFromTop(50));
    globalVolumeLabel.setBounds(r);
    
    gBoundsLocal.setX(r.getX() + r.getWidth() + UI::GLOBAL::paddingComponentsInside + 50);
    gBoundsLocal.setWidth(300);
    glideSlider.setBounds(gBoundsLocal.removeFromBottom(45));
    glideLabel.setBounds(gBoundsLocal);
}
