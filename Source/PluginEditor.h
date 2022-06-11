#pragma once

#include <JuceHeader.h>
#include "GlobalUIConstants.h"
#include "PluginProcessor.h"
#include "OscillatorsBlockComponent.h"
#include "LFOBlockComponent.h"
#include "CustomLookAndFeel.h"
#include "ModulationMatrixComponent.h"
#include "EqualizerComponent.h"

//==============================================================================
/**
*/
class Wave5AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Wave5AudioProcessorEditor (Wave5AudioProcessor&);
    ~Wave5AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Wave5AudioProcessor& audioProcessor;
    OscillatorsBlockComponent oscBlock;
    LFOBlockComponent lfoBlock;
    EqualizerComponent equalizer;
    ModulationMatrixComponent matrixComponent;
    juce::Viewport matrixViewport;
    CustomLookAndFeel* lookAndFeel;
    
    juce::Rectangle<int> globalPropBounds;
    CustomRotarySlider globalVolumeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> globalVolumeAttachment;
    juce::Label globalVolumeLabel;
    juce::Slider glideSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> glideAttachment;
    juce::Label glideLabel;
    
    int am_of_parameters = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Wave5AudioProcessorEditor)
};
