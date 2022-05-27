/*
  ==============================================================================

    LFOComponent.h
    Created: 17 May 2022 12:28:13pm
    Author:  Валентин Чекулаев

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "EnvelopeVisualComponent.h"
#include "SynthVoice.h"

//==============================================================================
/*
*/
class LFOComponent  : public juce::Component, juce::Timer
{
public:
    LFOComponent(LFOData* lfo);
    ~LFOComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void timerCallback() override;
    
    //void addVoice(std::atomic<SynthVoice*>* voice, int voiceId){ voices[voiceId] = voice; }
    
    void setSizes();

private:
    
    LFOData* relativeLFO;
    
    EnvelopeVisualComponent envelope;
    //int lfoNumber = 0;
    
    juce::Rectangle<int> envBounds;
    
    //std::map<int, std::atomic<SynthVoice*>* > voices;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOComponent)
};
