/*
  ==============================================================================

    LFOData.cpp
    Created: 16 May 2022 12:34:49pm
    Author:  Валентин Чекулаев

  ==============================================================================
*/

#include "LFOData.h"

LFOData::LFOData(){
    // up
    periodsVect.push_back(std::make_pair(std::make_pair(0.f, 0.5f), std::make_pair(0.f, 1.f)));
    
    // down
    periodsVect.push_back(std::make_pair(std::make_pair(0.5f, 1.0f), std::make_pair(1.f, 0.f)));
}

LFOData::~LFOData(){
    
}

float LFOData::getEnvelopeValue(juce::AudioPlayHead* playHead){
    float envelopeVal = 0;
    if(playHead != nullptr){
        juce::AudioPlayHead::CurrentPositionInfo info;
        playHead->getCurrentPosition(info);
        
        //current pos in beat
        float currentLFOPos = std::fmod(info.ppqPosition, 1.0);
        
        std::vector<PeriodType>::iterator periodIter = periodsVect.begin();
        while (periodIter != periodsVect.end())
        {
            if(periodIter->first.first <= currentLFOPos &&
               periodIter->first.second > currentLFOPos)
            {
                double currentRate = (periodIter->second.second - periodIter->second.first) *
                                     (currentLFOPos - periodIter->first.first) /
                                     (periodIter->first.second - periodIter->first.first);
                if(currentRate < 0.0)
                    currentRate = periodIter->second.first + currentRate;
                
                envelopeVal = currentRate;
                break;
            }
            ++periodIter;
        }
    }
    
    return envelopeVal;
}

float LFOData::getEnvelopeValue(uint64_t voiceWorkingTime){
    float envelopeVal = 0;
    
    //double samplesPerBeat = info.bpm / 60.0 * sampleRate;
    
    double currentLFOPos = info.bpm / 60.0 * (voiceWorkingTime / 1000.0);
    
    currentLFOPos = std::fmod(currentLFOPos, 1.0);
    
    std::vector<PeriodType>::iterator periodIter = periodsVect.begin();
    while (periodIter != periodsVect.end())
    {
        if(periodIter->first.first <= currentLFOPos &&
           periodIter->first.second > currentLFOPos)
        {
            double currentRate = (periodIter->second.second - periodIter->second.first) *
                                 (currentLFOPos - periodIter->first.first) /
                                 (periodIter->first.second - periodIter->first.first);
            if(currentRate < 0.0)
                currentRate = periodIter->second.first + currentRate;
            
            envelopeVal = currentRate;
            break;
        }
        ++periodIter;
    }
    
    return envelopeVal;
}

void LFOData::begin(){
    
}

void LFOData::end(){
    
}
