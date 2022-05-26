#include "ModulationMatrixData.h"

ModulationatrixData::ModulationatrixData(juce::AudioProcessorValueTreeState* tree)
 : apvts(tree)
{
    
}

void ModulationatrixData::applyLFO(std::vector<LFOData>& LFOvector){
    auto parameterIter = modulatedParameters.begin();
    while(parameterIter != modulatedParameters.end()){
        
        auto& param = *apvts->getParameter(*parameterIter);
        auto& paramRange = param.getNormalisableRange();
        float delta = 0;
        
        auto lfoIter = LFOvector.begin();
        while(lfoIter != LFOvector.end()){
            if(lfoIter->isEnable()){
                delta += lfoIter->getEnvelopeValue() *
                         paramRange.getRange().getLength() *
                         modulationDepth[lfoIter->getName()][*parameterIter];
            }
            
            ++lfoIter;
        }
        
        //targetValues[*parameterIter].setTargetValue(paramRange.convertTo0to1(initalValues[*parameterIter] + delta));
        
        //param.setValueNotifyingHost(targetValues[*parameterIter].getNextValue());
        param.setValueNotifyingHost(paramRange.convertTo0to1(initalValues[*parameterIter] + delta));
        
        ++parameterIter;
    }
}

/*void ModulationatrixData::prepare(double sampleRate){
    auto valuesIter = targetValues.begin();
    while(valuesIter != targetValues.end()){
        valuesIter->second.reset(sampleRate, smoothSeconds);
        ++valuesIter;
    }
}*/

/*void ModulationatrixData::applyEnvelopesForVoice(int voiceId){
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
}*/

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
