#include "EqualizerDataHelperClasses.h"
#include "StringConstants.h"

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts) {
    ChainSettings settings;

    settings.lowCutFreq = apvts.getRawParameterValue(STR_CONST::EQ::lowCutFreq)->load();
    settings.highCutFreq = apvts.getRawParameterValue(STR_CONST::EQ::highCutFreq)->load();
    settings.peakFreq = apvts.getRawParameterValue(STR_CONST::EQ::peakFreq)->load();
    settings.peakGainInDecibels = apvts.getRawParameterValue(STR_CONST::EQ::peakGain)->load();
    settings.peakQuality = apvts.getRawParameterValue(STR_CONST::EQ::peakQuality)->load();
    settings.lowCutSlope = static_cast<Slope>(apvts.getRawParameterValue(STR_CONST::EQ::lowCutSlope)->load());
    settings.highCutSlope = static_cast<Slope>(apvts.getRawParameterValue(STR_CONST::EQ::highCutSlope)->load());

    settings.lowCutBypassed = apvts.getRawParameterValue(STR_CONST::EQ::lowCutBypassed)->load() < 0.5f;
    settings.peakBypassed = apvts.getRawParameterValue(STR_CONST::EQ::peakBypassed)->load() < 0.5f;
    settings.highCutBypassed = apvts.getRawParameterValue(STR_CONST::EQ::highCutBypassed)->load() < 0.5f;

    return settings;
}

Coefficients makePeakFilter(const ChainSettings& chainSettings, double sampleRate) {
    return juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate,
                                                               chainSettings.peakFreq,
                                                               chainSettings.peakQuality,
                                                               juce::Decibels::decibelsToGain(chainSettings.peakGainInDecibels));
}

void updateCoefficients(Coefficients& old, const Coefficients& replacement) {
    *old = *replacement;
}
