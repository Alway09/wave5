/*
  ==============================================================================

    EnvelopeVisualComponent.cpp
    Created: 3 May 2022 1:22:38pm
    Author:  alway

  ==============================================================================
*/

#include <JuceHeader.h>
#include "EnvelopeVisualComponent.h"
#include <limits>

//==============================================================================
EnvelopeVisualComponent::EnvelopeVisualComponent() :
    juce::Component()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

EnvelopeVisualComponent::~EnvelopeVisualComponent()
{
    for(auto dot : dotsVector)
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
        auto pos = event.getEventRelativeTo(this).getPosition();
        juce::Point<int> centre = stayPointInsideComponent(dot, pos);
        centre = complyOrder(dot, centre);
        dot->setCentrePosition(centre);
    }
}

juce::Point<int> EnvelopeVisualComponent::stayPointInsideComponent(MovedDot* dot, juce::Point<int> eventPos) {
    juce::Point<int> centre;

    if (eventPos.getX() < 0) {
        centre.setX(0);
    }
    else if (eventPos.getX() > getWidth()) {
        centre.setX(getWidth());
    }
    else {
        centre.setX(eventPos.getX());
    }

    if (eventPos.getY() < 0) {
        centre.setY(0);
    }
    else if (eventPos.getY() > getHeight()) {
        centre.setY(getHeight());
    }
    else {
        centre.setY(eventPos.getY());
    }

    return centre;
}

juce::Point<int> EnvelopeVisualComponent::complyOrder(MovedDot* dot, juce::Point<int> eventPos) {
    int leftIndex = getDotIndex(dot->getLeftId());
    int rigthIndex = getDotIndex(dot->getRightId());

    //int leftX = 0;
    //int rightX = getWidth();
    juce::Point<int> dotCentre(eventPos.getX(), eventPos.getY());

    if (leftIndex != -1) {
        int leftX = dotsVector[leftIndex]->getX() + dotsVector[leftIndex]->getWidth() / 2;
        if (dotCentre.getX() < leftX) {
            dotCentre.setX(leftX);
            //dot->setCentrePosition(dotCentre);

            //return;
        }
    }

    if (rigthIndex != -1) {
        //DBG(rigthIndex);
        int rightX = dotsVector[rigthIndex]->getX() + dotsVector[rigthIndex]->getWidth() / 2;
        if (dotCentre.getX() > rightX) {
            dotCentre.setX(rightX);
            //dot->setCentrePosition(dotCentre);

            //return;
        }
    }

    return dotCentre;
}

void EnvelopeVisualComponent::addDot(int x, int y) {
    std::vector<MovedDot*>::iterator iterStart = dotsVector.begin();
    int leftID = 0; 
    //int rightID = std::numeric_limits<int>::max();

    while (iterStart != dotsVector.end()) { // find left dot
        if ((*iterStart)->getPosition().getX() + (*iterStart)->getWidth() < x) {
            leftID = (*iterStart)->getId();
            //(*iterStart)->setRightId(dotsIdConted);
        }

        ++iterStart;
    }

    if(leftID != 0)
        dotsVector[getDotIndex(leftID)]->setRightId(dotsIdConted);

    std::vector<MovedDot*>::iterator iterEnd = dotsVector.end(); 
    int rightID = std::numeric_limits<int>::max();

    while (iterEnd != dotsVector.begin()) { // find rigth dot
        --iterEnd; // cause end() iter is out of bounds

        if ((*iterEnd)->getPosition().getX() + (*iterEnd)->getWidth() > x) {
            rightID = (*iterEnd)->getId();
            //(*iterEnd)->setLeftId(dotsIdConted);
        }   
    }

    if(rightID != std::numeric_limits<int>::max())
        dotsVector[getDotIndex(rightID)]->setLeftId(dotsIdConted);

    MovedDot* dot = new MovedDot(dotsIdConted, leftID, rightID, true);

    DBG(dotsIdConted);
    DBG(leftID);
    DBG(rightID);
    DBG("-----------");

    dotsVector.push_back(dot);
    //dotsIDs.push_back(dotsAmount);

    ++dotsIdConted;

    dot->setCentrePosition(x, y);
    dot->addMouseListener(this, false);
    addAndMakeVisible(dot);
}

void EnvelopeVisualComponent::removeDot(int ID) {
    int dotIndex = getDotIndex(ID);  
    
    if (dotIndex != -1) {
        int leftDotIndex = getDotIndex(dotsVector[dotIndex]->getLeftId());
        int rightDotIndex = getDotIndex(dotsVector[dotIndex]->getRightId());

        if (leftDotIndex != -1 && rightDotIndex != -1) {
            dotsVector[leftDotIndex]->setRightId(dotsVector[rightDotIndex]->getId()); //connect before erasing
            dotsVector[rightDotIndex]->setRightId(dotsVector[leftDotIndex]->getId());
        } 

        delete *(dotsVector.begin() + dotIndex);
        dotsVector.erase(dotsVector.begin() + dotIndex);
        //dotsIDs.erase(dotsIDs.begin() + dotIndex);
    }
}

int EnvelopeVisualComponent::getDotIndex(int ID) {
    std::vector<MovedDot*>::iterator iter(dotsVector.begin());
    int counter = 0;

    while (iter != dotsVector.end()) {
        if ((*iter)->getId() == ID) {
            return counter;
        }

        ++iter;
        ++counter;
    }

    return -1;
}

MovedDot::MovedDot(int ID, int leftID, int rightID, bool doubleClickAllow) : 
    juce::Component(), dotId(ID), doubleClickAllowed(doubleClickAllow) , 
    mouseIsOn(true), leftDotId(leftID), rightDotId(rightID)
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
