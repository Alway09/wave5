/*#include "AdsrData.h"

AdsrData::AdsrData(const juce::String& name) : LFOData(name)
{
    gainRange = juce::NormalisableRange<float>(-96.f, 0.f, 0.1f, 1);
    setEnable(true);
}

AdsrData::~AdsrData(){
    
}

void AdsrData::applyEnvelopeToBuffer(juce::dsp::ProcessContextReplacing<float>& context){
    gain.setGainDecibels(gainRange.convertFrom0to1(calculateEnvelopeValue()));
    
    if(isTimerRunning()){
        DBG("enable");
    }
    
    DBG(calculateEnvelopeValue());
    gain.process(context);
}

void AdsrData::setPeriod(State period, int leftId, int rightId){
    
    if(period == State::attack){
        attackPeriod.first = leftId;
        attackPeriod.second = rightId;
    }else if (period == State::decay){
        decayPeriod.first = leftId;
        decayPeriod.second = rightId;
    }else if(period == State::sustain){
        sustainPeriod.first = leftId;
        sustainPeriod.second = leftId;
    }else if(period == State::release){
        releasePeriod.first = leftId;
        releasePeriod.second = rightId;
    }else{
        jassertfalse;
    }
}*/
