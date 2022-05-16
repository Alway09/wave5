/*
  ==============================================================================

    SynthVoice.h
    Created: 1 May 2022 2:04:37pm
    Author:  alway

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "OscData.h"
#include "ModifiedAdsrData.h"
#include "LFOData.h"

//==============================================================================
/*
*/
class SynthVoice  : public juce::SynthesiserVoice, public juce::Timer
{
public:
    SynthVoice();
    ~SynthVoice() override;

    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples) override;
    void timerCallback () override { ++workingTime; }

    OscData& getFirstOscillator() { return firstOsc; }
    OscData& getSecondOscillator() { return secondOsc; }
    OscData& getThirdOscillator() { return thirdOsc; }
    
    ModifiedAdsrData& getFirstAdsr() { return firstAdsr; }
    ModifiedAdsrData& getSecondAdsr() { return secondAdsr; }
    ModifiedAdsrData& getThirdAdsr() { return thirdAdsr; }
    
    void setFirstOscState(bool state){ firstOscIsTurnedOn = state; };
    void setSecondOscState(bool state){ secondOscIsTurnedOn = state; };
    void setThirdOscState(bool state){ thirdOscIsTurnedOn = state; };
    
    juce::dsp::Gain<float>& getFirstOscGain(){ return firstOscGain; };
    juce::dsp::Gain<float>& getSecondOscGain(){ return secondOscGain; };
    juce::dsp::Gain<float>& getThirdOscGain(){ return thirdOscGain; };
    
    LFOData& getFirstLFO(){ return firstLFO; };
    uint64_t getWorkingTime() const { return workingTime; }

private:
    juce::AudioBuffer<float> firstVoiceBuffer;
    juce::AudioBuffer<float> secondVoiceBuffer;
    juce::AudioBuffer<float> thirdVoiceBuffer;

    OscData firstOsc;
    OscData secondOsc;
    OscData thirdOsc;
    
    juce::dsp::Gain<float> firstOscGain;
    juce::dsp::Gain<float> secondOscGain;
    juce::dsp::Gain<float> thirdOscGain;
    
    ModifiedAdsrData firstAdsr;
    ModifiedAdsrData secondAdsr;
    ModifiedAdsrData thirdAdsr;
    
    //juce::AudioPlayHead::CurrentPositionInfo hostInfo;
    
    LFOData firstLFO;

    bool isPrepared{ false };
    
    bool firstOscIsTurnedOn{ true };
    bool secondOscIsTurnedOn{ true };
    bool thirdOscIsTurnedOn{ true };
    
    uint64_t workingTime = 0; // in milliseconds
    
    bool firstLFOIsTurnedOn{ true };
};
