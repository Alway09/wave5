#pragma once
#include<JuceHeader.h>
#include"LFOData.h"

class AdsrData : public LFOData
{
private:
    
    /*struct  Parameters
    {
        Parameters() = default;

        Parameters(float attackTimeSeconds,
            float decayTimeSeconds,
            float sustainLevel,
            float releaseTimeSeconds)
            : attackTime(attackTimeSeconds),
            decayTime(decayTimeSeconds),
            sustainLevel(sustainLevel),
            releaseTime(releaseTimeSeconds)
        {
        }

        float attackTime = 1.0f, decayTime = 0.1f, sustainLevel = 0.5f, releaseTime = 0.1f;
    };*/
    
public:
    AdsrData(const juce::String& name);
    ~AdsrData();
    
    enum class State { idle, attack, decay, sustain, release };
    
    void setSampleRate(double newSampleRate){
        sampleRate = newSampleRate;
    }
    
    void noteOn(){ begin(); }
    void noteOff(){ end(); }
    
    //bool isActive() const noexcept { return state != State::idle; }
    
    void applyEnvelopeToBuffer(juce::dsp::ProcessContextReplacing<float>& context);
    
    //void setPeriod(State period, int leftId, int rightId);
    
private:
    
    State state = State::idle;
    //Parameters parameters;
    
    float attack = 0.f;
    
    //std::pair<int, int> attackPeriod;
    //std::pair<int, int> decayPeriod;
    //std::pair<int, int> sustainPeriod;
    //std::pair<int, int> releasePeriod;
    
    juce::dsp::Gain<float> gain;
    juce::NormalisableRange<float> gainRange;
    double sampleRate = 44100.0;
    
    //float resolution = 6.f; // width in seconds
};
