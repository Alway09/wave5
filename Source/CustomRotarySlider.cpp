#include "CustomRotarySlider.h"

CustomRotarySlider::CustomRotarySlider() : juce::Slider()
{
	setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
	setTextBoxStyle(juce::Slider::TextBoxBelow, true, UI::GLOBAL::sliderTextBoxWidth, UI::GLOBAL::sliderTextBoxHeight);
}

CustomRotarySlider::~CustomRotarySlider() {};