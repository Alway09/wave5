/*
  ==============================================================================

    OscData.h
    Created: 1 May 2022 2:27:25pm
    Author:  alway

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class OscData : public juce::dsp::Oscillator<float>
{
public:
    OscData();
    ~OscData();

    void prepareToPlay(juce::dsp::ProcessSpec& spec);
    void setWaveType(const int choice);
    void setWaveFrequency(const int midiNoteNumber);
    void getNextAudioBlock(juce::dsp::AudioBlock<float>& block);
    
    int getLastNote() const { return lastMidiNote; }

private:
    //juce::dsp::Oscillator<float> osc{ [](float x) { return std::sin(x); } };
    int lastMidiNote{ 0 };
};
