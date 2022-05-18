#include "LFOData.h"

LFOData::LFOData(){

}

LFOData::~LFOData(){
    
}

void LFOData::addPeriod(int leftID, int rightID, float xStart, float xEnd, float yStart, float yEnd){
    
    periodsVect.push_back(std::make_tuple(std::make_pair(leftID, rightID),
                                          std::make_pair(xStart, xEnd),
                                          std::make_pair(yStart, yEnd)
                                          )
                          );
}

void LFOData::deletePeriod(int leftID, int rightID){
    
    auto iter = periodsVect.begin();
    while (iter != periodsVect.end()){
        if(std::get<0>(*iter).first == leftID &&
           std::get<0>(*iter).second == rightID)
        {
            periodsVect.erase(iter);
            return;
        }
        ++iter;
    }
}

void LFOData::updatePeriod(int leftID, int rightID, float xStart, float xEnd, float yStart, float yEnd){
    auto iter = periodsVect.begin();
    
    while (iter != periodsVect.end()){
        if(std::get<0>(*iter).first == leftID &&
           std::get<0>(*iter).second == rightID)
        {
            std::get<1>(*iter).first = xStart;
            std::get<1>(*iter).second = xEnd;
            std::get<2>(*iter).first = yStart;
            std::get<2>(*iter).second = yEnd;
            
            return;
        }
        ++iter;
    }
        
}

float LFOData::getEnvelopeValue(uint64_t voiceWorkingTime){
    float envelopeVal = 0;
        
    float currentLFOPos = info.bpm / 60.f * (voiceWorkingTime / 100.f);
    
    currentLFOPos = std::fmod(currentLFOPos, 1.0);

    std::vector<PeriodType>::iterator periodIter = periodsVect.begin();
    while (periodIter != periodsVect.end())
    {
        if(currentLFOPos >= std::get<1>(*periodIter).first &&
           currentLFOPos < std::get<1>(*periodIter).second)
        {
            float currentRate = std::get<2>(*periodIter).first +
                                (std::get<2>(*periodIter).second - std::get<2>(*periodIter).first) *
                                (currentLFOPos - std::get<1>(*periodIter).first) /
                                (std::get<1>(*periodIter).second - std::get<1>(*periodIter).first);
            
            //DBG(currentRate);
            //DBG("-----");
            
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
