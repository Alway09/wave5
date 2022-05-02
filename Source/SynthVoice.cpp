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

    osc.prepareToPlay(spec);
    //filterAdsr.setSampleRate(sampleRate);
    //filter.prepareToPlay(sampleRate, samplesPerBlock, outputChannels);
    adsr.setSampleRate(sampleRate);
    //gain.prepare(spec);

    //gain.setGainLinear(0.3f);

    isPrepared = true;
}

void SynthVoice::renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples)
{
    jassert(isPrepared);

    if (!isVoiceActive())
        return;

    voiceBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    //filterAdsr.applyEnvelopeToBuffer(outputBuffer, 0, numSamples);
    voiceBuffer.clear();

    juce::dsp::AudioBlock<float> audioBlock{ voiceBuffer };
    osc.getNextAudioBlock(audioBlock);
    adsr.applyEnvelopeToBuffer(voiceBuffer, 0, voiceBuffer.getNumSamples());
    //filter.process(synthBuffer);
    //gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom(channel, startSample, voiceBuffer, channel, 0, numSamples);

        if (!adsr.isActive())
            clearCurrentNote();
    }
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    osc.setWaveFrequency(midiNoteNumber);
    adsr.noteOn();
    //filterAdsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    adsr.noteOff();
    //filterAdsr.noteOff();

    if (!allowTailOff || !adsr.isActive())
        clearCurrentNote();
}

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue){}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue){}