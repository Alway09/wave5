#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Wave5AudioProcessor::Wave5AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),equalizer(&apvts), apvts(*this, nullptr, "Parameters", createParams())
                        /*modulationMatrix(&apvts)*/
#endif
{
    allRangedParametersIDs.add(juce::String("<add automation>"));
    allRangedParametersIDs.add(STR_CONST::ADSR::firstOscGain);
    allRangedParametersIDs.add(STR_CONST::ADSR::firstOscTranspose);
    allRangedParametersIDs.add(STR_CONST::ADSR::firstOscPan);
    allRangedParametersIDs.add(STR_CONST::ADSR::secondOscGain);
    allRangedParametersIDs.add(STR_CONST::ADSR::secondOscTranspose);
    allRangedParametersIDs.add(STR_CONST::ADSR::secondOscPan);
    allRangedParametersIDs.add(STR_CONST::ADSR::thirdOscGain);
    allRangedParametersIDs.add(STR_CONST::ADSR::thirdOscTranspose);
    allRangedParametersIDs.add(STR_CONST::ADSR::thirdOscPan);
    allRangedParametersIDs.add(STR_CONST::EQ::lowCutFreq);
    allRangedParametersIDs.add(STR_CONST::EQ::peakFreq);
    allRangedParametersIDs.add(STR_CONST::EQ::lowCutFreq);
    allRangedParametersIDs.add(STR_CONST::EQ::peakGain);
    allRangedParametersIDs.add(STR_CONST::EQ::peakQuality);
    
    modulationMatrixAtomic.store(new ModulationMatrixData(&apvts));
    
    //DBG("constructor");
    //DBG(allRangedParametersIDs.size());
    synth.addSound(new SynthSound());

    for(int i = 0; i < numberOfVoices; ++i){
        auto voice = new SynthVoice();
        //voice->getAdsr().attachADSRState(apvts, STR_CONST::ADSR::adsrStateId);
        //voice->getAdsr().setChangingStateAction(&apvts, STR_CONST::ADSR::adsrStateId);
        synth.addVoice(voice);
        //modulationMatrix.addVoice(voice, i);
    }
    
    LFOvector.push_back(LFOData("LFO 1"));
    LFOvector.push_back(LFOData("LFO 2"));
    LFOvector.push_back(LFOData("LFO 3"));
    
    LFOstates.push_back(false);
    LFOstates.push_back(false);
    LFOstates.push_back(false);
    
    //LFOvector[0].addPeriod(1, 2, 0.f, 0.5f, 0.f, 1.f);
    //LFOvector[0].addPeriod(2, 3, 0.5f, 1.f, 1.f, 0.f);
    
    //modulationMatrix.addModulatedParameter(STR_CONST::ADSR::firstOscGain, "LFO 1");
    
    globalGain.setRampDurationSeconds(0.0005);
}

Wave5AudioProcessor::~Wave5AudioProcessor()
{
    delete modulationMatrixAtomic.load();
}

//==============================================================================
const juce::String Wave5AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Wave5AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Wave5AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Wave5AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Wave5AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Wave5AudioProcessor::getNumPrograms()
{
    return 1;
}

int Wave5AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Wave5AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Wave5AudioProcessor::getProgramName (int index)
{
    return {};
}

void Wave5AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Wave5AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    equalizer.prepareToPlay(sampleRate, samplesPerBlock);

    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    globalGain.prepare(spec);
}

void Wave5AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Wave5AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Wave5AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());    
    
    // Update voice
    bool voicesIsActive = false;
    {
        for (int i = 0; i < synth.getNumVoices(); ++i)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                // Osc
                auto& firstOscGain = *apvts.getRawParameterValue(STR_CONST::ADSR::firstOscGain);
                auto& secondOscGain = *apvts.getRawParameterValue(STR_CONST::ADSR::secondOscGain);
                auto& thirdOscGain = *apvts.getRawParameterValue(STR_CONST::ADSR::thirdOscGain);
                
                juce::AudioParameterChoice* firstWaveTypeParam = static_cast<juce::AudioParameterChoice*>(apvts.getParameter(STR_CONST::ADSR::firstOscWaveCoose));
                
                juce::AudioParameterChoice* secondWaveTypeParam = static_cast<juce::AudioParameterChoice*>(apvts.getParameter(STR_CONST::ADSR::secondOscWaveCoose));
                
                juce::AudioParameterChoice* thirdWaveTypeParam = static_cast<juce::AudioParameterChoice*>(apvts.getParameter(STR_CONST::ADSR::thirdOscWaveCoose));
                
                auto& firstOscState = *apvts.getRawParameterValue(STR_CONST::ADSR::firstOscOn);
                auto& secondOscState = *apvts.getRawParameterValue(STR_CONST::ADSR::secondOscOn);
                auto& thirdOscState = *apvts.getRawParameterValue(STR_CONST::ADSR::thirdOscOn);
                
                auto& firstOscTranspose = *apvts.getRawParameterValue(STR_CONST::ADSR::firstOscTranspose);
                auto& secondOscTranspose = *apvts.getRawParameterValue(STR_CONST::ADSR::secondOscTranspose);
                auto& thirdOscTranspose = *apvts.getRawParameterValue(STR_CONST::ADSR::thirdOscTranspose);
                
                auto& firstOscPan = *apvts.getRawParameterValue(STR_CONST::ADSR::firstOscPan);
                auto& secondOscPan = *apvts.getRawParameterValue(STR_CONST::ADSR::secondOscPan);
                auto& thirdOscPan = *apvts.getRawParameterValue(STR_CONST::ADSR::thirdOscPan);
                
                auto& portamentoTime = *apvts.getRawParameterValue(STR_CONST::glideTime);
                
                voice->setPortamentoTime(portamentoTime.load());
                
                voice->getFirstOscillator().setWaveType(firstWaveTypeParam->getIndex());
                voice->setFirstOscState(firstOscState.load());
                voice->getFirstOscGain().setGainDecibels(firstOscGain.load());
                
                voice->setFirstTranspose(firstOscTranspose.load());
                voice->setSecondTranspose(secondOscTranspose.load());
                voice->setThirdTranspose(thirdOscTranspose.load());
                
                voice->getSecondOscillator().setWaveType(secondWaveTypeParam->getIndex());
                voice->setSecondOscState(secondOscState.load());
                voice->getSecondOscGain().setGainDecibels(secondOscGain.load());
                
                voice->getThirdOscillator().setWaveType(thirdWaveTypeParam->getIndex());
                voice->setThirdOscState(thirdOscState.load());
                voice->getThirdOscGain().setGainDecibels(thirdOscGain.load());
                
                voice->setFirstOscPan(firstOscPan.load());
                voice->setSecondOscPan(secondOscPan.load());
                voice->setThirdOscPan(thirdOscPan.load());
                
                // ADSR
                updateAdsr(voice->getFirstAdsr(), STR_CONST::ADSR::firstAdsrParameters);
                updateAdsr(voice->getSecondAdsr(), STR_CONST::ADSR::secondAdsrParameters);
                updateAdsr(voice->getThirdAdsr(), STR_CONST::ADSR::thirdAdsrParameters);
                
                // LFO
                voicesIsActive = voice->isVoiceActive() || voicesIsActive;
                
                modulationMatrixAtomic.load()->applyLFO(LFOvector, buffer.getNumSamples(), getSampleRate());
            }
            
        }
    }
    
    //DBG(midiMessages.getFirstEventTime());
    
    // LFO
    auto& firstLFOState = *apvts.getRawParameterValue(STR_CONST::LFO::firstLFOOn);
    auto& secondLFOState = *apvts.getRawParameterValue(STR_CONST::LFO::firstLFOOn);
    auto& thirdLFOState = *apvts.getRawParameterValue(STR_CONST::LFO::firstLFOOn);
    
    auto& firstLFOHzRate= *apvts.getRawParameterValue(STR_CONST::LFO::firstLFORateHZController);
    auto& secondLFOHzRate= *apvts.getRawParameterValue(STR_CONST::LFO::secondLFORateHZController);
    auto& thirdLFOHzRate= *apvts.getRawParameterValue(STR_CONST::LFO::thirdLFORateHZController);
    
    juce::AudioParameterChoice* firstLFOworkingMode = static_cast<juce::AudioParameterChoice*>(apvts.getParameter(STR_CONST::LFO::firstLFOWorkMode));
    juce::AudioParameterChoice* secondLFOworkingMode = static_cast<juce::AudioParameterChoice*>(apvts.getParameter(STR_CONST::LFO::secondLFOWorkMode));
    juce::AudioParameterChoice* thirdLFOworkingMode = static_cast<juce::AudioParameterChoice*>(apvts.getParameter(STR_CONST::LFO::thirdLFOWorkMode));
    
    juce::AudioParameterChoice* firstLFOrateMode = static_cast<juce::AudioParameterChoice*>(apvts.getParameter(STR_CONST::LFO::firstLFORateMode));
    juce::AudioParameterChoice* secondLFOrateMode = static_cast<juce::AudioParameterChoice*>(apvts.getParameter(STR_CONST::LFO::firstLFORateMode));
    juce::AudioParameterChoice* thirdLFOrateMode = static_cast<juce::AudioParameterChoice*>(apvts.getParameter(STR_CONST::LFO::firstLFORateMode));
    
    juce::AudioParameterChoice* firstLFOBPMRate = static_cast<juce::AudioParameterChoice*>(apvts.getParameter(STR_CONST::LFO::firstLFORateBpmController));
    juce::AudioParameterChoice* secondLFOBPMRate = static_cast<juce::AudioParameterChoice*>(apvts.getParameter(STR_CONST::LFO::secondLFORateBpmController));
    juce::AudioParameterChoice* thirdLFOBPMRate = static_cast<juce::AudioParameterChoice*>(apvts.getParameter(STR_CONST::LFO::thirdLFORateBpmController));
    
    //DBG(firstLFOrateMode->getIndex());
    
    LFOvector[0].setWoringMode(firstLFOworkingMode->getIndex());
    LFOvector[1].setWoringMode(secondLFOworkingMode->getIndex());
    LFOvector[2].setWoringMode(thirdLFOworkingMode->getIndex());
    
    LFOvector[0].setRateMode(firstLFOrateMode->getIndex());
    LFOvector[1].setRateMode(secondLFOrateMode->getIndex());
    LFOvector[2].setRateMode(thirdLFOrateMode->getIndex());
    
    LFOvector[0].setHzRate(firstLFOHzRate.load());
    LFOvector[1].setHzRate(secondLFOHzRate.load());
    LFOvector[2].setHzRate(thirdLFOHzRate.load());
    
    LFOvector[0].setBpmRate(firstLFOBPMRate->getIndex());
    LFOvector[1].setBpmRate(secondLFOBPMRate->getIndex());
    LFOvector[2].setBpmRate(thirdLFOBPMRate->getIndex());
    
    LFOstates[0] = firstLFOState.load();
    LFOstates[1] = secondLFOState.load();
    LFOstates[2] = thirdLFOState.load();
    
    updateLFO(voicesIsActive);

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
    equalizer.process(buffer);
    
    auto& globalGainVal = *apvts.getRawParameterValue(STR_CONST::globalVolume);
    
    globalGain.setGainDecibels(globalGainVal.load());
    juce::dsp::AudioBlock<float> audioBlock(buffer);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);
    globalGain.process(context);
}

void Wave5AudioProcessor::updateLFO(bool voiceIsActive){
    if(getPlayHead()){
        auto lfoIter = LFOvector.begin();
        
        int i = 0;
        while(lfoIter != LFOvector.end()){
            getPlayHead()->getCurrentPosition(lfoIter->getPlayHeadInfo());
            lfoIter->setEnable(LFOstates[i]);
            lfoIter->turnInState(voiceIsActive);
            
            ++i;
            ++lfoIter;
        }
    }
}

void Wave5AudioProcessor::updateAdsr(ModifiedAdsrData& adsr, const juce::StringArray& idList) {

    auto& attack = *apvts.getRawParameterValue(idList[0]);
    auto& decay = *apvts.getRawParameterValue(idList[1]);
    auto& sustain = *apvts.getRawParameterValue(idList[2]);
    auto& release = *apvts.getRawParameterValue(idList[3]);

    adsr.updateParameters(attack.load(), decay.load(), sustain.load(), release.load());
}

juce::AudioProcessorValueTreeState::ParameterLayout Wave5AudioProcessor::createParams() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    float attack_init = 0.f;
    float decay_init = 0.9f;
    float sustain_init = 1.f;
    float release_init = 0.1f;
    
    float attack_start = 0.f;
    float attack_end = 5.f;
    float decay_start = 0.f;
    float decay_end = 5.f;
    float release_start = 0.f;
    float release_end = 5.f;
    
    float sustain_start = 0.f;
    float sustain_end = 1.f;
    
    float adsr_skew = 0.3f;
    float adsr_step = 0.01f;
    
    int tr_min = -24;
    int tr_max = 24;
    
    float lfoHzStart = 0.01f;
    float lfoHzEnd = 60.f;
    float lfoHzInit = 1.f;
    float lfoHzStep = 0.01f;
    float lfoHzSkew = 1.f;
    
    /*float lfoBpmStart = 1.f / 64;
    float lfoBpmEnd = 4.f;*/

    // ---------ADSR---------
    // for OSC 1
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::ADSR::firstAdsrParameters[0],
        "Attack Time", 
        juce::NormalisableRange<float> { attack_start, attack_end, adsr_step, adsr_skew }, attack_init));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::ADSR::firstAdsrParameters[1],
        "Decay Time",
        juce::NormalisableRange<float> { decay_start, decay_end, adsr_step, adsr_skew }, decay_init));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::ADSR::firstAdsrParameters[2],
        "Sustain Level",
        juce::NormalisableRange<float> { sustain_start, sustain_end, adsr_step, adsr_skew }, sustain_init));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::ADSR::firstAdsrParameters[3],
        "Release Time",
        juce::NormalisableRange<float> { release_start, release_end, adsr_step, adsr_skew }, release_init));
    
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        STR_CONST::ADSR::firstOscOn,
        "OSC 1 On",
        true));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        STR_CONST::ADSR::firstOscWaveCoose,
        "OSC1 Wave",
        STR_CONST::ADSR::oscWavesVariants, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::ADSR::firstOscGain,
        "OSC 1 Gain",
        juce::NormalisableRange<float>(-48.0f, 6.0f, 0.1f, 0.3f), 0.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterInt>(
        STR_CONST::ADSR::firstOscTranspose,
        "OSC 1 Transpose", tr_min, tr_max, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::ADSR::firstOscPan,
        "Pan",
        juce::NormalisableRange<float> { -1.f, 1.f, 0.01, 1 }, 0));
    
    // for OSC 2
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::ADSR::secondAdsrParameters[0],
        "Attack Time",
        juce::NormalisableRange<float> { attack_start, attack_end, adsr_step, adsr_skew }, attack_init));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::ADSR::secondAdsrParameters[1],
        "Decay Time",
        juce::NormalisableRange<float> { decay_start, decay_end, adsr_step, adsr_skew }, decay_init));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::ADSR::secondAdsrParameters[2],
        "Sustain Level",
        juce::NormalisableRange<float> { sustain_start, sustain_end, adsr_step, adsr_skew }, sustain_init));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::ADSR::secondAdsrParameters[3],
        "Release Time",
        juce::NormalisableRange<float> { release_start, release_end, adsr_step, adsr_skew }, release_init));
    
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        STR_CONST::ADSR::secondOscOn,
        "OSC 2 On",
        false));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        STR_CONST::ADSR::secondOscWaveCoose,
        "OSC2 Wave",
        STR_CONST::ADSR::oscWavesVariants, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::ADSR::secondOscGain,
        "OSC 2 Gain",
        juce::NormalisableRange<float>(-48.0f, 6.0f, 0.1f, 0.3f), 0.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterInt>(
        STR_CONST::ADSR::secondOscTranspose,
        "OSC 2 Transpose", tr_min, tr_max, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::ADSR::secondOscPan,
        "Pan",
        juce::NormalisableRange<float> { -1.f, 1.f, 0.01, 1 }, 0));
    
    // for OSC 3
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::ADSR::thirdAdsrParameters[0],
        "Attack Time",
        juce::NormalisableRange<float> { attack_start, attack_end, adsr_step, adsr_skew }, attack_init));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::ADSR::thirdAdsrParameters[1],
        "Decay Time",
        juce::NormalisableRange<float> { decay_start, decay_end, adsr_step, adsr_skew }, decay_init));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::ADSR::thirdAdsrParameters[2],
        "Sustain Level",
        juce::NormalisableRange<float> { sustain_start, sustain_end, adsr_step, adsr_skew }, sustain_init));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::ADSR::thirdAdsrParameters[3],
        "Release Time",
        juce::NormalisableRange<float> { release_start, release_end, adsr_step, adsr_skew }, release_init));
    
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        STR_CONST::ADSR::thirdOscOn,
        "OSC 3 On",
        false));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        STR_CONST::ADSR::thirdOscWaveCoose,
        "OSC3 Wave",
        STR_CONST::ADSR::oscWavesVariants, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::ADSR::thirdOscGain,
        "OSC 3 Gain",
        juce::NormalisableRange<float>(-48.0f, 6.0f, 0.1f, 0.3f), 0.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterInt>(
        STR_CONST::ADSR::thirdOscTranspose,
        "OSC 3 Transpose", tr_min, tr_max, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::ADSR::thirdOscPan,
        "Pan",
        juce::NormalisableRange<float> { -1.f, 1.f, 0.01, 1 }, 0));
    
    // ---------LFO---------
    // for LFO 1
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        STR_CONST::LFO::firstLFOOn,
        "LFO 1 On",
        true));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        STR_CONST::LFO::firstLFOWorkMode,
        "LFO1 Work Mode",
        STR_CONST::LFO::LFOWorkModes, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        STR_CONST::LFO::firstLFORateMode,
        "LFO1 Rate Mode",
        STR_CONST::LFO::LFORateModes, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::LFO::firstLFORateHZController,
        "LFO1 Rate Hz",
        juce::NormalisableRange<float> { lfoHzStart, lfoHzEnd, lfoHzStep, lfoHzSkew }, lfoHzInit));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        STR_CONST::LFO::firstLFORateBpmController,
        "LFO1 Rate Bpm",
        STR_CONST::LFO::LFOBpmRates, 0));
    
    // for LFO 2
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        STR_CONST::LFO::secondLFOOn,
        "LFO 2 On",
        false));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        STR_CONST::LFO::secondLFOWorkMode,
        "LFO2 Work Mode",
        STR_CONST::LFO::LFOWorkModes, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        STR_CONST::LFO::secondLFORateMode,
        "LFO2 Rate Mode",
        STR_CONST::LFO::LFORateModes, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::LFO::secondLFORateHZController,
        "LFO2 Rate Hz",
        juce::NormalisableRange<float> { lfoHzStart, lfoHzEnd, lfoHzStep, lfoHzSkew }, lfoHzInit));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        STR_CONST::LFO::secondLFORateBpmController,
        "LFO2 Rate Bpm",
        STR_CONST::LFO::LFOBpmRates, 0));
    
    // for LFO 3
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        STR_CONST::LFO::thirdLFOOn,
        "LFO 3 On",
        false));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        STR_CONST::LFO::thirdLFOWorkMode,
        "LFO3 Work Mode",
        STR_CONST::LFO::LFOWorkModes, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        STR_CONST::LFO::thirdLFORateMode,
        "LFO3 Rate Mode",
        STR_CONST::LFO::LFORateModes, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::LFO::thirdLFORateHZController,
        "LFO3 Rate Hz",
        juce::NormalisableRange<float> { lfoHzStart, lfoHzEnd, lfoHzStep, lfoHzSkew }, lfoHzInit));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        STR_CONST::LFO::thirdLFORateBpmController,
        "LFO3 Rate Bpm",
        STR_CONST::LFO::LFOBpmRates, 0));
    
    // ---------EQUALIZER---------
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::EQ::lowCutFreq,
        "LowCut Freq",
        juce::NormalisableRange<float> { 20.f, 20000.f, 1.f, 0.25f }, 20.f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::EQ::highCutFreq,
        "HighCut Freq",
        juce::NormalisableRange<float> { 20.f, 20000.f, 1.f, 0.25f }, 20000.f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::EQ::peakFreq,
        "Peak Freq",
        juce::NormalisableRange<float> { 20.f, 20000.f, 1.f, 0.25f }, 750.f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::EQ::peakGain,
        "Peak Gain",
        juce::NormalisableRange<float> { -24.f, 24.f, 0.5f, 1.f }, 0.f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::EQ::peakQuality,
        "Peak Quality",
        juce::NormalisableRange<float> { 0.1f, 10.f, 0.05f, 0.25f }, 1.f));
    
    juce::StringArray strArr;
    for (int i = 0; i < 4; ++i) {
        juce::String str;
        str << (12 + i * 12);
        str << " db/Oct";
        strArr.add(str);
    }
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        STR_CONST::EQ::lowCutSlope,
        "LowCut Slope",
        strArr, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        STR_CONST::EQ::highCutSlope,
        "HighCut Slope",
        strArr, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        STR_CONST::EQ::lowCutBypassed,
        "LowCut Bypassed",
        false));
    
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        STR_CONST::EQ::highCutBypassed,
        "HighCut Bypassed",
        false));
    
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        STR_CONST::EQ::highCutBypassed,
        "HighCut Bypassed",
        false));
    
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        STR_CONST::EQ::peakBypassed,
        "Peak Bypassed",
        false));
    
    //------------------------------
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::glideTime,
        "Glide Time",
        juce::NormalisableRange<float> { 0.f, 10.f, 0.01f, 1.f }, 0.f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        STR_CONST::globalVolume,
        "Global Volume",
        juce::NormalisableRange<float> { -48.f, 6.f, 0.01f, 0.3f }, -6.f));
    
    return { params.begin(), params.end() };
}

//==============================================================================
bool Wave5AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Wave5AudioProcessor::createEditor()
{
    return new Wave5AudioProcessorEditor (*this);
}

//==============================================================================
void Wave5AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Wave5AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Wave5AudioProcessor();
}
