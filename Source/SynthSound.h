/*
  ==============================================================================

    SynthSound.h
    Created: 1 May 2022 2:04:15pm
    Author:  alway

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }

};