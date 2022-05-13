#pragma once

#include <JuceHeader.h>
#include "ModifiedAdsrComponent.h"

//==============================================================================
/*
*/
class OscillatorsBlockComponent : public juce::TabbedComponent
{
private:
    class PageComponent : public juce::Component
    {
    public:
        PageComponent(juce::AudioProcessorValueTreeState& apvts, const juce::StringArray& idList);
        ~PageComponent();
        
        void paint (juce::Graphics&) override;
        void resized() override;
        
    private:
        //juce::AudioProcessorValueTreeState& apvts;
        
        ModifiedAdsrComponent adsrComponent;
        
    };
    
public:
    OscillatorsBlockComponent(juce::AudioProcessorValueTreeState& apvts);
    ~OscillatorsBlockComponent() override;

    //void paint (juce::Graphics&) override;
    //void resized() override;
    
    void currentTabChanged (int newCurrentTabIndex, const juce::String &newCurrentTabName) override;
    void popupMenuClickOnTab (int tabIndex, const juce::String &tabName) override;

private:
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> firstOscStateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> secondOscStateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> thirdOscStateAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorsBlockComponent)
};
