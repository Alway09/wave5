#pragma once
#include <JuceHeader.h>
#include "GlobalUIConstants.h"

class CustomRotarySlider : public juce::Slider
{
public:
	CustomRotarySlider(const juce::String &sliderName);
	~CustomRotarySlider();
private:
	//juce::Label label;
};
