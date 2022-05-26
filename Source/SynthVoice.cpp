/*
  ==============================================================================

    SynthVoice.cpp
    Created: 1 May 2022 2:04:37pm
    Author:  alway

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SynthVoice.h"

//==============================================================================
SynthVoice::SynthVoice()
{
    firstOscGain.setRampDurationSeconds(rampDuration);
    secondOscGain.setRampDurationSeconds(rampDuration);
    thirdOscGain.setRampDurationSeconds(rampDuration);
}

SynthVoice::~SynthVoice()
{
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    if(firstOscIsTurnedOn){
        firstOsc.prepareToPlay(spec);
        firstOscGain.prepare(spec);
    }
    
    if(secondOscIsTurnedOn){
        secondOsc.prepareToPlay(spec);
        secondOscGain.prepare(spec);
    }
    
    if(thirdOscIsTurnedOn){
        thirdOsc.prepareToPlay(spec);
        thirdOscGain.prepare(spec);
    }
            
    firstAdsr.setSampleRate(sampleRate);
    secondAdsr.setSampleRate(sampleRate);
    thirdAdsr.setSampleRate(sampleRate);
    
    isPrepared = true;
}

void SynthVoice::renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples)
{
    jassert(isPrepared);

    if (!isVoiceActive())
        return;

    firstVoiceBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    secondVoiceBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    thirdVoiceBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    
    firstVoiceBuffer.clear();
    secondVoiceBuffer.clear();
    thirdVoiceBuffer.clear();

    juce::dsp::AudioBlock<float> firstAudioBlock{ firstVoiceBuffer };
    juce::dsp::AudioBlock<float> secondAudioBlock{ secondVoiceBuffer };
    juce::dsp::AudioBlock<float> thirdAudioBlock{ thirdVoiceBuffer };
    
    if(firstOscIsTurnedOn){
        firstOsc.getNextAudioBlock(firstAudioBlock);
        juce::dsp::ProcessContextReplacing<float> firstContext(firstAudioBlock);
        firstOscGain.process(firstContext); // ?????????
        firstAdsr.applyEnvelopeToBuffer(firstVoiceBuffer, 0, firstVoiceBuffer.getNumSamples());
    }
        
    
    if(secondOscIsTurnedOn){
        secondOsc.getNextAudioBlock(secondAudioBlock);
        juce::dsp::ProcessContextReplacing<float> secondContext(secondAudioBlock);
        secondOscGain.process(secondContext);
        secondAdsr.applyEnvelopeToBuffer(secondVoiceBuffer, 0, secondVoiceBuffer.getNumSamples());
    }
        
    
    if(thirdOscIsTurnedOn){
        thirdOsc.getNextAudioBlock(thirdAudioBlock);
        juce::dsp::ProcessContextReplacing<float> thirdContext(thirdAudioBlock);
        thirdOscGain.process(thirdContext);
        thirdAdsr.applyEnvelopeToBuffer(thirdVoiceBuffer, 0, thirdVoiceBuffer.getNumSamples());
    }
    
    //filter.process(synthBuffer);
    //gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom(channel, startSample, firstVoiceBuffer, channel, 0, numSamples);
        outputBuffer.addFrom(channel, startSample, secondVoiceBuffer, channel, 0, numSamples);
        outputBuffer.addFrom(channel, startSample, thirdVoiceBuffer, channel, 0, numSamples);

        if (!firstAdsr.isActive() && !secondAdsr.isActive() && !thirdAdsr.isActive())
            clearCurrentNote();
    }
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    if(firstOscIsTurnedOn){
        firstOsc.setWaveFrequency(midiNoteNumber);
        firstAdsr.noteOn();
    }
    
    if(secondOscIsTurnedOn){
        secondOsc.setWaveFrequency(midiNoteNumber);
        secondAdsr.noteOn();
    }
    
    if(thirdOscIsTurnedOn){
        thirdOsc.setWaveFrequency(midiNoteNumber);
        thirdAdsr.noteOn();
    }

}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    if(firstOscIsTurnedOn)
        firstAdsr.noteOff();
    
    if(secondOscIsTurnedOn)
        secondAdsr.noteOff();
    
    if(thirdOscIsTurnedOn)
        thirdAdsr.noteOff();

    if (!allowTailOff)
        clearCurrentNote();
    
        
}

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue){}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue){}
