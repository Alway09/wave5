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
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

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

    if(firstOscIsTurnedOn)
        firstOsc.prepareToPlay(spec);
    
    if(secondOscIsTurnedOn)
        secondOsc.prepareToPlay(spec);
    
    if(thirdOscIsTurnedOn)
        thirdOsc.prepareToPlay(spec);
    
    //filterAdsr.setSampleRate(sampleRate);
    //filter.prepareToPlay(sampleRate, samplesPerBlock, outputChannels);
    firstAdsr.setSampleRate(sampleRate);
    secondAdsr.setSampleRate(sampleRate);
    thirdAdsr.setSampleRate(sampleRate);
    //gain.prepare(spec);

    //gain.setGainLinear(0.3f);

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
    //filterAdsr.applyEnvelopeToBuffer(outputBuffer, 0, numSamples);
    firstVoiceBuffer.clear();
    secondVoiceBuffer.clear();
    thirdVoiceBuffer.clear();

    juce::dsp::AudioBlock<float> firstAudioBlock{ firstVoiceBuffer };
    juce::dsp::AudioBlock<float> secondAudioBlock{ secondVoiceBuffer };
    juce::dsp::AudioBlock<float> thirdAudioBlock{ thirdVoiceBuffer };
    
    if(firstOscIsTurnedOn){
        firstOsc.getNextAudioBlock(firstAudioBlock);
        firstAdsr.applyEnvelopeToBuffer(firstVoiceBuffer, 0, firstVoiceBuffer.getNumSamples());
    }
        
    
    if(secondOscIsTurnedOn){
        secondOsc.getNextAudioBlock(secondAudioBlock);
        secondAdsr.applyEnvelopeToBuffer(secondVoiceBuffer, 0, secondVoiceBuffer.getNumSamples());
    }
        
    
    if(thirdOscIsTurnedOn){
        thirdOsc.getNextAudioBlock(thirdAudioBlock);
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
    firstOsc.setWaveFrequency(midiNoteNumber);
    secondOsc.setWaveFrequency(midiNoteNumber);
    thirdOsc.setWaveFrequency(midiNoteNumber);
    
    firstAdsr.noteOn();
    secondAdsr.noteOn();
    thirdAdsr.noteOn();
    //filterAdsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    firstAdsr.noteOff();
    secondAdsr.noteOff();
    thirdAdsr.noteOff();
    //filterAdsr.noteOff();

    if (!allowTailOff || (!firstAdsr.isActive() && !secondAdsr.isActive() && !thirdAdsr.isActive()))
        clearCurrentNote();
}

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue){}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue){}
