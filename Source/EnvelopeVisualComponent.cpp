/*
  ==============================================================================

    EnvelopeVisualComponent.cpp
    Created: 3 May 2022 1:22:38pm
    Author:  alway

  ==============================================================================
*/

#include <JuceHeader.h>
#include "EnvelopeVisualComponent.h"

//==============================================================================
EnvelopeVisualComponent::EnvelopeVisualComponent() :
    juce::Component()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

EnvelopeVisualComponent::~EnvelopeVisualComponent()
{
    for each (auto dot in dotsVector)
    {
        delete dot;
    }
}

void EnvelopeVisualComponent::paint (juce::Graphics& g)
{
    g.fillAll (UI::GLOBAL::backColour);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("EnvelopeVisualComponent", getLocalBounds(),
                juce::Justification::centred, true);
}

void EnvelopeVisualComponent::resized()
{
    
}

void EnvelopeVisualComponent::mouseDoubleClick(const juce::MouseEvent& event) {
    MovedDot* dot = dynamic_cast<MovedDot*>(event.eventComponent);

    if (dot == nullptr) {
        addDot(event.getMouseDownX(), event.getMouseDownY());
    }
    else {
        removeDot(dot->getId());
    }
        
}

void EnvelopeVisualComponent::mouseDrag(const juce::MouseEvent& event) {
    MovedDot* dot = dynamic_cast<MovedDot*>(event.eventComponent);

    if (dot != nullptr) {
        auto relativeEvent = event.getEventRelativeTo(this);

        dot->setCentrePosition(relativeEvent.getPosition());
    }
}

void EnvelopeVisualComponent::addDot(int x, int y) {
    MovedDot* dot = new MovedDot(dotsAmount, true);
    dotsVector.push_back(dot);
    ++dotsAmount;

    dot->setCentrePosition(x, y);

    dot->addMouseListener(this, false);
    addAndMakeVisible(dot);
}

void EnvelopeVisualComponent::removeDot(int ID) {

    std::vector<MovedDot*>::iterator iter(dotsVector.begin());

    while(iter != dotsVector.end())
    {
        if ((*iter)->getId() == ID) {
            delete (*iter);
            dotsVector.erase(iter);
            break;
        }

        ++iter;
    }
}

MovedDot::MovedDot(int ID, bool doubleClickAllow) : juce::Component(), dotId(ID), doubleClickAllowed(doubleClickAllow) , mouseIsOn(true)
{
    localBounds = juce::Rectangle<int>(UI::ADSR::movedDotAreaSize, UI::ADSR::movedDotAreaSize);
    dotBounds = juce::Rectangle<int>(UI::ADSR::movedDotDiameter, UI::ADSR::movedDotDiameter);

    updateBounds();

    setOpaque(false);
    setBounds(localBounds);
}

void MovedDot::updateBounds() {   
    dotBounds.setCentre(localBounds.getCentre());
}

void MovedDot::paint(juce::Graphics& g){
    g.setOpacity(0.0f);
    g.fillAll(juce::Colour::fromRGBA(0, 0, 0, 0));
    g.setOpacity(1.0f);

    g.setColour(mouseIsOn ? UI::ADSR::dotMouseOnColour : UI::ADSR::dotColour);
    g.fillEllipse(dotBounds.toFloat());
}
void MovedDot::resized(){

}

void MovedDot::mouseEnter(const juce::MouseEvent& event) {
    mouseIsOn = true;
    //DBG("ONNNNNNNNN");
    repaint();
}

void MovedDot::mouseExit(const juce::MouseEvent& event) {
    mouseIsOn = false;
    repaint();
}
