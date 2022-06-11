

#include <JuceHeader.h>
#include "EqualizerComponent.h"
#include "StringConstants.h"

//==============================================================================
EqualizerComponent::EqualizerComponent(juce::AudioProcessorValueTreeState& apvts) :
lowCutFreqSlider("Freq"),
highCutFreqSlider("Freq"),
peakFreqSlider("Freq"),
peakQSlider("Q"),
peakGainSlider("Gain")
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    lowCutLabel.setText("LowCut", juce::NotificationType::dontSendNotification);
    //lowCutLabel.setJustificationType(juce::Justification::centred);
    lowCutLabel.setColour(juce::Label::textColourId, juce::Colours::black);

    lowCutFreqLabel.setText("Freq", juce::NotificationType::dontSendNotification);
    lowCutFreqLabel.setJustificationType(juce::Justification::centred);
    lowCutFreqLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    
    lowCutSlopeLable.setText("Slope", juce::NotificationType::dontSendNotification);
    lowCutSlopeLable.setJustificationType(juce::Justification::centred);
    lowCutSlopeLable.setColour(juce::Label::textColourId, juce::Colours::black);
    
    peakLabel.setText("Peak", juce::NotificationType::dontSendNotification);
    //peakLabel.setJustificationType(juce::Justification::centred);
    peakLabel.setColour(juce::Label::textColourId, juce::Colours::black);

    peakFreqLabel.setText("Freq", juce::NotificationType::dontSendNotification);
    peakFreqLabel.setJustificationType(juce::Justification::centred);
    peakFreqLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    
    peakQLabel.setText("Q", juce::NotificationType::dontSendNotification);
    peakQLabel.setJustificationType(juce::Justification::centred);
    peakQLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    
    peakGainLabel.setText("Gain", juce::NotificationType::dontSendNotification);
    peakGainLabel.setJustificationType(juce::Justification::centred);
    peakGainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    
    highCutLabel.setText("HighCut", juce::NotificationType::dontSendNotification);
    //highCutLabel.setJustificationType(juce::Justification::centred);
    highCutLabel.setColour(juce::Label::textColourId, juce::Colours::black);

    highCutFreqLabel.setText("Freq", juce::NotificationType::dontSendNotification);
    highCutFreqLabel.setJustificationType(juce::Justification::centred);
    highCutFreqLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    
    highCutSlopeLable.setText("Slope", juce::NotificationType::dontSendNotification);
    highCutSlopeLable.setJustificationType(juce::Justification::centred);
    highCutSlopeLable.setColour(juce::Label::textColourId, juce::Colours::black);
    
    lowCutFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts,
                                                                                                  STR_CONST::EQ::lowCutFreq,
                                                                                                  lowCutFreqSlider);
    
    peakFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts,
                                                                                                STR_CONST::EQ::peakFreq,
                                                                                                peakFreqSlider);
    
    peakGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts,
                                                                                                STR_CONST::EQ::peakGain,
                                                                                                peakGainSlider);
    
    highCutFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts,
                                                                                                   STR_CONST::EQ::highCutFreq,
                                                                                                   highCutFreqSlider);
    
    peakQAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts,
                                                                                             STR_CONST::EQ::peakQuality,
                                                                                             peakQSlider);
    
    lowCutBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            apvts,
            STR_CONST::EQ::lowCutBypassed,
            lowCutBypass);
    
    peakBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            apvts,
            STR_CONST::EQ::peakBypassed,
            peakBypass);
    
    highCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            apvts,
            STR_CONST::EQ::highCutBypassed,
            highCutBypass);
    
    juce::StringArray strArr;
    for (int i = 0; i < 4; ++i) {
        juce::String str;
        str << (12 + i * 12);
        str << " db/Oct";
        strArr.add(str);
    }
    
    lowCutSlopeChooser.addItemList(strArr, 1);
    highCutSlopeChooser.addItemList(strArr, 1);
    
    lowCutSlopeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
                                                                apvts,
                                                                STR_CONST::EQ::lowCutSlope,
                                                                lowCutSlopeChooser);
    
    highCutSlopeAttahment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
                                                                apvts,
                                                                STR_CONST::EQ::highCutSlope,
                                                                highCutSlopeChooser);
    
    lowCutSlopeChooser.setColour(juce::ComboBox::textColourId, juce::Colours::black);
    highCutSlopeChooser.setColour(juce::ComboBox::textColourId, juce::Colours::black);
    
    addAndMakeVisible(lowCutLabel);
    addAndMakeVisible(lowCutBypass);
    addAndMakeVisible(lowCutFreqSlider);
    addAndMakeVisible(lowCutFreqLabel);
    addAndMakeVisible(lowCutSlopeChooser);
    addAndMakeVisible(lowCutSlopeLable);
    
    addAndMakeVisible(peakLabel);
    addAndMakeVisible(peakBypass);
    addAndMakeVisible(peakFreqSlider);
    addAndMakeVisible(peakFreqLabel);
    addAndMakeVisible(peakQSlider);
    addAndMakeVisible(peakQLabel);
    addAndMakeVisible(peakGainSlider);
    addAndMakeVisible(peakGainLabel);
    
    addAndMakeVisible(highCutLabel);
    addAndMakeVisible(highCutBypass);
    addAndMakeVisible(highCutFreqSlider);
    addAndMakeVisible(highCutFreqLabel);
    addAndMakeVisible(highCutSlopeChooser);
    addAndMakeVisible(highCutSlopeLable);
}

EqualizerComponent::~EqualizerComponent()
{
}

void EqualizerComponent::paint (juce::Graphics& g)
{
    g.fillAll(UI::GLOBAL::backColour);
    g.drawRect(0, 0, getWidth(), getHeight(), UI::GLOBAL::strokeLineWigthInside);
    
    auto rectCopy = slidersAreaRect;
    
    g.drawRect(rectCopy, UI::GLOBAL::strokeLineWigthInside);
    
    rectCopy.setY(rectCopy.getY() + rectCopy.getHeight() + 2 * UI::GLOBAL::paddingFromStoke);
    g.drawRect(rectCopy, UI::GLOBAL::strokeLineWigthInside);
    
    rectCopy.setY(rectCopy.getY() + rectCopy.getHeight() + 2 * UI::GLOBAL::paddingFromStoke);
    g.drawRect(rectCopy, UI::GLOBAL::strokeLineWigthInside);
}

void EqualizerComponent::setSizes(){
    
    int width = getWidth() - 2 * UI::GLOBAL::paddingFromStoke;
    int height = getHeight() / 3 - 2 * UI::GLOBAL::paddingFromStoke;
    int x = UI::GLOBAL::paddingFromStoke;
    int y = UI::GLOBAL::paddingFromStoke;
    
    slidersAreaRect = juce::Rectangle<int>(x, y, width, height);
    
    width = UI::GLOBAL::sliderComponentWidth;
    height = UI::GLOBAL::sliderComponentHeight + 8;
    
    sliderRect = juce::Rectangle<int>(0, 0, width, height);
    
    width = UI::OSC_BLOCK::toggleWidth;
    height = UI::OSC_BLOCK::toggleHeight;
    
    toggleRect = juce::Rectangle<int>(0, 0, width, width);
}

void EqualizerComponent::resized()
{
    setSizes();
    const int padding = 15;
    const int sliderReduce = 8;
    const int sliderYPadding = 6;
    const int mysteryPadding = 90;
    //=============================================
    auto localSlidersArea = slidersAreaRect;
    
    auto localToggleRect = toggleRect;
    localToggleRect.setX(localSlidersArea.getX() + UI::GLOBAL::paddingFromStoke);
    //localToggleRect.setX(0);
    localToggleRect.setY((localSlidersArea.getY() + localSlidersArea.getHeight()) / 2 - localToggleRect.getHeight() / 2);
    lowCutBypass.setBounds(localToggleRect);
    
    juce::Rectangle<int> labelRect(localToggleRect.getX() + localToggleRect.getWidth() + UI::GLOBAL::paddingComponentsInside,
                                   (localSlidersArea.getY() + localSlidersArea.getHeight()) / 2 - 30 / 2,
                                   60, 30);
    
    lowCutLabel.setBounds(labelRect);
    
    auto localSliderBounds = sliderRect;
    localSliderBounds.setX(labelRect.getX() + labelRect.getWidth() + padding);
    localSliderBounds.setY((localSlidersArea.getY() + localSlidersArea.getHeight()) / 2 - localSliderBounds.getHeight() / 2 + sliderYPadding);
    auto r = localSliderBounds;
    lowCutFreqSlider.setBounds(r.removeFromTop(UI::GLOBAL::sliderComponentHeight - sliderReduce));
    lowCutFreqLabel.setBounds(r);
    
    localSliderBounds.setX(localSliderBounds.getX() + localSliderBounds.getWidth() + padding);
    r = localSliderBounds;
    r.setWidth(r.getWidth() + 50);
    lowCutSlopeChooser.setBounds(r.removeFromTop(UI::GLOBAL::sliderComponentHeight - sliderReduce).reduced(0, 10));
    lowCutSlopeLable.setBounds(r);
    
    //==============================
    
    localSlidersArea.setY(mysteryPadding + localSlidersArea.getY() + localSlidersArea.getHeight() + 2 * UI::GLOBAL::paddingFromStoke);
    
    localToggleRect = toggleRect;
    localToggleRect.setX(localSlidersArea.getX() + UI::GLOBAL::paddingFromStoke);
    //localToggleRect.setX(0);
    localToggleRect.setY((localSlidersArea.getY() + localSlidersArea.getHeight()) / 2 - localToggleRect.getHeight() / 2);
    peakBypass.setBounds(localToggleRect);
    
    labelRect.setX(localToggleRect.getX() + localToggleRect.getWidth() + UI::GLOBAL::paddingComponentsInside);
    labelRect.setY((localSlidersArea.getY() + localSlidersArea.getHeight()) / 2 - 30 / 2);
    
    peakLabel.setBounds(labelRect);
    
    localSliderBounds = sliderRect;
    localSliderBounds.setX(labelRect.getX() + labelRect.getWidth() + padding);
    localSliderBounds.setY((localSlidersArea.getY() + localSlidersArea.getHeight()) / 2 - localSliderBounds.getHeight() / 2 + sliderYPadding);
    r = localSliderBounds;
    peakFreqSlider.setBounds(r.removeFromTop(UI::GLOBAL::sliderComponentHeight - sliderReduce));
    peakFreqLabel.setBounds(r);
    
    localSliderBounds.setX(localSliderBounds.getX() + localSliderBounds.getWidth() + padding);
    r = localSliderBounds;
    peakQSlider.setBounds(r.removeFromTop(UI::GLOBAL::sliderComponentHeight - sliderReduce));
    peakQLabel.setBounds(r);
    
    localSliderBounds.setX(localSliderBounds.getX() + localSliderBounds.getWidth() + padding);
    r = localSliderBounds;
    peakGainSlider.setBounds(r.removeFromTop(UI::GLOBAL::sliderComponentHeight - sliderReduce));
    peakGainLabel.setBounds(r);
    
    //==============================
    localSlidersArea.setY(mysteryPadding + localSlidersArea.getY() + localSlidersArea.getHeight() + 2 * UI::GLOBAL::paddingFromStoke);
    
    localToggleRect = toggleRect;
    localToggleRect.setX(localSlidersArea.getX() + UI::GLOBAL::paddingFromStoke);
    //localToggleRect.setX(0);
    localToggleRect.setY((localSlidersArea.getY() + localSlidersArea.getHeight()) / 2 - localToggleRect.getHeight() / 2);
    highCutBypass.setBounds(localToggleRect);
    
    labelRect.setX(localToggleRect.getX() + localToggleRect.getWidth() + UI::GLOBAL::paddingComponentsInside);
    labelRect.setY((localSlidersArea.getY() + localSlidersArea.getHeight()) / 2 - 30 / 2);
    
    highCutLabel.setBounds(labelRect);
    
    localSliderBounds = sliderRect;
    localSliderBounds.setX(labelRect.getX() + labelRect.getWidth() + padding);
    localSliderBounds.setY((localSlidersArea.getY() + localSlidersArea.getHeight()) / 2 - localSliderBounds.getHeight() / 2 + sliderYPadding);
    r = localSliderBounds;
    highCutFreqSlider.setBounds(r.removeFromTop(UI::GLOBAL::sliderComponentHeight - sliderReduce));
    highCutFreqLabel.setBounds(r);
    
    localSliderBounds.setX(localSliderBounds.getX() + localSliderBounds.getWidth() + padding);
    r = localSliderBounds;
    r.setWidth(r.getWidth() + 50);
    highCutSlopeChooser.setBounds(r.removeFromTop(UI::GLOBAL::sliderComponentHeight - sliderReduce).reduced(0, 10));
    highCutSlopeLable.setBounds(r);
}
