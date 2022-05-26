/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "SynthVoice.h"
#include "ModifiedAdsrData.h"
#include "StringConstants.h"
#include "ModulationMatrixData.h"

//==============================================================================
/**
*/

class Wave5AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    Wave5AudioProcessor();
    ~Wave5AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void updateAdsr(ModifiedAdsrData& adsr, const juce::StringArray& idList);

    //juce::Synthesiser& getSynthesizer() { return synth; }
    
    void updateLFO(bool voiceIsActive);

    juce::AudioProcessorValueTreeState apvts;

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    //ParametersIDs paramsIDs;
    
    std::vector<LFOData> LFOvector;
    std::vector<bool> LFOstates;
    
    ModulationatrixData modulationMatrix;
    juce::Synthesiser synth;
    
    unsigned int numberOfVoices = 1;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Wave5AudioProcessor)
};
