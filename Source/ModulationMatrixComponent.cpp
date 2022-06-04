#include <JuceHeader.h>
#include "ModulationMatrixComponent.h"

//==============================================================================
ModulationMatrixComponent::ModulationMatrixComponent(juce::StringArray& paramIDs, std::atomic<ModulationMatrixData*>& mData) :
    parametersIDs(paramIDs), matrixData(mData)
{
    //parametersIDs = paramIDs;
    matrixWidth = UI::MODULATION_MATRIX::width - 20;

    for(int i = 0; i < am_of_coloumns; ++i){
        juce::Label* label = new juce::Label(coloumnNames[i], coloumnNames[i]);
        label->setJustificationType(juce::Justification::centred);
        label->setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
        header.push_back(label);
        addAndMakeVisible(label);
    }
    
    //apvts.addParameterListener("ATTACKTIME1", this);
    /*for(auto ID : parametersIDs){
        
        apvts.addParameterListener(ID, this);
    }*/
    
    paramChooser.onChange = [this](){ chooserHandler(); };
    
    paramChooser.addItemList(parametersIDs, 1);
    addAndMakeVisible(paramChooser);
}

void ModulationMatrixComponent::chooserHandler(){
    
    //const juce::MessageManagerLock mmLock;
    int itemIndex = paramChooser.getSelectedItemIndex();
    if(itemIndex != 0 && strLabels[paramChooser.getItemText(itemIndex)] == nullptr){
        
        auto strIter = strings.begin();
        while(strIter != strings.end()){
            
            if(strIter->second[0]->getValue() == 0 &&
               strIter->second[1]->getValue() == 0 &&
               strIter->second[2]->getValue() == 0)
            {
                auto labelItem = strLabels.find(strIter->first);
                if(labelItem != strLabels.end()){
                    delete labelItem->second;
                    strLabels.erase(labelItem);
                }
                
                delete strIter->second[0];
                delete strIter->second[1];
                delete strIter->second[2];
                
                strings.erase(strIter);
                
                matrixData.load()->removeModulatedParameter(strIter->first, coloumnNames[1]);
                matrixData.load()->removeModulatedParameter(strIter->first, coloumnNames[2]);
                matrixData.load()->removeModulatedParameter(strIter->first, coloumnNames[3]);
                
                break;
            }
            
            ++strIter;
        }
        
        std::vector<juce::Slider*> sliders;
        
        juce::Label* label = new juce::Label(paramChooser.getItemText(itemIndex), paramChooser.getItemText(itemIndex));
        label->setJustificationType(juce::Justification::centred);
        label->setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
        
        int y = UI::MODULATION_MATRIX::labelHeight * ((int)strings.size() + 1);
        
        juce::Rectangle<int> compBounds(0, y, labelWidth, UI::MODULATION_MATRIX::labelHeight);
        
        label->setBounds(compBounds);
        compBounds.setX(compBounds.getX() + labelWidth);
        
        juce::NormalisableRange<double> slidersRange(-100.0, 100.0, 1, 1);
        
        juce::Slider* slider1 = new juce::Slider();
        slider1->setBounds(compBounds.withWidth(compBounds.getWidth() - 2*UI::GLOBAL::strokeLineWigthInside));
        slider1->setNormalisableRange(slidersRange);
        slider1->setTextBoxStyle(juce::Slider::TextBoxAbove, true, labelWidth, UI::MODULATION_MATRIX::labelHeight);
        compBounds.setX(compBounds.getX() + labelWidth);
        slider1->addListener(this);
        //slider1->setName("LFO1");
        sliders.push_back(slider1);
        
        juce::Slider* slider2 = new juce::Slider();
        slider2->setBounds(compBounds.withWidth(compBounds.getWidth() - 2*UI::GLOBAL::strokeLineWigthInside));
        slider2->setNormalisableRange(slidersRange);
        slider2->setTextBoxStyle(juce::Slider::TextBoxAbove, true, labelWidth, UI::MODULATION_MATRIX::labelHeight);
        compBounds.setX(compBounds.getX() + labelWidth);
        slider2->addListener(this);
        sliders.push_back(slider2);
        
        juce::Slider* slider3 = new juce::Slider();
        slider3->setBounds(compBounds.withWidth(compBounds.getWidth() - 2*UI::GLOBAL::strokeLineWigthInside));
        slider3->setNormalisableRange(slidersRange);
        slider3->setTextBoxStyle(juce::Slider::TextBoxAbove, true, labelWidth, UI::MODULATION_MATRIX::labelHeight);
        compBounds.setX(compBounds.getX() + labelWidth);
        slider3->addListener(this);
        sliders.push_back(slider3);
        
        addAndMakeVisible(label);
        addAndMakeVisible(slider1);
        addAndMakeVisible(slider2);
        addAndMakeVisible(slider3);
        
        strings[paramChooser.getItemText(itemIndex)] = sliders;
        strLabels[paramChooser.getItemText(itemIndex)] = label;
        
        matrixData.load()->addModulatedParameter(paramChooser.getItemText(itemIndex), coloumnNames[1]);
        matrixData.load()->addModulatedParameter(paramChooser.getItemText(itemIndex), coloumnNames[2]);
        matrixData.load()->addModulatedParameter(paramChooser.getItemText(itemIndex), coloumnNames[3]);
        
        paramChooser.setBounds(paramChooser.getBounds().withY(y + UI::MODULATION_MATRIX::labelHeight));
    }
    
    paramChooser.setSelectedItemIndex(0);
}

ModulationMatrixComponent::~ModulationMatrixComponent()
{
    for(int i = 0; i < am_of_coloumns; ++i){
        if(header[i])
            delete header[i];
    }
    
    auto strIter = strings.begin();
    while(strIter != strings.end()){
        auto labelItem = strLabels.find(strIter->first);
        if(labelItem != strLabels.end()){
            delete labelItem->second;
            //strLabels.erase(labelItem);
        }
            
        delete strIter->second[0];
        delete strIter->second[1];
        delete strIter->second[2];
        
        ++strIter;
    }
    
    strings.clear();
    strLabels.clear();
}

void ModulationMatrixComponent::sliderValueChanged (juce::Slider *slider){
    auto stringIter = strings.begin();
    while(stringIter != strings.end()){
        
        if(stringIter->second[0] == slider){
            matrixData.load()->updateDepth(stringIter->first, coloumnNames[1], slider->getValue() / 100.f);
            break;
        }else if(stringIter->second[1] == slider){
            matrixData.load()->updateDepth(stringIter->first, coloumnNames[2], slider->getValue() / 100.f);
            break;
        }else if (stringIter->second[2] == slider){
            matrixData.load()->updateDepth(stringIter->first, coloumnNames[3], slider->getValue() / 100.f);
            break;
        }
        
        ++stringIter;
    }
}

void ModulationMatrixComponent::paint (juce::Graphics& g)
{
    g.fillAll (UI::GLOBAL::backColour);   // clear the background
    
    for(int i = 0; i <= am_of_coloumns; ++i){
        int x = i * labelWidth - UI::GLOBAL::strokeLineWigthInside;
        juce::Line<float> line(x, 0, x, getMatrixHeight());
        g.drawLine(line, UI::GLOBAL::strokeLineWigthInside);
    }
}

void ModulationMatrixComponent::resized()
{
    labelWidth = matrixWidth / am_of_coloumns;
    
    juce::Rectangle<int> labelBounds(0, 0, labelWidth, UI::MODULATION_MATRIX::labelHeight);
    
    matrixHeight = labelBounds.getHeight();
    for(int i = 0; i < am_of_coloumns; ++i){
        header[i]->setBounds(labelBounds);
        labelBounds.setX(labelBounds.getX() + labelWidth);
    }
    
    juce::Rectangle<int> rect(0, labelBounds.getHeight(), matrixWidth, UI::MODULATION_MATRIX::labelHeight);
    paramChooser.setBounds(rect);
}

void ModulationMatrixComponent::setCustomLookAndFeel(CustomLookAndFeel* lookAndFeel){
    paramChooser.setLookAndFeel(lookAndFeel);
}
