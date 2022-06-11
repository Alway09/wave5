#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "SynthVoice.h"
#include "ModifiedAdsrData.h"
#include "StringConstants.h"
#include "ModulationMatrixData.h"
#include "EqualizerData.h"

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

    std::vector<LFOData> LFOvector;
    EqualizerData equalizer;
    juce::AudioProcessorValueTreeState apvts;
    juce::StringArray allRangedParametersIDs;
    //ModulationMatrixData modulationMatrix;
    
    std::atomic<ModulationMatrixData*> modulationMatrixAtomic;

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    
    std::vector<bool> LFOstates;
    juce::Synthesiser synth;
    
    juce::dsp::Gain<float> globalGain;
    
    unsigned int numberOfVoices = 4;
    
    //int currentBuffer = 0;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Wave5AudioProcessor)
};
