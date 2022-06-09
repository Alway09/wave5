#pragma once

#include <JuceHeader.h>
#include "GlobalUIConstants.h"
#include "PluginProcessor.h"
#include "OscillatorsBlockComponent.h"
#include "LFOBlockComponent.h"
#include "CustomLookAndFeel.h"
#include "ModulationMatrixComponent.h"

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
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Wave5AudioProcessor& audioProcessor;
    //ModifiedAdsrComponent adsrComponent;
    OscillatorsBlockComponent oscBlock;
    LFOBlockComponent lfoBlock;
    
    ModulationMatrixComponent matrixComponent;
    juce::Viewport matrixViewport;
    
    CustomLookAndFeel* lookAndFeel;
    
    int am_of_parameters = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Wave5AudioProcessorEditor)
};
