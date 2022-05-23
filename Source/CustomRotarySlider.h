#pragma once
#include <JuceHeader.h>
#include "CustomLookAndFeel.h"
#include "GlobalUIConstants.h"

class CustomRotarySlider : public juce::Slider
{
public:
	CustomRotarySlider(const juce::String &sliderName);
	~CustomRotarySlider();
    
    //void resized() override;
    juce::Rectangle<int> getLocalBounds() const;
    
private:
	//juce::Label nameLabel;
};
