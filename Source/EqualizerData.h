#pragma once
#include "EqualizerDataHelperClasses.h"


class EqualizerData
{
public:
    EqualizerData(juce::AudioProcessorValueTreeState* tree);
    ~EqualizerData();
    
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    
    using BlockType = juce::AudioBuffer<float>;
    SingleChannelSampleFifo<BlockType> leftChannelFifo{ Channel::Left };
    SingleChannelSampleFifo<BlockType> rightChannelFifo{ Channel::Right };
    
private:
    double sampleRate = 44100.0;
    MonoChain leftChain, rightChain;
    
    juce::AudioProcessorValueTreeState* apvts;
    
    void updatePeakFilter(const ChainSettings& chainSettings);
    void updateLowCutFilters(const ChainSettings& chainSettings);
    void updateHighCutFilters(const ChainSettings& chainSettings);
    void updateFilters();
};
