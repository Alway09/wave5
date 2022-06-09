#pragma once

#include <JuceHeader.h>
#include "GlobalUIConstants.h"
#include "ModulationMatrixData.h"

using HeadType = std::vector<juce::Label*>;
using StrType = std::map<juce::String, std::vector<juce::Slider*>>;
using APVTS = juce::AudioProcessorValueTreeState;
//==============================================================================
/*
*/
class ModulationMatrixComponent  : public juce::Component, public juce::Slider::Listener
{
public:
    ModulationMatrixComponent(juce::StringArray& paramIDs, std::atomic<ModulationMatrixData*>& mData);
    ~ModulationMatrixComponent() override;
    
    void sliderValueChanged (juce::Slider *slider) override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void setCustomLookAndFeel(CustomLookAndFeel* lookAndFeel);
    
    //int getMatrixWidth() const { return matrixWidth; }
    //int getMatrixHeight() const { return std::max(matrixHeight, UI::MODULATION_MATRIX::height); }
    
    //juce::Rectangle<int> getMatrixLocalBounds();
    
    //juce::Viewport& getViewport() { return viewport; }

private:
    
    void chooserHandler();
    
    //juce::Viewport viewport;
    
    //int matrixWidth = 10;
    //int matrixHeight = 10;
    
    HeadType header;
    juce::StringArray coloumnNames = {"", "LFO 1", "LFO 2", "LFO 3"};
    StrType strings;
    std::map<juce::String, juce::Label*> strLabels;
    
    juce::StringArray parametersIDs;
    
    std::atomic<ModulationMatrixData*>& matrixData;
    
    juce::ComboBox paramChooser;
    
    CustomLookAndFeel* customLookAndFeel;
    
    const int am_of_coloumns = 4;
    int labelWidth;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModulationMatrixComponent)
};
