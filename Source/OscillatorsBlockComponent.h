#pragma once

#include <JuceHeader.h>
#include "ModifiedAdsrComponent.h"
#include "OscPropertiesComponent.h"

//==============================================================================
/*
*/
class OscillatorsBlockComponent : public juce::TabbedComponent
{
private:
    class PageComponent : public juce::Component
    {
    public:
        PageComponent(juce::AudioProcessorValueTreeState& apvts,
                      const juce::StringArray& idList,
                      const juce::String& waveChooseId,
                      const juce::String& gainId,
                      const juce::String& transposeId,
                      const juce::String& panId);
        ~PageComponent();
        
        void paint (juce::Graphics&) override;
        void resized() override;
        
        void setCustomLookAndFeel(CustomLookAndFeel* lookAndFeel);
        
    private:
        //juce::AudioProcessorValueTreeState& apvts;
        
        ModifiedAdsrComponent adsrComponent;
        OscPropertiesComponent oscPropertiesComponent;
        
    };
    
public:
    OscillatorsBlockComponent(juce::AudioProcessorValueTreeState& apvts);
    ~OscillatorsBlockComponent() override;

    void paint (juce::Graphics& g) override{
        g.setColour(juce::Colours::black);
        g.drawRect(getLocalBounds().removeFromTop(getTabbedButtonBar().getBounds().getHeight()), UI::GLOBAL::strokeLineWigthOutside);
        g.drawRect(getLocalBounds(), UI::GLOBAL::strokeLineWigthOutside);
    }
    //void resized() override;
    
    void currentTabChanged (int newCurrentTabIndex, const juce::String &newCurrentTabName) override;
    void popupMenuClickOnTab (int tabIndex, const juce::String &tabName) override;
    
    void setCustomLookAndFeel(CustomLookAndFeel* lookAndFeel);

private:
    
    PageComponent firstPageComponent;
    PageComponent secondPageComponent;
    PageComponent thirdPageComponent;
    
    juce::ToggleButton* firstToggle;
    juce::ToggleButton* secondToggle;
    juce::ToggleButton* thirdToggle;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> firstOscStateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> secondOscStateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> thirdOscStateAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorsBlockComponent)
};
