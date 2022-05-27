/*
  ==============================================================================

    LFOData.h
    Created: 16 May 2022 12:34:49pm
    Author:  Валентин Чекулаев

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//first - left and right dot ids, second - start and end of period, third - range of period
using PeriodType = std::tuple<std::pair<int, int>, std::pair<float, float>, std::pair<float, float>>;
// left and right dot ids and index in vector
//using IDElem = std::pair<std::pair<int, int>, int>;

class LFOData : public juce::Timer
{
public:
    LFOData(const juce::String& name);
    ~LFOData();
    
    void timerCallback() override;
    
    /*void setSampleRate(float newSampleRate){
        jassert(newSampleRate > 0.f);
        sampleRate = newSampleRate;
    }*/
    
    const juce::String& getName() const {return lfoName; }
    
    //float getEnvelopeValue(juce::AudioPlayHead* playHead);
    
    void addPeriod(int leftID, int rightID, float xStart, float xEnd, float yStart, float yEnd);
    void deletePeriod(int leftID, int rightID);
    void updatePeriod(int leftID, int rightID, float xStart, float xEnd, float yStart, float yEnd);
    
    float calculateEnvelopeValue();
    float getEnvelopeValue() const { return envelopeVal; }
    
    juce::AudioPlayHead::CurrentPositionInfo& getPlayHeadInfo() { return info; }
    
    void setEnable(bool enable){ LFOisEnable = enable; }
    void turnInState(bool state);
    bool isEnable() const { return LFOisEnable; }
    
    void begin();
    void end();
    
private:
    //bool isRunning = false;
    
    juce::String lfoName;
    //double sampleRate = 44100.0;
    
    enum class RateMode{HZ, BPM};
    RateMode currentRateMode = RateMode::BPM;
    
    enum class TriggerMode{OFF, Trig, Env};
    TriggerMode currentTriggerMode = TriggerMode::Trig;
    
    bool LFOisEnable { true };
    
    juce::AudioPlayHead::CurrentPositionInfo info;

    std::vector<PeriodType> periodsVect;
    unsigned int workingTime{ 0 };
    bool isRunning{ false };
    
    float envelopeVal = 0;
    //bool isEnvelope{ true };
    
    //bool isHZRate{ false };
    //bool isBPMRate{ true };
};
