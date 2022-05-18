#pragma once
#include <JuceHeader.h>
#include "LFOData.h"
#include "SynthVoice.h"
#include "StringConstants.h"

class ModulationatrixData : public juce::AudioProcessorValueTreeState::Listener
{
public:
    ModulationatrixData(juce::AudioProcessorValueTreeState* apvts);
    ~ModulationatrixData(){};
    
    void addModulatedParameter(const juce::String& paramId){
        modulatedParameters.add(paramId);
        initalValues[paramId] = apvts->getRawParameterValue(paramId)->load();
        //DBG(apvts->getRawParameterValue(paramId)->load());
        
        modulationDepthLFO1[paramId] = 0.3f;
        modulationDepthLFO2[paramId] = 0.3f;
        modulationDepthLFO3[paramId] = 0.3f;
    };
    
    void restoreValues();
    
    void parameterChanged(const juce::String &parameterID, float newValue) override;
    
    void addVoice(SynthVoice* voice, int voiceId){ voices[voiceId] = voice; }
    
    std::atomic<SynthVoice*>& getVoice(int voiceId){ return voices[voiceId]; }
    
    void applyEnvelopesForVoice(int voiceId);
    
private:
    juce::StringArray modulatedParameters;
    juce::AudioProcessorValueTreeState* apvts;
    
    std::map<int, std::atomic<SynthVoice*>> voices;
    
    std::map<juce::String, float> initalValues;
    std::map<juce::String, float> modulationDepthLFO1;
    std::map<juce::String, float> modulationDepthLFO2;
    std::map<juce::String, float> modulationDepthLFO3;
    
    double sampleRate = 44100.0;
};
