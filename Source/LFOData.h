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
    
    //float getEnvelopeValue(juce::AudioPlayHead* playHead);
    
    void deletePeriod(int leftID, int rightID);
    
    void updatePeriod(int leftID, int rightID, float xStart, float xEnd, float yStart, float yEnd);
    
    float getEnvelopeValue(uint64_t voiceWorkingTime);
    //void updateHostInfo(juce::AudioPlayHead::CurrentPositionInfo& newInfo){ info = newInfo; };
    juce::AudioPlayHead::CurrentPositionInfo& getBPMInfo() {return info;}
    
    void addPeriod(int leftID, int rightID, float xStart, float xEnd, float yStart, float yEnd);
    
    void begin();
    void end();
    
private:
    bool isRunning = false;
    double sampleRate = 44100.0;
    
    juce::AudioPlayHead::CurrentPositionInfo info;

    //std::vector<std::vector<std::pair<float, float>>[3]> periodsVect;
    std::vector<PeriodType> periodsVect;
    //std::vector<IDElem> periodsIDs; // crashes because unactual indexes
};
