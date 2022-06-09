#include "LFOData.h"

LFOData::LFOData(const juce::String& name) : lfoName(name){

}

LFOData::~LFOData(){
    
}

void LFOData::addPeriod(int leftID, int rightID, float xStart, float xEnd, float yStart, float yEnd){
    
    //DBG(xStart);
    //DBG(xEnd);
    
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

float LFOData::calculateEnvelopeValue(int bufferSize, double sampleRate){
    //float envelopeVal = 0;
        
    if(LFOisEnable){
        
        if(currentBuffer == 0){
            envelopeVal = 0;
            //DBG("----");
            return 0;
        }
        
        float currentLFOPos = 0;
        
        //DBG(getName());
        //DBG((int)currentBuffer);
        //DBG(info.bpm / 60);
        //DBG(bufferSize);
        //DBG(info.timeInSamples);
        
        if(currentRateMode == RateMode::BPM){
            //currentLFOPos = info.bpm / 60.f * (workingTime / 1000.f)/* * 1.07f*/;
            currentLFOPos = info.bpm / 60.0 * (double)currentBuffer * (double)bufferSize / sampleRate;
        }else if(currentRateMode == RateMode::HZ){
            //currentLFOPos = workingTime / 100.f;
        }else{
            jassertfalse;
        }
        
        currentLFOPos /= 1.f; // rate
        if(lfoName == "LFO 1")
            DBG(currentLFOPos);
        
        //DBG(currentLFOPos);
        
        if(currentTriggerMode == TriggerMode::Trig){
            currentLFOPos = std::fmod(currentLFOPos, 1.0);
        }else if(currentTriggerMode == TriggerMode::Env){
            currentLFOPos = std::fmin(currentLFOPos, 1.0);
        }else if (currentTriggerMode == TriggerMode::OFF){
            
        }else{
            jassertfalse;
        }
        
        //DBG(currentLFOPos);
        
        //currentLFOPos = !isEnvelope ? std::fmod(currentLFOPos, 1.0) : std::fmin(currentLFOPos, 1.0);

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
    }
    
    //DBG(envelopeVal);
    return envelopeVal;
    //return 0;
}

void LFOData::timerCallback(){
    //++workingTime;
}

void LFOData::begin(){
    if(LFOisEnable && !isRunning){
        //startTimer(1);
        isRunning = true;
    }
    if(isRunning)
        ++currentBuffer;
}

void LFOData::end(){
    //stopTimer();
    currentBuffer = 0;
    isRunning = false;
}

void LFOData::turnInState(bool state){
    if(state){
        begin();
    }else{
        end();
    }
}
