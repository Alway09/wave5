/*
  ==============================================================================

    LFOData.h
    Created: 16 May 2022 12:34:49pm
    Author:  Валентин Чекулаев

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

// second - start and end of period, third - range of period
using PeriodType = std::pair<std::pair<float, float>, std::pair<float, float>>;

class LFOData
{
public:
    LFOData();
    ~LFOData();
    
    bool isActive() const { return isRunning; };
    void setSampleRate(float newSampleRate){
        jassert(newSampleRate > 0.f);
        sampleRate = newSampleRate;
    }
    
    float getEnvelopeValue(juce::AudioPlayHead* playHead);
    
    float getEnvelopeValue(uint64_t voiceWorkingTime);
    void updateHostInfo(juce::AudioPlayHead::CurrentPositionInfo& newInfo){ info = newInfo; };
    
    void begin();
    void end();
    
private:
    bool isRunning = false;
    double sampleRate = 44100.0;
    
    juce::AudioPlayHead::CurrentPositionInfo info;

    std::vector<PeriodType> periodsVect;
};
