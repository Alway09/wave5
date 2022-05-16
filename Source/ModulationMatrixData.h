#pragma once
#include <JuceHeader.h>
#include "LFOData.h"
#include "SynthVoice.h"

class ModulationatrixData : public juce::AudioProcessorValueTreeState::Listener
{
public:
    ModulationatrixData(juce::AudioProcessorValueTreeState* apvts);
    ~ModulationatrixData(){};
    
    void addModulatedParameter(const juce::String& paramId){
        modulatedParameters.add(paramId);
        initalValues[paramId] = apvts->getRawParameterValue(paramId)->load();
        DBG(apvts->getRawParameterValue(paramId)->load());
        modulationDepthLFO1[paramId] = 0.3f;
        //apvts->addParameterListener(paramId, this);
    };
    
    void setSampleRate(double newSampleRate){
        LFO1.setSampleRate(newSampleRate);
    }
    
    void restoreValues();
    
    void parameterChanged(const juce::String &parameterID, float newValue) override;
    
    void applyEnvelopes(juce::AudioPlayHead *playHead);
    
    void applyEnvelopesForVoice(SynthVoice* voice);
    
private:
    LFOData LFO1;
    
    juce::StringArray modulatedParameters;
    juce::AudioProcessorValueTreeState* apvts;
    
    std::map<juce::String, float> initalValues;
    std::map<juce::String, float> modulationDepthLFO1;
    
    double sampleRate = 44100.0;
};
