#include "AdsrData.h"

AdsrData::AdsrData(const juce::String& name) : LFOData(name)
{
    
}

AdsrData::~AdsrData(){
    
}

void AdsrData::applyEnvelopeToBuffer(juce::dsp::ProcessContextReplacing<float>& context){
    gain.process(context);
}
