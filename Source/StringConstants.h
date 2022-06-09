#pragma once

namespace STR_CONST{
    namespace ADSR{
        const juce::StringArray firstAdsrParameters = {
            "ATTACKTIME1",
            "DECAYTIME1",
            "SUSTAINLEVEL1",
            "RELEASETIME1"
        };
    
        const juce::String firstOscOn = "OSCON1";
        const juce::String firstOscGain = "GAINOSC1";
        const juce::String firstOscWaveCoose = "WAVECHOOSEOSC1";
        const juce::String firstOscPan = "PANOSC1";
        const juce::String firstOscTranspose = "TRANSPOSE1";
    
        const juce::StringArray secondAdsrParameters = {
            "ATTACKTIME2",
            "DECAYTIME2",
            "SUSTAINLEVEL2",
            "RELEASETIME2"
        };
    
        const juce::String secondOscOn = "OSCON2";
        const juce::String secondOscGain = "GAINOSC2";
        const juce::String secondOscWaveCoose = "WAVECHOOSEOSC2";
        const juce::String secondOscPan = "PANOSC2";
    
        const juce::String secondOscTranspose = "TRANSPOSE2";
        
        const juce::StringArray thirdAdsrParameters = {
            "ATTACKTIME3",
            "DECAYTIME3",
            "SUSTAINLEVEL3",
            "RELEASETIME3"
        };
    
        const juce::String thirdOscOn = "OSCON3";
        const juce::String thirdOscGain = "GAINOSC3";
        const juce::String thirdOscWaveCoose = "WAVECHOOSEOSC3";
        const juce::String thirdOscPan = "PANOSC3";
        const juce::String thirdOscTranspose = "TRANSPOSE3";
        
        //const juce::String adsrStateId = "ADSRSTATE";
        const juce::StringArray adsrStates = {
            "Idle",
            "Attack",
            "Decay",
            "Sustain",
            "Release"
        };
    
        const juce::StringArray oscWavesVariants = {"Sine", "Square", "Saw", "Triangle", "Noise"};
    }

    namespace LFO{
    
        const juce::StringArray LFOWorkModes = {"Trig", "Env", "Off"};
        const juce::StringArray LFORateModes = {"BPM", "Hz"};
    
        const juce::String firstLFOWorkMode = "LFOWMOD1";
        const juce::String secondLFOWorkMode = "LFOWMOD2";
        const juce::String thirdLFOWorkMode = "LFOWMOD3";
    
        const juce::String firstLFORateMode = "LFORMODE1";
        const juce::String secondLFORateMode = "LFORMODE2";
        const juce::String thirdLFORateMode = "LFORMODE3";
    
        const juce::String firstLFORateController = "LFORCONTR1";
        const juce::String secondLFORateController = "LFORCONTR2";
        const juce::String thirdLFORateController = "LFORCONTR3";
    
        const juce::String firstLFOOn = "LFOON1";
        const juce::String firstLFORate = "LFORATE1";
        const juce::String secondLFOOn = "LFOON2";
        const juce::String secondLFORate = "LFORATE2";
        const juce::String thirdLFOOn = "LFOON3";
        const juce::String thirdLFORate = "LFORATE3";
    }
}
