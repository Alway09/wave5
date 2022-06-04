#include "ModulationMatrixData.h"

ModulationMatrixData::ModulationMatrixData(juce::AudioProcessorValueTreeState* tree)
 : apvts(tree)
{
    
}

void ModulationMatrixData::applyLFO(std::vector<LFOData>& LFOvector, int bufferSize, double sampleRate){
    auto parameterIter = modulatedParameters.begin();
    while(parameterIter != modulatedParameters.end()){
        
        auto& param = *apvts->getParameter(*parameterIter);
        auto& paramRange = param.getNormalisableRange();
        float delta = 0;
        
        auto lfoIter = LFOvector.begin();
        while(lfoIter != LFOvector.end()){
            if(lfoIter->isEnable()){
                delta += lfoIter->calculateEnvelopeValue(bufferSize, sampleRate) *
                         paramRange.getRange().getLength() *
                         modulationDepth[lfoIter->getName()][*parameterIter];
            }
            
            ++lfoIter;
        }
        
        if(std::abs(delta) <= 0.000001f){
            if(!valuesRestored)
                restoreValues();
            return;
        }
        
        //targetValues[*parameterIter].setTargetValue(paramRange.convertTo0to1(initalValues[*parameterIter] + delta));
        
        //param.setValueNotifyingHost(targetValues[*parameterIter].getNextValue());
        apvts->removeParameterListener(*parameterIter, this);
        
        float newValue = initalValues[*parameterIter] + delta;
        if(newValue > paramRange.getRange().getEnd()){
            newValue = paramRange.getRange().getEnd();
        }else if (newValue < paramRange.getRange().getStart()){
            newValue = paramRange.getRange().getStart();
        }
        
        param.setValueNotifyingHost(paramRange.convertTo0to1(newValue));
        //apvts->addParameterListener(*parameterIter, this);
        
        ++parameterIter;
    }
    valuesRestored = false;
}

void ModulationMatrixData::restoreValues(){
    for(auto ID : modulatedParameters){
        auto& param = *apvts->getParameter(ID);
        
        //DBG(initalValues[ID]);
        param.setValueNotifyingHost(param.getNormalisableRange().convertTo0to1(initalValues[ID]));
        apvts->addParameterListener(ID, this);
        valuesRestored = true;
    }
}

void ModulationMatrixData::parameterChanged(const juce::String &parameterID, float newValue){
    initalValues[parameterID] = newValue;
}
