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
    
    for(auto line : linesVector){
        delete line.second;
    }
}

void EnvelopeVisualComponent::paint (juce::Graphics& g)
{
    g.fillAll (UI::GLOBAL::backColour);
    
    g.setColour(UI::ADSR::lineColour);
    for(auto l : linesVector){
        g.drawLine(*(l.second), UI::ADSR::lineThikness);
    }

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("EnvelopeVisualComponent", getLocalBounds(),
                juce::Justification::centred, true);
}

void EnvelopeVisualComponent::resized(){}

void EnvelopeVisualComponent::mouseDoubleClick(const juce::MouseEvent& event) {
    MovedDot* dot = dynamic_cast<MovedDot*>(event.eventComponent);

    if (dot == nullptr) {
        auto relativeEvent = event.getEventRelativeTo(this);
        addDot(relativeEvent.getMouseDownX(), relativeEvent.getMouseDownY());
    }
    else {
        removeDot(dot->getId());
    }
        
}

LinesVector::iterator EnvelopeVisualComponent::getLineElement(juce::Line<float>* line){
    //LinesVectorElement* elem = nullptr;
    LinesVector::iterator iter = linesVector.begin();
    
    while(iter != linesVector.end()){
        if(iter->second == line){
            return iter;
        }
        ++iter;
    }
    
    return linesVector.end();
}


void EnvelopeVisualComponent::mouseDrag(const juce::MouseEvent& event) {
    MovedDot* dot = dynamic_cast<MovedDot*>(event.eventComponent);

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
        
        /*for(auto line : linesVector){
            if(line->isLineBetween(dot->getLeftId(), dot->getId())){
                auto leftDot = dotsVector[getDotIndex(dot->getLeftId())];
                line->updatePositions(leftDot->getCentrePosition(), dot->getCentrePosition());
            }
            
            if(line->isLineBetween(dot->getId(), dot->getRightId())){
                auto rightDot = dotsVector[getDotIndex(dot->getRightId())];
                line->updatePositions(dot->getCentrePosition(), rightDot->getCentrePosition());
            }
        }*/
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

MovedDot* EnvelopeVisualComponent::getLeftDot(int x, int y){
    MovedDot* dot = nullptr;
    
    std::vector<MovedDot*>::iterator iter = dotsVector.begin();
    int minDistance = std::numeric_limits<int>::max();

    while (iter != dotsVector.end()) { // find nearest left dot
        if ((*iter)->getCentrePosition().getX() < x) {
            int tmp = std::abs((*iter)->getCentrePosition().getX() - x);
            if(tmp < minDistance){
                minDistance = tmp;
                dot = (*iter);
                //leftID = (*iterStart)->getId();
            }
        }
        ++iter;
    }
    
    return dot;
}

MovedDot* EnvelopeVisualComponent::getRightDot(int x, int y){
    MovedDot* dot = nullptr;
    
    std::vector<MovedDot*>::iterator iter = dotsVector.end();
    
    int minDistance = std::numeric_limits<int>::max();

    while (iter != dotsVector.begin()) { // find nearest rigth dot
        --iter; // cause end() iter is out of bounds

        if ((*iter)->getCentrePosition().getX() > x) {
            int tmp = std::abs((*iter)->getCentrePosition().getX() - x);
            if(tmp < minDistance){
                minDistance = tmp;
                dot = (*iter);
                //rightID = (*iterEnd)->getId();
            }
        }
    }
    
    return dot;
}

void EnvelopeVisualComponent::addDot(int x, int y) {
    //-----------------------------------------------
    MovedDot* leftDot = getLeftDot(x, y);
    if(leftDot){
        dotsVector[getDotIndex(leftDot->getId())]->setRightId(dotsIdConted);
    }
    
    MovedDot* rightDot = getRightDot(x, y);
    if(rightDot){
        dotsVector[getDotIndex(rightDot->getId())]->setLeftId(dotsIdConted);
    }

    MovedDot* dot = new MovedDot(dotsIdConted,
                                 leftDot ? leftDot->getId() : 0,
                                 rightDot ? rightDot->getId() : std::numeric_limits<int>::max(),
                                 true);

    /*DBG(dotsIdConted);
    DBG(leftID);
    DBG(rightID);
    DBG("-----------");*/
    dotsVector.push_back(dot);
    //dotsIDs.push_back(dotsAmount);
    ++dotsIdConted;
    
    dot->setCentrePosition(x, y);
    dot->addMouseListener(this, false);
    addAndMakeVisible(dot);
    //-----------------------------------------------
    if(leftDot && rightDot){
        auto line = lineBetween(leftDot->getId(), rightDot->getId());
        if(line){
            auto elem = getLineElement(line);
            delete elem->second;
            linesVector.erase(elem);
        }
    }
    
    if(leftDot){
        /*LineBetween* line = new LineBetween(leftDot, leftDot->getCentrePosition(), dot, dot->getCentrePosition());
        linesVector.push_back(line);
        line->addMouseListener(this, false);
        addAndMakeVisible(line);*/
        LinesVectorElement elem(std::make_pair(leftDot->getId(), dot->getId()),
                                new juce::Line<float>(leftDot->getCentrePosition().toFloat(),
                                                      dot->getCentrePosition().toFloat()));
        linesVector.push_back(elem);
    }
    
    if(rightDot){
        /*LineBetween* line = new LineBetween(dot, dot->getCentrePosition(), rightDot, rightDot->getCentrePosition());
        linesVector.push_back(line);
        line->addMouseListener(this, false);
        addAndMakeVisible(line);*/
        LinesVectorElement elem(std::make_pair(dot->getId(), rightDot->getId()),
                                new juce::Line<float>(dot->getCentrePosition().toFloat(),
                                                      rightDot->getCentrePosition().toFloat()));
        linesVector.push_back(elem);
        
    }
    //-----------------------------------------------
    repaint();
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

juce::Line<float>* EnvelopeVisualComponent::lineBetween(int leftId, int rightId){
    juce::Line<float>* line = nullptr;
    
    for(auto l : linesVector){
        if(l.first.first == leftId && l.first.second == rightId)
            line = l.second;
    }
    
    return line;
}

//######################################################################

/*LineBetween::LineBetween(MovedDot* leftDot, juce::Point<int> leftDotPos, MovedDot* rightDot, juce::Point<int> rightDotPos) :
    juce::Component(),
    leftDot(leftDot), leftDotCentre(leftDotPos),
    rightDot(rightDot), rightDotCentre(rightDotPos)
{
    setOpaque(false);
    setBounds(juce::Rectangle<int>(leftDotCentre, rightDotCentre));
}

void LineBetween::paint(juce::Graphics& g){
    g.setOpacity(0.0f);
    g.fillAll(juce::Colour::fromRGBA(0, 0, 0, 0));
    g.setOpacity(1.0f);
    
    g.setColour(UI::ADSR::lineColour);
    //g.drawRect(localBounds);
    g.drawRect(getLocalBounds());
    paintLine(g);
}

void LineBetween::paintLine(juce::Graphics& g){
    
    juce::Line<float> line;
    if(leftDotCentre.getY() < rightDotCentre.getY()){
        line = juce::Line<float>(getLocalBounds().getTopLeft().toFloat(),
                                 getLocalBounds().getBottomRight().toFloat());
    }else{
        line = juce::Line<float>(getLocalBounds().getBottomLeft().toFloat(), getLocalBounds().getTopRight().toFloat());
    }
    //juce::Line<float> line(getLocalBounds().getBottomLeft().toFloat(), getLocalBounds().getTopRight().toFloat());
    g.drawLine(line, UI::ADSR::lineThikness);

    
}

void LineBetween::updatePositions(juce::Point<int> leftCentre, juce::Point<int> rightCentre){
    leftDotCentre = leftCentre; rightDotCentre = rightCentre;
    setBounds(juce::Rectangle<int>(leftDotCentre, rightDotCentre));
    
    juce::AffineTransform tr;
    int one = (leftCentre.getX() + rightCentre.getX()) / 2;
    int two = (leftCentre.getX() + rightCentre.getX()) / 2;
    tr.transformPoint<int>(one, two);
    tr.rotation(rightCentre.getY());
    setBounds(getLocalBounds().transformedBy(tr));
};

void LineBetween::resized(){
    
};*/

//######################################################################

MovedDot::MovedDot(int ID, int leftID, int rightID, bool doubleClickAllow) : 
    juce::Component(), doubleClickAllowed(doubleClickAllow),
    mouseIsOn(true), dotId(ID), leftDotId(leftID), rightDotId(rightID)
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
    
    g.drawRect(localBounds);

    g.setColour(mouseIsOn ? UI::ADSR::dotMouseOnColour : UI::ADSR::dotColour);
    g.fillEllipse(dotBounds.toFloat());
}
void MovedDot::resized(){

}

void MovedDot::mouseEnter(const juce::MouseEvent& event) {
    mouseIsOn = true;
    repaint();
}

void MovedDot::mouseExit(const juce::MouseEvent& event) {
    mouseIsOn = false;
    repaint();
}

juce::Point<int> MovedDot::getCentrePosition() const
{
    juce::Point<int> centre(getPosition());
    centre.setX(centre.getX() + getWidth()/2);
    centre.setY(centre.getY() + getWidth()/2);
    return centre;
}
