#pragma once
#include <JuceHeader.h>
#include "LFOData.h"
#include "SynthVoice.h"
#include "StringConstants.h"

class ModulationMatrixData : public juce::AudioProcessorValueTreeState::Listener
{
public:
    ModulationMatrixData(juce::AudioProcessorValueTreeState* apvts);
    ~ModulationMatrixData(){};
    
    void addModulatedParameter(const juce::String& paramId, const juce::String& modulator){
        if(modulatedParameters.indexOf(paramId) == -1){
            modulatedParameters.add(paramId);
            apvts->addParameterListener(paramId, this);
        }
                    
        auto paramValue = apvts->getRawParameterValue(paramId)->load();
        
        initalValues[paramId] = paramValue;
        //targetValues[paramId].setTargetValue(paramValue);
        //DBG(apvts->getRawParameterValue(paramId)->load());
        
        modulationDepth[modulator][paramId] = 0.f;
    };
    
    void updateDepth(const juce::String& paramId, const juce::String& modulator, float newValue){
        modulationDepth[modulator][paramId] = newValue;
    }
    
    void removeModulatedParameter(const juce::String& paramId, const juce::String& modulator){
        if(modulatedParameters.indexOf(paramId) != -1){
            modulatedParameters.removeString(paramId);
            apvts->removeParameterListener(paramId, this);
        }
    }
    
    //void prepare(double sampleRate);
    
    void restoreValues();
    
    void parameterChanged(const juce::String &parameterID, float newValue) override;
    
    void applyLFO(std::vector<LFOData>& LFOvector, int bufferSize, double sampleRate);
    
private:
    juce::StringArray modulatedParameters;
    juce::AudioProcessorValueTreeState* apvts;

    std::map<juce::String, float> initalValues;
    std::map<juce::String, std::map<juce::String, float>> modulationDepth;
    
    bool valuesRestored = true;
};
