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
{}

EnvelopeVisualComponent::~EnvelopeVisualComponent()
{
    for(auto dot : dotsVector)
    {
        delete dot;
    }
    
    for(auto line : linesVector){
        delete line.second;
    }
}

void EnvelopeVisualComponent::paint (juce::Graphics& g)
{
    g.fillAll (UI::GLOBAL::backColour);
    
    g.setColour(UI::ADSR::lineColour); //drawing lines
    for(auto l : linesVector){
        g.drawLine(*(l.second), UI::ADSR::lineThikness);
    }
}

void EnvelopeVisualComponent::resized(){}

void EnvelopeVisualComponent::mouseDoubleClick(const juce::MouseEvent& event) {
    MovingDot* dot = dynamic_cast<MovingDot*>(event.eventComponent);

    if (dot == nullptr) { // if event source is not dot
        auto relativeEvent = event.getEventRelativeTo(this);
        addDot(relativeEvent.getMouseDownX(), relativeEvent.getMouseDownY());
    }
    else {
        removeDot(dot->getId());
    }
        
}

void EnvelopeVisualComponent::mouseDrag(const juce::MouseEvent& event) {
    MovingDot* dot = dynamic_cast<MovingDot*>(event.eventComponent);

    if (dot != nullptr) {
        auto pos = event.getEventRelativeTo(this).getPosition();
        juce::Point<int> centre = stayPointInsideComponent(dot, pos);
        centre = complyOrder(dot, centre);
        dot->setCentrePosition(centre);
                
        auto line = lineBetween(dot->getLeftId(), dot->getId());
        
        if(line){
            auto elem = getLineElement(line);
            delete line;
            line = new juce::Line<float>(dotsVector[getDotIndex(dot->getLeftId())]->getCentrePosition().toFloat(),
                                         dot->getCentrePosition().toFloat());
            elem->second = line;
        }
        
        line = lineBetween(dot->getId(), dot->getRightId());
        
        if(line){
            auto elem = getLineElement(line);
            delete line;
            line = new juce::Line<float>(dot->getCentrePosition().toFloat(),
                                         dotsVector[getDotIndex(dot->getRightId())]->getCentrePosition().toFloat());
            elem->second = line;
        }
        
        repaint();
    }
}

juce::Point<int> EnvelopeVisualComponent::stayPointInsideComponent(MovingDot* dot, juce::Point<int> eventPos) {
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

juce::Point<int> EnvelopeVisualComponent::complyOrder(MovingDot* dot, juce::Point<int> eventPos) {
    int leftIndex = getDotIndex(dot->getLeftId());
    int rigthIndex = getDotIndex(dot->getRightId());

    juce::Point<int> dotCentre(eventPos.getX(), eventPos.getY());

    if (leftIndex != -1) {
        int leftX = dotsVector[leftIndex]->getCentrePosition().getX();
        if (dotCentre.getX() < leftX) {
            dotCentre.setX(leftX);
        }
    }

    if (rigthIndex != -1) {
        int rightX = dotsVector[rigthIndex]->getCentrePosition().getX();
        if (dotCentre.getX() > rightX) {
            dotCentre.setX(rightX);
        }
    }

    return dotCentre;
}

EnvelopeVisualComponent::MovingDot* EnvelopeVisualComponent::getLeftDot(int x, int y){
    EnvelopeVisualComponent::MovingDot* dot = nullptr;
    
    std::vector<MovingDot*>::iterator iter = dotsVector.begin();
    int minDistance = std::numeric_limits<int>::max();

    while (iter != dotsVector.end()) { // find nearest left dot
        if ((*iter)->getCentrePosition().getX() < x) {
            int tmp = std::abs((*iter)->getCentrePosition().getX() - x);
            if(tmp < minDistance){
                minDistance = tmp;
                dot = (*iter);
            }
        }
        ++iter;
    }
    
    return dot;
}

EnvelopeVisualComponent::MovingDot* EnvelopeVisualComponent::getRightDot(int x, int y){
    MovingDot* dot = nullptr;
    
    std::vector<MovingDot*>::iterator iter = dotsVector.end();
    
    int minDistance = std::numeric_limits<int>::max();

    while (iter != dotsVector.begin()) { // find nearest rigth dot
        --iter; // cause end() iter is out of bounds

        if ((*iter)->getCentrePosition().getX() > x) {
            int tmp = std::abs((*iter)->getCentrePosition().getX() - x);
            if(tmp < minDistance){
                minDistance = tmp;
                dot = (*iter);
            }
        }
    }
    
    return dot;
}

LinesVector::iterator EnvelopeVisualComponent::getLineElement(juce::Line<float>* line){
    LinesVector::iterator iter = linesVector.begin();
    
    while(iter != linesVector.end()){
        if(iter->second == line){
            return iter;
        }
        ++iter;
    }
    
    return linesVector.end();
}

void EnvelopeVisualComponent::addLine(EnvelopeVisualComponent::MovingDot const * leftDot, EnvelopeVisualComponent::MovingDot const * rightDot){
    LinesVectorElement elem(std::make_pair(leftDot->getId(), rightDot->getId()),
                            new juce::Line<float>(leftDot->getCentrePosition().toFloat(),
                                                  rightDot->getCentrePosition().toFloat()));
    linesVector.push_back(elem);

}

void EnvelopeVisualComponent::removeLine(int leftId, int rightId){
    auto line = lineBetween(leftId, rightId);
    if(line){
        auto elem = getLineElement(line);
        delete elem->second;
        linesVector.erase(elem);
    }
}

void EnvelopeVisualComponent::addDot(int x, int y) {
    MovingDot* leftDot = getLeftDot(x, y);
    if(leftDot){
        dotsVector[getDotIndex(leftDot->getId())]->setRightId(dotsIdCounted);
    }
    
    MovingDot* rightDot = getRightDot(x, y);
    if(rightDot){
        dotsVector[getDotIndex(rightDot->getId())]->setLeftId(dotsIdCounted);
    }

    MovingDot* dot = new MovingDot(dotsIdCounted,
                                 leftDot ? leftDot->getId() : 0,
                                 rightDot ? rightDot->getId() : std::numeric_limits<int>::max(),
                                 true);

    dotsVector.push_back(dot);
    ++dotsIdCounted;
    
    dot->setCentrePosition(x, y);
    dot->addMouseListener(this, false);
    addAndMakeVisible(dot);
    
    if(leftDot && rightDot){
        removeLine(leftDot->getId(), rightDot->getId());
    }
    
    if(leftDot){
        addLine(leftDot, dot);
    }
    
    if(rightDot){
        addLine(dot, rightDot);
        
    }
    
    repaint();
}

void EnvelopeVisualComponent::removeDot(int ID) {
    int dotIndex = getDotIndex(ID);
    
    if (dotIndex != -1) {
        int leftDotIndex = getDotIndex(dotsVector[dotIndex]->getLeftId());
        int rightDotIndex = getDotIndex(dotsVector[dotIndex]->getRightId());
        
        if(leftDotIndex != -1){
            removeLine(dotsVector[dotIndex]->getLeftId(), dotsVector[dotIndex]->getId());
            
            if(rightDotIndex != -1){ //connect before erasin
                dotsVector[leftDotIndex]->setRightId(dotsVector[rightDotIndex]->getId());
            }else{
                dotsVector[leftDotIndex]->setRightId(std::numeric_limits<int>::max());
            }
        }
        
        if(rightDotIndex != -1){
            removeLine(dotsVector[dotIndex]->getId(), dotsVector[dotIndex]->getRightId());
            
            if(leftDotIndex != -1){ //connect before erasin
                dotsVector[rightDotIndex]->setLeftId(dotsVector[leftDotIndex]->getId());
            }else{
                dotsVector[rightDotIndex]->setLeftId(0);
            }
        }

        if (leftDotIndex != -1 && rightDotIndex != -1) {
            addLine(dotsVector[leftDotIndex], dotsVector[rightDotIndex]);
        }

        delete *(dotsVector.begin() + dotIndex);
        dotsVector.erase(dotsVector.begin() + dotIndex);
        
        repaint();
    }
}

int EnvelopeVisualComponent::getDotIndex(int ID) {
    std::vector<MovingDot*>::iterator iter(dotsVector.begin());
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

juce::Line<float>* EnvelopeVisualComponent::lineBetween(int leftId, int rightId){
    juce::Line<float>* line = nullptr;
    
    for(auto l : linesVector){
        if(l.first.first == leftId && l.first.second == rightId)
            line = l.second;
    }
    
    return line;
}

//######################################################################

//######################################################################

EnvelopeVisualComponent::MovingDot::MovingDot(int ID, int leftID, int rightID, bool doubleClickAllow) :
    juce::Component(), doubleClickAllowed(doubleClickAllow),
    mouseIsOn(true), dotId(ID), leftDotId(leftID), rightDotId(rightID)
{
    dotBounds = juce::Rectangle<int>(UI::ADSR::movedDotDiameter, UI::ADSR::movedDotDiameter);
    
    setOpaque(false);
    setBounds(juce::Rectangle<int>(UI::ADSR::movedDotAreaSize, UI::ADSR::movedDotAreaSize));
    dotBounds.setCentre(getLocalBounds().getCentre());
}

void EnvelopeVisualComponent::MovingDot::paint(juce::Graphics& g){
    g.setOpacity(0.0f);
    g.fillAll(juce::Colour::fromRGBA(0, 0, 0, 0));
    g.setOpacity(1.0f);
    
    g.drawRect(getLocalBounds());

    g.setColour(mouseIsOn ? UI::ADSR::dotMouseOnColour : UI::ADSR::dotColour);
    g.fillEllipse(dotBounds.toFloat());
}
void EnvelopeVisualComponent::MovingDot::resized(){}

void EnvelopeVisualComponent::MovingDot::mouseEnter(const juce::MouseEvent& event) {
    mouseIsOn = true;
    repaint();
}

void EnvelopeVisualComponent::MovingDot::mouseExit(const juce::MouseEvent& event) {
    mouseIsOn = false;
    repaint();
}

juce::Point<int> EnvelopeVisualComponent::MovingDot::getCentrePosition() const
{
    juce::Point<int> centre(getPosition());
    centre.setX(centre.getX() + getWidth()/2);
    centre.setY(centre.getY() + getWidth()/2);
    return centre;
}
