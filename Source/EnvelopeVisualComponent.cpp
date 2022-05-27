
#include <JuceHeader.h>
#include "EnvelopeVisualComponent.h"
#include <limits>

//==============================================================================
EnvelopeVisualComponent::EnvelopeVisualComponent() :
    juce::Component()
{
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
        addDot(relativeEvent.getMouseDownX(), relativeEvent.getMouseDownY(),
               true, false, false);
        
    }
    else {
        if(dot->removable())
            removeDot(dot->getId());
    }
}

void EnvelopeVisualComponent::mouseDrag(const juce::MouseEvent& event) {
    MovingDot* dot = dynamic_cast<MovingDot*>(event.eventComponent);

    if (dot != nullptr) {
        auto dotPos = dot->getCentrePosition();
        
        auto pos = event.getEventRelativeTo(this).getPosition();
        juce::Point<int> centre = stayPointInsideComponent(pos);
        centre = complyOrder(dot, centre);
        dot->setCentrePosition(centre);
        
        if(!dotsRelativeToParameters.empty()){
            updateRelativeValue(dot, dotPos);
            
        }
            
        
        updateLine(dot->getLeftId(), dot->getId());
        updateLine(dot->getId(), dot->getRightId());
        
        repaint();
    }
}

void EnvelopeVisualComponent::updateRelativeValue(MovingDot* dot, juce::Point<int> dotPreviousePosition){
    jassert(apvts);
    
    auto relativeDotIter = dotsRelativeToParameters.begin();
    while(relativeDotIter != dotsRelativeToParameters.end()){
        
        if(relativeDotIter->second.first == dot->getId()){
            
            if(relativeDotIter->second.second == DotRelationDirection::Vertical){
                float newValue = 1.f - (float)dot->getY() / getHeight();
                
                auto param = apvts->getParameter(relativeDotIter->first);
                apvts->removeParameterListener(relativeDotIter->first, this);
                param->setValueNotifyingHost(newValue);
                apvts->addParameterListener(relativeDotIter->first, this);
                
            }else if(relativeDotIter->second.second == DotRelationDirection::Horizontal){
                int x = 0;
                
                int leftDotIndex = getDotIndex(dot->getLeftId());
                if(leftDotIndex != -1){
                    x = dotsVector[leftDotIndex]->getCentrePosition().getX();
                }
                
                float pixIn1 = getWidth() / widthFactor;
                float newValue = (float)(dot->getCentrePosition().getX() - x) / pixIn1;
                
                auto param = apvts->getParameter(relativeDotIter->first);
                auto paramRange = param->getNormalisableRange();
                
                if(newValue > paramRange.getRange().getEnd()){
                    dot->setCentrePosition(dotPreviousePosition.withY(dot->getCentrePosition().getY()));
                    return;
                }
                
                if(newValue > paramRange.getRange().getStart()){
                    apvts->removeParameterListener(relativeDotIter->first, this);
                    param->setValueNotifyingHost(paramRange.convertTo0to1(newValue));
                    apvts->addParameterListener(relativeDotIter->first, this);
                }
                
                moveRightDots(dot, dot->getCentrePosition().getX() - dotPreviousePosition.getX());
                
            }
        }
        
        ++relativeDotIter;
    }
}

void EnvelopeVisualComponent::updateLine(int leftId, int rightId){
    auto line = lineBetween(leftId, rightId);
    
    if(line){
        auto elem = getLineElement(line);
        delete line;
        
        auto leftDotPos = dotsVector[getDotIndex(leftId)]->getCentrePosition();
        auto rightDotPos = dotsVector[getDotIndex(rightId)]->getCentrePosition();
        
        line = new juce::Line<float>(leftDotPos.toFloat(),
                                     rightDotPos.toFloat());
        elem->second = line;
        
        if(relativeLFO){
            relativeLFO->updatePeriod(leftId, rightId,
                                      (float)leftDotPos.getX() / getWidth(),
                                      (float)rightDotPos.getX() / getWidth(),
                                      1.f - (float)leftDotPos.getY() / getHeight(),
                                      1.f - (float)rightDotPos.getY() / getHeight());
        }
    }
}

juce::Point<int> EnvelopeVisualComponent::stayPointInsideComponent(juce::Point<int> eventPos) {
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
        if ((*iter)->getCentrePosition().getX() <= x) {
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

void EnvelopeVisualComponent::addLine(EnvelopeVisualComponent::MovingDot const * leftDot,
                                      EnvelopeVisualComponent::MovingDot const * rightDot)
{
    auto leftDotPos = leftDot->getCentrePosition();
    auto rightDotPos = rightDot->getCentrePosition();
    
    LinesVectorElement elem(std::make_pair(leftDot->getId(), rightDot->getId()),
                            new juce::Line<float>(leftDotPos.toFloat(),
                                                  rightDotPos.toFloat()));
    linesVector.push_back(elem);
    
    if(relativeLFO){
        relativeLFO->addPeriod(leftDot->getId(), rightDot->getId(),
                               (float)leftDotPos.getX() / getWidth(),
                               (float)rightDotPos.getX() / getWidth(),
                               1.f - (float)leftDotPos.getY() / getHeight(),
                               1.f - (float)rightDotPos.getY() / getHeight());
    }
}

void EnvelopeVisualComponent::removeLine(int leftId, int rightId){
    auto line = lineBetween(leftId, rightId);
    if(line){
        
        if(relativeLFO){
            relativeLFO->deletePeriod(leftId, rightId);
        }
        
        auto elem = getLineElement(line);
        delete elem->second;
        linesVector.erase(elem);
    }
}

int EnvelopeVisualComponent::addDot(int x, int y,
                                     bool removable,
                                     bool withConstantX,
                                     bool withConstantY)
{
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
                                 removable, withConstantX, withConstantY);

    dotsVector.push_back(dot);
    ++dotsIdCounted;
    
    
    dot->setCentrePosition(stayPointInsideComponent(juce::Point<int>(x, y)));
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
    return dot->getId();
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

void EnvelopeVisualComponent::setDotRelativeToParameter(const juce::String& paramId,
                                                        int dotId,
                                                        DotRelationDirection direction)
{
    jassert(apvts);
    
    apvts->addParameterListener(paramId, this);
    dotsRelativeToParameters[paramId] = std::make_pair(dotId, direction);
}

void EnvelopeVisualComponent::parameterChanged(const juce::String &parameterID, float newValue)
{
    jassert(apvts);
    
    auto range = apvts->getParameter(parameterID)->getNormalisableRange().getRange();
    
    auto elem = dotsRelativeToParameters[parameterID];
    
    auto dot = dotsVector[getDotIndex(elem.first)];
    auto leftDot = dotsVector[getDotIndex(dot->getLeftId())];
    auto dotPos = dot->getCentrePosition();
    
    if(elem.second == DotRelationDirection::Horizontal){
        int x = getWidth() / widthFactor * newValue;
        
        if(leftDot){
            x += leftDot->getCentrePosition().getX();
        }
        
        int deltaX = x - dotPos.getX();
        moveRightDots(dot, deltaX);
        
        dotPos.setX(x);
    }else if(elem.second == DotRelationDirection::Vertical){
        float y = juce::jmap(newValue, range.getStart(), range.getEnd(), 0.f, (float)getHeight());
        
        dotPos.setY(getHeight() - y);
        updateLine(dot->getId(), dot->getRightId());
    }
    
    dot->setCentrePosition(dotPos);
    updateLine(dot->getLeftId(), dot->getId());
    repaint();
}

void EnvelopeVisualComponent::moveRightDots(MovingDot* dot, int deltaX){
    
    int rightDotIndex = getDotIndex(dot->getRightId());
    
    if(rightDotIndex != -1){
        auto rightDot = dotsVector[rightDotIndex];

        auto rightDotPos = rightDot->getCentrePosition();
        rightDot->setCentrePosition(rightDotPos.withX(rightDotPos.getX() + deltaX));
        updateLine(dot->getId(), rightDot->getId());
        moveRightDots(rightDot, deltaX);
    }
}

//######################################################################
//######################################################################

EnvelopeVisualComponent::MovingDot::MovingDot(int ID, int leftID, int rightID,
                                              bool makeRemovable, bool withConstantX,
                                              bool withConstantY) :
    juce::Component(), isRemovable(makeRemovable), mouseIsOn(false),
    xIsConstant(withConstantX), yIsConstant(withConstantY),
    dotId(ID), leftDotId(leftID), rightDotId(rightID)
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
    
    //g.drawRect(getLocalBounds());

    g.setColour(mouseIsOn ? UI::ADSR::dotMouseOnColour : UI::ADSR::dotColour);
    g.fillEllipse(dotBounds.toFloat());
}
void EnvelopeVisualComponent::MovingDot::resized(){
    
}

void EnvelopeVisualComponent::MovingDot::mouseEnter(const juce::MouseEvent& event) {
    mouseIsOn = true;
    repaint();
}

void EnvelopeVisualComponent::MovingDot::mouseExit(const juce::MouseEvent& event) {
    mouseIsOn = false;
    repaint();
}

void EnvelopeVisualComponent::MovingDot::setCentrePosition(juce::Point<int> p){
    juce::Point<int> originalCentre = getBounds().getCentre();
    juce::Point<int> resultCentre = p.transformedBy(getTransform().inverted());
    
    if(xIsConstant && positionInitialized)
        resultCentre.setX(originalCentre.getX());
    
    if(yIsConstant && positionInitialized)
        resultCentre.setY(originalCentre.getY());
    
    setBounds(getBounds().withCentre(resultCentre));
    positionInitialized = true;
}

juce::Point<int> EnvelopeVisualComponent::MovingDot::getCentrePosition() const
{
    juce::Point<int> centre(getPosition());
    centre.setX(centre.getX() + getWidth()/2);
    centre.setY(centre.getY() + getWidth()/2);
    return centre;
}
