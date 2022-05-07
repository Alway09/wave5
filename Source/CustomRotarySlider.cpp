#include "CustomRotarySlider.h"

CustomRotarySlider::CustomRotarySlider(const juce::String &sliderName) : juce::Slider(sliderName)
{
	setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
	setTextBoxStyle(juce::Slider::TextBoxBelow, true, UI::GLOBAL::sliderTextBoxWidth, UI::GLOBAL::sliderTextBoxHeight);
}

CustomRotarySlider::~CustomRotarySlider() {};
