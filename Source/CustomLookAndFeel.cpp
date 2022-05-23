#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel() : juce::LookAndFeel_V4()
{
    
}

CustomLookAndFeel::~CustomLookAndFeel()
{
    
}

void CustomLookAndFeel::drawRotarySlider (juce::Graphics& g,
                                          int x,
                                          int y,
                                          int width,
                                          int height,
                                          float sliderPosProportional,
                                          float rotaryStartAngle,
                                          float rotaryEndAngle,
                                          juce::Slider& slider)
{
    g.fillAll(UI::GLOBAL::backColour);
    g.setColour(UI::GLOBAL::strokeLineColour);
    
    //g.drawRect(x, y, width, height);
    
    float size = width > height ? height : width;
    
    juce::Rectangle<float> currentRect(width / 2 - size / 2, height / 2 - size / 2, size, size);
    
    g.drawEllipse(currentRect.reduced(UI::GLOBAL::strokeLineWigthInside,
                                      UI::GLOBAL::strokeLineWigthInside),
                  UI::GLOBAL::strokeLineWigthInside);
    
    juce::Line<float> line(x + width / 2, UI::GLOBAL::strokeLineWigthInside, x + width / 2, (y + height / 2) * 0.5f);
    float currentPos = (rotaryEndAngle - rotaryStartAngle) * sliderPosProportional;
    //DBG(sliderPosProportional);
    line.applyTransform(juce::AffineTransform::rotation(rotaryStartAngle + currentPos, x + width / 2, y + height / 2));
    g.drawLine(line, UI::GLOBAL::strokeLineWigthInside);
}

void CustomLookAndFeel::drawToggleButton(juce::Graphics& g,
                      juce::ToggleButton& button,
                      bool shouldDrawButtonAsHighlighted,
                      bool shouldDrawButtonAsDown)
{
    g.fillAll(UI::GLOBAL::backColour);
    g.setColour(juce::Colours::black);
    
    juce::Rectangle<int> rect(0, 0, button.getBounds().getWidth(), button.getBounds().getHeight());
    rect.reduce(7, 7);
    g.drawRect(rect, UI::GLOBAL::strokeLineWigthInside);
    
    if(button.getToggleState()){
        g.fillRect(rect.reduced(4, 4));
    }
}

juce::Label* CustomLookAndFeel::createSliderTextBox(juce::Slider& slider){
    juce::Label* l = new juce::Label();
    
    l->setJustificationType(juce::Justification::centred);
    l->setColour(juce::Label::textColourId, juce::Colours::black);
    
    l->setColour(juce::Label::outlineColourId, juce::Colours::black);
    
    l->setBounds(l->getBounds().withWidth(7));

    return l;
}

juce::Slider::SliderLayout CustomLookAndFeel::getSliderLayout(juce::Slider& slider){
    using namespace juce;
    // 1. compute the actually visible textBox size from the slider textBox size and some additional constraints

    int minXSpace = 0;
    int minYSpace = 0;

    auto textBoxPos = slider.getTextBoxPosition();

    if (textBoxPos == Slider::TextBoxLeft || textBoxPos == Slider::TextBoxRight)
        minXSpace = 30;
    else
        minYSpace = 15;

    auto localBounds = slider.getLocalBounds();

    //auto textBoxWidth  = jmax (0, jmin (slider.getTextBoxWidth(),  localBounds.getWidth() - minXSpace));
    //auto textBoxHeight = jmax (0, jmin (slider.getTextBoxHeight(), localBounds.getHeight() - minYSpace));
    
    auto textBoxWidth = UI::GLOBAL::sliderTextBoxWidth;
    auto textBoxHeight = UI::GLOBAL::sliderTextBoxHeight;

    Slider::SliderLayout layout;

    // 2. set the textBox bounds

    if (textBoxPos != Slider::NoTextBox)
    {
        if (slider.isBar())
        {
            layout.textBoxBounds = localBounds;
        }
        else
        {
            layout.textBoxBounds.setWidth (textBoxWidth);
            layout.textBoxBounds.setHeight (textBoxHeight * 0.7);
            
            //layout.textBoxBounds.setWidth (UI::GLOBAL::sliderTextBoxWidth);
            //layout.textBoxBounds.setHeight (UI::GLOBAL::sliderTextBoxHeight);

            if (textBoxPos == Slider::TextBoxLeft)           layout.textBoxBounds.setX (0);
            else if (textBoxPos == Slider::TextBoxRight)     layout.textBoxBounds.setX (localBounds.getWidth() - textBoxWidth);
            else /* above or below -> centre horizontally */ layout.textBoxBounds.setX ((localBounds.getWidth() - textBoxWidth) / 2);

            if (textBoxPos == Slider::TextBoxAbove)          layout.textBoxBounds.setY (0);
            else if (textBoxPos == Slider::TextBoxBelow)     layout.textBoxBounds.setY (localBounds.getHeight() - textBoxHeight * 0.7);
            else /* left or right -> centre vertically */    layout.textBoxBounds.setY ((localBounds.getHeight() - textBoxHeight) / 2);
        }
    }

    // 3. set the slider bounds

    layout.sliderBounds = localBounds;

    if (slider.isBar())
    {
        layout.sliderBounds.reduce (1, 1);   // bar border
    }
    else
    {
        if (textBoxPos == Slider::TextBoxLeft)       layout.sliderBounds.removeFromLeft (textBoxWidth);
        else if (textBoxPos == Slider::TextBoxRight) layout.sliderBounds.removeFromRight (textBoxWidth);
        else if (textBoxPos == Slider::TextBoxAbove) layout.sliderBounds.removeFromTop (textBoxHeight);
        else if (textBoxPos == Slider::TextBoxBelow) layout.sliderBounds.removeFromBottom (textBoxHeight);

        const int thumbIndent = getSliderThumbRadius (slider);

        if (slider.isHorizontal())    layout.sliderBounds.reduce (thumbIndent, 0);
        else if (slider.isVertical()) layout.sliderBounds.reduce (0, thumbIndent);
    }

    return layout;
}
