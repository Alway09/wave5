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

//==============================================================================
/*
*/
class SynthVoice  : public juce::SynthesiserVoice
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

    OscData& getFirstOscillator() { return firstOsc; }
    OscData& getSecondOscillator() { return secondOsc; }
    OscData& getThirdOscillator() { return thirdOsc; }
    
    ModifiedAdsrData& getFirstAdsr() { return firstAdsr; }
    ModifiedAdsrData& getSecondAdsr() { return firstAdsr; }
    ModifiedAdsrData& getThirdAdsr() { return firstAdsr; }

private:
    juce::AudioBuffer<float> firstVoiceBuffer;
    juce::AudioBuffer<float> secondVoiceBuffer;
    juce::AudioBuffer<float> thirdVoiceBuffer;

    OscData firstOsc;
    OscData secondOsc;
    OscData thirdOsc;
    
    ModifiedAdsrData firstAdsr;
    ModifiedAdsrData secondAdsr;
    ModifiedAdsrData thirdAdsr;

    bool isPrepared{ false };
};
