#include "CustomRotarySlider.h"

//CustomLookAndFeel lookAndFeel;

CustomRotarySlider::CustomRotarySlider(const juce::String &sliderName) : juce::Slider(sliderName)
{
    //setLookAndFeel(&lookAndFeel);
    
    //nameLabel.setText(sliderName, juce::NotificationType::dontSendNotification);
    //nameLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
    //nameLabel.setJustificationType(juce::Justification::centred);
    
	setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
	setTextBoxStyle(juce::Slider::TextBoxBelow, true, UI::GLOBAL::sliderTextBoxWidth, UI::GLOBAL::sliderTextBoxHeight);
    
    //addAndMakeVisible(nameLabel);
}

CustomRotarySlider::~CustomRotarySlider() {};

/*void CustomRotarySlider::resized(){
    this->juce::Slider::resized();
    nameLabel.setBounds(0, getHeight(), getWidth(), getHeight() * 0.3f);
    //nameLabel.setBounds(0, 0, getWidth(), getHeight() * 0.3f);
}*/

/*juce::Rectangle<int> CustomRotarySlider::getLocalBounds() const{
    return this->juce::Component::getLocalBounds().withHeight(100);
}*/
