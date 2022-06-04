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
    
    void drawComboBox (juce::Graphics& g, int width, int height, bool,
                                       int, int, int, int, juce::ComboBox& box) override;
    
    void drawPopupMenuItem (juce::Graphics& g, const juce::Rectangle<int>& area,
                            const bool isSeparator, const bool isActive,
                            const bool isHighlighted, const bool isTicked,
                            const bool hasSubMenu, const juce::String& text,
                            const juce::String& shortcutKeyText,
                            const juce::Drawable* icon, const juce::Colour* const textColourToUse) override;
    
    void drawPopupMenuBackground (juce::Graphics& g, int width, int height) override;
    
    juce::Label* createSliderTextBox(juce::Slider& ) override;
    juce::Slider::SliderLayout getSliderLayout(juce::Slider& slider) override;
    //juce::Path getTickShape (float height);
};

//const CustomLookAndFeel lookAndFeel;
