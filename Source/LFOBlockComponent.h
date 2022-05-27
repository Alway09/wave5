/*
  ==============================================================================

    LFOBlockComponent.h
    Created: 17 May 2022 12:38:41pm
    Author:  Валентин Чекулаев

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LFOComponent.h"
#include "GlobalUIConstants.h"
#include "SynthVoice.h"
#include "StringConstants.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/*
*/
class LFOBlockComponent  : public juce::TabbedComponent
{
private:
    class PageComponent : public juce::Component
    {
    public:
        PageComponent(LFOData* lfo);
        ~PageComponent();
        
        void paint (juce::Graphics&) override;
        void resized() override;
        
        //void addVoice(std::atomic<SynthVoice*>* voice, int voiceId){lfo.addVoice(voice, voiceId);}
        
    private:
        LFOComponent lfoComponent;
    };
    
public:
    LFOBlockComponent(juce::AudioProcessorValueTreeState& apvts,
                      LFOData* firstLFO,
                      LFOData* secondLFO,
                      LFOData* thirdLFO);
    
    ~LFOBlockComponent() override;
    
    void currentTabChanged (int newCurrentTabIndex, const juce::String &newCurrentTabName) override;
    void popupMenuClickOnTab (int tabIndex, const juce::String &tabName) override;
    
    //void setFirstLFOVoices(std::atomic<SynthVoice*>& voice);
    /*void addVoice(std::atomic<SynthVoice*>* voice, int voiceId){
        pageOne.addVoice(voice, voiceId);
        pageTwo.addVoice(voice, voiceId);
        pageThree.addVoice(voice, voiceId);
    }*/
    
    void setCustomLookAndFeel(CustomLookAndFeel* lookAndFeel){
        firstToggle->setLookAndFeel(lookAndFeel);
        secondToggle->setLookAndFeel(lookAndFeel);
        thirdToggle->setLookAndFeel(lookAndFeel);
    }

private:
    PageComponent pageOne;
    PageComponent pageTwo;
    PageComponent pageThree;
    
    juce::ToggleButton* firstToggle;
    juce::ToggleButton* secondToggle;
    juce::ToggleButton* thirdToggle;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> firstLFOStateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> secondLFOStateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> thirdLFOStateAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOBlockComponent)
};
