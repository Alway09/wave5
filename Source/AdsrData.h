#pragma once
#include<JuceHeader.h>
#include"LFOData.h"

class AdsrData : public LFOData
{
private:
    
    struct  Parameters
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
    };
    
public:
    AdsrData(const juce::String& name);
    ~AdsrData();
    
    bool isActive() const noexcept { return state != State::idle; }
    
    void applyEnvelopeToBuffer(juce::dsp::ProcessContextReplacing<float>& context);
    
private:
    enum class State { idle, attack, decay, sustain, release };
    State state = State::idle;
    Parameters parameters;
    
    juce::dsp::Gain<float> gain;
    
    float resolution = 6.f; // width in seconds
};
