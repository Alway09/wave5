#include "ModulationMatrixData.h"

ModulationatrixData::ModulationatrixData(juce::AudioProcessorValueTreeState* tree)
 : apvts(tree)
{
    
}

void ModulationatrixData::applyEnvelopes(juce::AudioPlayHead *playHead){
    for(auto ID : modulatedParameters){
        auto& param = *apvts->getParameter(ID);
        
        //DBG(initalValues[ID] * LFO1.getEnvelopeValue(playHead));
        auto& range = param.getNormalisableRange();
        float delta = LFO1.getEnvelopeValue(playHead) * range.getRange().getLength() * modulationDepthLFO1[ID];
        param.setValueNotifyingHost(range.convertTo0to1(initalValues[ID] + delta));
    }
}

void ModulationatrixData::applyEnvelopesForVoice(SynthVoice* voice){
    uint64_t workingTime = voice->getWorkingTime();
    if(workingTime != 0){
        //DBG(voice->getFirstLFO().getEnvelopeValue(workingTime));
        for(auto ID : modulatedParameters){
            auto& param = *apvts->getParameter(ID);
            
            //DBG(initalValues[ID] * LFO1.getEnvelopeValue(playHead));
            auto& range = param.getNormalisableRange();
            float delta = LFO1.getEnvelopeValue(workingTime) * range.getRange().getLength() * modulationDepthLFO1[ID];
            param.setValueNotifyingHost(range.convertTo0to1(initalValues[ID] + delta));
        }
    }
}

void ModulationatrixData::restoreValues(){
    for(auto ID : modulatedParameters){
        auto& param = *apvts->getParameter(ID);
        param.setValueNotifyingHost(initalValues[ID]);
    }
}

void ModulationatrixData::parameterChanged(const juce::String &parameterID, float newValue){
    initalValues[parameterID] = newValue;
    DBG(initalValues[parameterID]);
}
