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
    std::srand(std::time(NULL));
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
        // Sine wave
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
            
    case 3:
        // Triangle wave
        initialise([](float x){ return 1.f - std::abs(x / juce::MathConstants<float>::pi) + 2.f; });
        break;
    case 4:
        // White Noise wave
            initialise([](float x){ return -1.f + static_cast<float>(std::rand()) * static_cast<float>(1.f + 1.f) / RAND_MAX; });
        break;

    default:
        jassertfalse;   // You're not supposed to be here!
        break;
    }
}

void OscData::setWaveFrequency(double frequencyInHz)
{
    setFrequency(frequencyInHz);
    //lastMidiNote = midiNoteNumber;
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
    process(juce::dsp::ProcessContextReplacing<float>(block));
}
