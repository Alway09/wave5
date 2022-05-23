#pragma once
#include <JuceHeader.h>
#include "GlobalUIConstants.h"


class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();
    ~CustomLookAndFeel();
    
    void drawRotarySlider (juce::Graphics&,
                           int x,
                           int y,
                           int width,
                           int height,
                           float sliderPosProportional,
                           float rotaryStartAngle,
                           float rotaryEndAngle,
                           juce::Slider&
                           ) override;
    
    void drawToggleButton(juce::Graphics&,
                          juce::ToggleButton&,
                          bool shouldDrawButtonAsHighlighted,
                          bool shouldDrawButtonAsDown
                          ) override;
    
    juce::Label* createSliderTextBox(juce::Slider& ) override;
    juce::Slider::SliderLayout getSliderLayout(juce::Slider& slider) override;
};

//const CustomLookAndFeel lookAndFeel;
