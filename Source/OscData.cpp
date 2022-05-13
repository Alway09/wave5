/*
  ==============================================================================

    OscData.cpp
    Created: 1 May 2022 2:27:25pm
    Author:  alway

  ==============================================================================
*/

#include "OscData.h"

OscData::OscData() 
    : juce::dsp::Oscillator<float>()
{

}

OscData::~OscData() {};

void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec)
{
    prepare(spec);
    //osc.prepare(spec);
}

void OscData::setWaveType(const int choice)
{
    switch (choice)
    {
    case 0:
        // Sine
        initialise([](float x) { return std::sin(x); });
        break;

    case 1:
        // Square wave
        initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
        break;

    case 2:
        // Saw wave
        initialise([](float x) { return x / juce::MathConstants<float>::pi; });
        break;

    default:
        jassertfalse;   // You're not supposed to be here!
        break;
    }
}

void OscData::setWaveFrequency(const int midiNoteNumber)
{
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    lastMidiNote = midiNoteNumber;
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
    process(juce::dsp::ProcessContextReplacing<float>(block));
}
