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
    
        const juce::StringArray secondAdsrParameters = {
            "ATTACKTIME2",
            "DECAYTIME2",
            "SUSTAINLEVEL2",
            "RELEASETIME2"
        };
    
        const juce::String secondOscOn = "OSCON2";
        const juce::String secondOscGain = "GAINOSC2";
        const juce::String secondOscWaveCoose = "WAVECHOOSEOSC2";
        
        const juce::StringArray thirdAdsrParameters = {
            "ATTACKTIME3",
            "DECAYTIME3",
            "SUSTAINLEVEL3",
            "RELEASETIME3"
        };
    
        const juce::String thirdOscOn = "OSCON3";
        const juce::String thirdOscGain = "GAINOSC3";
        const juce::String thirdOscWaveCoose = "WAVECHOOSEOSC3";
        
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
}
