
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
    
    if(adsrParams){
        delete adsrParams;
    }
}

void EnvelopeVisualComponent::setEnvelopeAsADSR(APVTS* apvtsListenTo, juce::StringArray paramsIDsListenTo){
    isADSR = true;
    
    adsrParams = new ADSRParameters;
    adsrParams->apvts = apvtsListenTo;
    adsrParams->idList = paramsIDsListenTo;
    
    startTimer(200); // cause needs to bounds of component have been set
                     // and apvts have been create
};

void EnvelopeVisualComponent::startEnvelope(){
    startTimer(1000/60); // 1/60 of second
}

void EnvelopeVisualComponent::stopEnvelope(){
    stopTimer();
}

void EnvelopeVisualComponent::timerCallback(){
    if(isADSR && adsrIsSettedUp){
        
    }
    
    if(isADSR && !adsrIsSettedUp){
        setupAdsr(); updateAdsr(); stopTimer();
        adsrParams->apvts->addParameterListener(adsrParams->idList[0], this); // listen attack change
        adsrParams->apvts->addParameterListener(adsrParams->idList[1], this); // listen decay change
        adsrParams->apvts->addParameterListener(adsrParams->idList[2], this); // listen sustain change
        adsrParams->apvts->addParameterListener(adsrParams->idList[3], this); // listen release change
        
        adsrIsSettedUp = true;
    }
};

void EnvelopeVisualComponent::setupAdsr(){
    adsrParams->attackDotId = addDot(0, getHeight(), false, true, true); // attack
    adsrParams->decayDotId = addDot(20, 0, false, false, true); // decay
    adsrParams->sustainDotId = addDot(40, 40, false, false, false); // sustain
    adsrParams->releaseDotId = addDot(60, getHeight(), false, false, true); // release
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
        if(!isADSR){
            auto relativeEvent = event.getEventRelativeTo(this);
            addDot(relativeEvent.getMouseDownX(), relativeEvent.getMouseDownY(),
                   true, false, false);
        }
    }
    else {
        if(dot->removable())
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
        
        if(!isADSR){
            updateLine(dot->getLeftId(), dot->getId());
            updateLine(dot->getId(), dot->getRightId());
        }else{
            updateApvtsValues(dot->getId());
        }
        
        repaint();
    }
}

void EnvelopeVisualComponent::updateApvtsValues(int ID){
    if(isADSR && adsrParams){
        static double widthScale = getWidth() / widthInSeconds; // pixels int 1 second
        
        static MovingDot* dotStart;
        static MovingDot* dotEnd;
        static int distanceX;
        static double distanceY;
        static float value;
        
        static juce::RangedAudioParameter* param;
        // need it because value in class is normailsed
        juce::NormalisableRange<float> range;
        
        if(ID == adsrParams->decayDotId){
            dotStart = dotsVector[getDotIndex(adsrParams->attackDotId)];
            dotEnd = dotsVector[getDotIndex(adsrParams->decayDotId)];
            
            distanceX = dotEnd->getCentrePosition().getX() - dotStart->getCentrePosition().getX();
            value = distanceX / widthScale;

            param = adsrParams->apvts->getParameter(adsrParams->idList[0]);
            range = param->getNormalisableRange();
            
            if(value > range.start && value < range.end)
                param->setValueNotifyingHost(range.convertTo0to1(value));
            
        }
        
        if(ID == adsrParams->sustainDotId){
            dotStart = dotsVector[getDotIndex(adsrParams->decayDotId)];
            dotEnd = dotsVector[getDotIndex(adsrParams->sustainDotId)];
            
            distanceX = dotEnd->getCentrePosition().getX() - dotStart->getCentrePosition().getX();
            value = distanceX / widthScale;
            
            param = adsrParams->apvts->getParameter(adsrParams->idList[1]);
            range = param->getNormalisableRange();
            
            if(value > range.start && value < range.end)
                param->setValueNotifyingHost(range.convertTo0to1(value));
            
            distanceY = getHeight() - dotEnd->getCentrePosition().getY();
            value = distanceY / getHeight();
            
            param = adsrParams->apvts->getParameter(adsrParams->idList[2]);
            range = param->getNormalisableRange();
            
            if(value > range.start && value < range.end)
                param->setValueNotifyingHost(range.convertTo0to1(value));
        }
        
        if(ID == adsrParams->releaseDotId){
            dotStart = dotsVector[getDotIndex(adsrParams->sustainDotId)];
            dotEnd = dotsVector[getDotIndex(adsrParams->releaseDotId)];
            
            distanceX = dotEnd->getCentrePosition().getX() - dotStart->getCentrePosition().getX();
            value = distanceX / widthScale;
            
            param = adsrParams->apvts->getParameter(adsrParams->idList[3]);
            range = param->getNormalisableRange();
            
            if(value > range.start && value < range.end)
                param->setValueNotifyingHost(range.convertTo0to1(value));        }
    }
}

void EnvelopeVisualComponent::updateLine(int leftId, int rightId){
    auto line = lineBetween(leftId, rightId);
    
    if(line){
        auto elem = getLineElement(line);
        delete line;
        line = new juce::Line<float>(dotsVector[getDotIndex(leftId)]->getCentrePosition().toFloat(),
                                     dotsVector[getDotIndex(rightId)]->getCentrePosition().toFloat());
        elem->second = line;
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
    
    dot->setCentrePosition(juce::Point<int>(x, y));
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

void EnvelopeVisualComponent::parameterChanged(const juce::String &parameterID, float newValue){
    if(isADSR && adsrParams){
        
        if(parameterID == adsrParams->idList[0]){ // if attack
            adsrParams->attackTimeSeconds = newValue;
        }else if(parameterID == adsrParams->idList[1]){ // if decay
            adsrParams->decayTimeSeconds = newValue;
        }else if(parameterID == adsrParams->idList[2]){ // if sustain
            adsrParams->sustainLevel = newValue;
        }else if(parameterID == adsrParams->idList[3]){ // if release
            adsrParams->releaseTimeSeconds = newValue;
        }
                                                   
        updateAdsr();
        repaint();
    }
}

void EnvelopeVisualComponent::updateAdsr(){
    // move dots to the right with the current values of seconds or levels
    //-------------REFACTOR THIS----------------------
    if(isADSR && !dotsVector.empty()){
        int widthScale = getWidth() / widthInSeconds; // pixels in 1 second
        
        int width = adsrParams->attackTimeSeconds * widthScale;
        auto dot = dotsVector[getDotIndex(adsrParams->decayDotId)];
        
        dot->setCentrePosition(dot->getCentrePosition().withX(width)); // move dot far from attack dot
        updateLine(adsrParams->attackDotId, adsrParams->decayDotId);
        
        width += adsrParams->decayTimeSeconds * widthScale;
        dot = dotsVector[getDotIndex(adsrParams->sustainDotId)];
        
        dot->setCentrePosition(dot->getCentrePosition().
                               withX(width).
                               withY(getHeight() - adsrParams->sustainLevel * getHeight()));
        
        updateLine(adsrParams->decayDotId, adsrParams->sustainDotId);
        
        width += adsrParams->releaseTimeSeconds * widthScale;
        dot = dotsVector[getDotIndex(adsrParams->releaseDotId)];
        
        dot->setCentrePosition(dot->getCentrePosition().withX(width));
        updateLine(adsrParams->sustainDotId, adsrParams->releaseDotId);
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
void EnvelopeVisualComponent::MovingDot::resized(){}

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
