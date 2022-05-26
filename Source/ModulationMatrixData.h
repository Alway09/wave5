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
    
    void addModulatedParameter(const juce::String& paramId, const juce::String& modulator){
        modulatedParameters.add(paramId);
        
        auto paramValue = apvts->getRawParameterValue(paramId)->load();
        
        initalValues[paramId] = paramValue;
        //targetValues[paramId].setTargetValue(paramValue);
        //DBG(apvts->getRawParameterValue(paramId)->load());
        
        modulationDepth[modulator][paramId] = 0.3f;
    };
    
    //void prepare(double sampleRate);
    
    void restoreValues();
    
    void parameterChanged(const juce::String &parameterID, float newValue) override;
    
    void applyLFO(std::vector<LFOData>& LFOvector);
    
private:
    juce::StringArray modulatedParameters;
    juce::AudioProcessorValueTreeState* apvts;
    
    //std::map<int, std::atomic<SynthVoice*>> voices;
    
    //juce::LinearSmoothedValue<float> smoothValue;
    
    //std::map<juce::String, juce::LinearSmoothedValue<float>> targetValues;
    std::map<juce::String, float> initalValues;
    std::map<juce::String, std::map<juce::String, float>> modulationDepth;
    //std::map<juce::String, float> modulationDepthLFO1;
    //std::map<juce::String, float> modulationDepthLFO2;
    //std::map<juce::String, float> modulationDepthLFO3;
    
    //double smoothSeconds = 0.0001;
    
    //double sampleRate = 44100.0;
};
