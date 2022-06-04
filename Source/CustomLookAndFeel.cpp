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

void CustomLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool,
                                   int, int, int, int, juce::ComboBox& box)
{
    using namespace juce;
    
    auto cornerSize = box.findParentComponentOfClass<ChoicePropertyComponent>() != nullptr ? 0.0f : 3.0f;
    Rectangle<int> boxBounds (0, 0, width, height);

    g.setColour (UI::GLOBAL::backColour);
    g.fillRoundedRectangle (boxBounds.toFloat(), cornerSize);

    g.setColour (box.findColour (ComboBox::outlineColourId));
    g.drawRoundedRectangle (boxBounds.toFloat().reduced (0.5f, 0.5f), cornerSize, 1.0f);

    Rectangle<int> arrowZone (width - 30, 0, 20, height);
    Path path;
    path.startNewSubPath ((float) arrowZone.getX() + 3.0f, (float) arrowZone.getCentreY() - 2.0f);
    path.lineTo ((float) arrowZone.getCentreX(), (float) arrowZone.getCentreY() + 3.0f);
    path.lineTo ((float) arrowZone.getRight() - 3.0f, (float) arrowZone.getCentreY() - 2.0f);

    g.setColour (Colours::black.withAlpha ((box.isEnabled() ? 0.9f : 0.2f)));
    g.strokePath (path, PathStrokeType (2.0f));
}

void CustomLookAndFeel::drawPopupMenuItem (juce::Graphics& g, const juce::Rectangle<int>& area,
                        const bool isSeparator, const bool isActive,
                        const bool isHighlighted, const bool isTicked,
                        const bool hasSubMenu, const juce::String& text,
                        const juce::String& shortcutKeyText,
                        const juce::Drawable* icon, const juce::Colour* const textColourToUse)
{
    using namespace juce;
    
    if (isSeparator)
    {
        auto r  = area.reduced (5, 0);
        r.removeFromTop (roundToInt (((float) r.getHeight() * 0.5f) - 0.5f));

        g.setColour (findColour (PopupMenu::textColourId).withAlpha (0.3f));
        //g.setColour (Colours::black);
        g.fillRect (r.removeFromTop (1));
    }
    else
    {
        auto textColour = (textColourToUse == nullptr ? Colours::black
                                                      : *textColourToUse);

        auto r  = area.reduced (1);

        if (isHighlighted && isActive)
        {
            g.setColour (UI::GLOBAL::backColour.darker());
            g.fillRect (r);

            g.setColour (findColour (PopupMenu::highlightedTextColourId));
        }
        else
        {
            g.setColour (textColour.withMultipliedAlpha (isActive ? 1.0f : 0.5f));
        }

        r.reduce (jmin (5, area.getWidth() / 20), 0);

        auto font = Font(17.f);

        auto maxFontHeight = (float) r.getHeight() / 1.3f;

        if (font.getHeight() > maxFontHeight)
            font.setHeight (maxFontHeight);

        g.setFont (font);

        auto iconArea = r.removeFromLeft (roundToInt (maxFontHeight)).toFloat();

        if (icon != nullptr)
        {
            icon->drawWithin (g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
            r.removeFromLeft (roundToInt (maxFontHeight * 0.5f));
        }
        else if (isTicked)
        {
            //auto tick = getTickShape (1.0f);
            //g.fillPath (tick, tick.getTransformToScaleToFit (iconArea.reduced (iconArea.getWidth() / 5, 0).toFloat(), true));
        }

        if (hasSubMenu)
        {
            auto arrowH = 0.6f * getPopupMenuFont().getAscent();

            auto x = static_cast<float> (r.removeFromRight ((int) arrowH).getX());
            auto halfH = static_cast<float> (r.getCentreY());

            Path path;
            path.startNewSubPath (x, halfH - arrowH * 0.5f);
            path.lineTo (x + arrowH * 0.6f, halfH);
            path.lineTo (x, halfH + arrowH * 0.5f);

            g.strokePath (path, PathStrokeType (2.0f));
        }

        r.removeFromRight (3);
        g.drawFittedText (text, r, Justification::centredLeft, 1);

        if (shortcutKeyText.isNotEmpty())
        {
            auto f2 = font;
            f2.setHeight (f2.getHeight() * 0.75f);
            f2.setHorizontalScale (0.95f);
            g.setFont (f2);

            g.drawText (shortcutKeyText, r, Justification::centredRight, true);
        }
    }
}

void CustomLookAndFeel::drawPopupMenuBackground (juce::Graphics& g, int width, int height){
    
    using namespace juce;
    
    auto background = UI::GLOBAL::backColour;

    g.fillAll (background);
    g.setColour (background.overlaidWith (Colour (0x2badd8e6)));

    for (int i = 0; i < height; i += 3)
        g.fillRect (0, i, width, 1);

   #if ! JUCE_MAC
    g.setColour (findColour (PopupMenu::textColourId).withAlpha (0.6f));
    g.drawRect (0, 0, width, height);
   #endif
}
