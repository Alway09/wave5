#include "ModulationMatrixData.h"

ModulationatrixData::ModulationatrixData(juce::AudioProcessorValueTreeState* tree)
 : apvts(tree)
{
    
}

void ModulationatrixData::applyEnvelopesForVoice(int voiceId){
    auto voiceIter = voices.find(voiceId);
    if(voiceIter == voices.end())
        return;
    
    uint64_t workingTime = voiceIter->second.load()->getWorkingTime();
    if(workingTime != 0){
        
        for(auto ID : modulatedParameters){
            auto& param = *apvts->getParameter(ID);
            
            auto& range = param.getNormalisableRange();
           
            float delta = 0;
            
            if(apvts->getRawParameterValue(STR_CONST::LFO::firstLFOOn)->load() > 0.9f){
                delta += voiceIter->second.load()->getFirstLFO().getEnvelopeValue(workingTime) *
                           range.getRange().getLength() *
                           modulationDepthLFO1[ID];
                DBG(1);
                DBG(delta);
            }
            
            if(apvts->getRawParameterValue(STR_CONST::LFO::secondLFOOn)->load() > 0.9f){
                delta += voiceIter->second.load()->getSecondLFO().getEnvelopeValue(workingTime) *
                           range.getRange().getLength() *
                           modulationDepthLFO2[ID];
                DBG(2);
                DBG(voiceIter->second.load()->getSecondLFO().getEnvelopeValue(workingTime));
                DBG(delta);
            }
            
            if(apvts->getRawParameterValue(STR_CONST::LFO::thirdLFOOn)->load() > 0.9f){
                delta += voiceIter->second.load()->getThirdLFO().getEnvelopeValue(workingTime) *
                           range.getRange().getLength() *
                           modulationDepthLFO3[ID];
                
                DBG(3);
                DBG(delta);
            }
            
            
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
