#pragma once

namespace STR_CONST{

    const juce::String glideTime = "GLIDETIME";
    const juce::String globalVolume = "Global Volume";


    namespace ADSR{
        const juce::StringArray firstAdsrParameters = {
            "ATTACKTIME1",
            "DECAYTIME1",
            "SUSTAINLEVEL1",
            "RELEASETIME1"
        };
    
        
    
        const juce::String firstOscOn = "OSCON1";
        const juce::String firstOscGain = "OSC1 Gain";
        const juce::String firstOscWaveCoose = "WAVECHOOSEOSC1";
        const juce::String firstOscPan = "OSC1 Pan";
        const juce::String firstOscTranspose = "OSC1 Transpose";
    
        const juce::StringArray secondAdsrParameters = {
            "ATTACKTIME2",
            "DECAYTIME2",
            "SUSTAINLEVEL2",
            "RELEASETIME2"
        };
    
        const juce::String secondOscOn = "OSCON2";
        const juce::String secondOscGain = "OSC2 Gain";
        const juce::String secondOscWaveCoose = "WAVECHOOSEOSC2";
        const juce::String secondOscPan = "OSC2 Pan";
        const juce::String secondOscTranspose = "OSC2 Tarnspose";
        
        const juce::StringArray thirdAdsrParameters = {
            "ATTACKTIME3",
            "DECAYTIME3",
            "SUSTAINLEVEL3",
            "RELEASETIME3"
        };
    
        const juce::String thirdOscOn = "OSCON3";
        const juce::String thirdOscGain = "OSC3 Gain";
        const juce::String thirdOscWaveCoose = "WAVECHOOSEOSC3";
        const juce::String thirdOscPan = "OSC3 Pan";
        const juce::String thirdOscTranspose = "OSC3 Transpose";

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
    
        const juce::StringArray LFOBpmRates =
        {
            "1/64", "1/32",
            "1/16", "1/8",
            "1/4", "1/2",
            "1", "3/2",
            "2", "5/2",
            "3", "7/2",
            "4"
        };
    
        const juce::String firstLFOWorkMode = "LFOWMOD1";
        const juce::String secondLFOWorkMode = "LFOWMOD2";
        const juce::String thirdLFOWorkMode = "LFOWMOD3";
    
        const juce::String firstLFORateMode = "LFORMODE1";
        const juce::String secondLFORateMode = "LFORMODE2";
        const juce::String thirdLFORateMode = "LFORMODE3";
    
        const juce::String firstLFORateHZController = "LFOHZRCONTR1";
        const juce::String secondLFORateHZController = "LFOHZRCONTR2";
        const juce::String thirdLFORateHZController = "LFOHZRCONTR3";
    
        const juce::String firstLFORateBpmController = "LFOBPMRCONTR1";
        const juce::String secondLFORateBpmController = "LFOBPMRCONTR2";
        const juce::String thirdLFORateBpmController = "LFOBPMRCONTR3";
    
        const juce::String firstLFOOn = "LFOON1";
        const juce::String firstLFORate = "LFORATE1";
        const juce::String secondLFOOn = "LFOON2";
        const juce::String secondLFORate = "LFORATE2";
        const juce::String thirdLFOOn = "LFOON3";
        const juce::String thirdLFORate = "LFORATE3";
    }

    namespace EQ
    {
        const juce::String lowCutFreq = "LowCut Freq";
        const juce::String highCutFreq = "HighCut Freq";
        const juce::String peakFreq = "Peak Freq";
        const juce::String peakGain = "Peak Gain";
        const juce::String peakQuality = "Peak Q";
        const juce::String lowCutSlope = "LOWCUTSLOPE";
        const juce::String highCutSlope = "HIGHCUTSLOPE";
        const juce::String lowCutBypassed = "LOWCUTBYPASSED";
        const juce::String peakBypassed = "PEAKBYPASSED";
        const juce::String highCutBypassed = "HIGHCUTBYPASSED";
    }
}
