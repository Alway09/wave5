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
#include "LFOPropertiesComponent.h"

//==============================================================================
/*
*/
class LFOBlockComponent  : public juce::TabbedComponent
{
private:
    class PageComponent : public juce::Component
    {
    public:
        PageComponent(juce::AudioProcessorValueTreeState& apvts,
                      LFOData* lfo,
                      const juce::String& rateHZControllerId,
                      const juce::String& rateBpmControllerId,
                      const juce::String& workingModeId,
                      const juce::String& rateModeId);
        ~PageComponent();
        
        void paint (juce::Graphics&) override;
        void resized() override;
        
        //void addVoice(std::atomic<SynthVoice*>* voice, int voiceId){lfo.addVoice(voice, voiceId);}
        
    private:
        LFOComponent lfoComponent;
        LFOPropertiesComponent lfoProperties;
    };
    
public:
    LFOBlockComponent(juce::AudioProcessorValueTreeState& apvts,
                      LFOData* firstLFO,
                      LFOData* secondLFO,
                      LFOData* thirdLFO);
    
    ~LFOBlockComponent() override;
    
    void paint(juce::Graphics& g) override{
        g.setColour(juce::Colours::black);
        g.drawRect(getLocalBounds().removeFromTop(getTabbedButtonBar().getBounds().getHeight()), UI::GLOBAL::strokeLineWigthOutside);
        g.drawRect(getLocalBounds(), UI::GLOBAL::strokeLineWigthOutside);
        
        //g.drawEllipse(7, getHeight() - 100, 100.f, 100.f, 15.f);
    }

    
    void currentTabChanged (int newCurrentTabIndex, const juce::String &newCurrentTabName) override;
    void popupMenuClickOnTab (int tabIndex, const juce::String &tabName) override;
    
    void setCustomLookAndFeel(CustomLookAndFeel* lookAndFeel){
        firstToggle->setLookAndFeel(lookAndFeel);
        secondToggle->setLookAndFeel(lookAndFeel);
        thirdToggle->setLookAndFeel(lookAndFeel);
        
        setLookAndFeel(lookAndFeel);
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
