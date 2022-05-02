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
                       ), apvts(*this, nullptr, "Parameters", createParams())
#endif
{
    synth.addSound(new SynthSound());

    for(int i = 0; i < numberOfVoices; ++i)
        synth.addVoice(new SynthVoice());
}

Wave5AudioProcessor::~Wave5AudioProcessor()
{
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
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
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

    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
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
    {
        for (int i = 0; i < synth.getNumVoices(); ++i)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                // Osc
                voice->getOscillator().setWaveType(0);
                // ADSR
                updateAdsr(voice->getAdsr());
                // Etc...
            }
            
        }
    }

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

void Wave5AudioProcessor::updateAdsr(ModifiedAdsrData& adsr) {
    using Params = ModifiedAdsrData::Parameters;

    auto& attack = *apvts.getRawParameterValue(Params::idList[Params::ParametersIDs::attackTimeSeconds]);
    auto& decay = *apvts.getRawParameterValue(Params::idList[Params::ParametersIDs::decayTimeSeconds]);
    auto& sustain = *apvts.getRawParameterValue(Params::idList[Params::ParametersIDs::sustainLevelNormalised]);
    auto& release = *apvts.getRawParameterValue(Params::idList[Params::ParametersIDs::releaseTimeSeconds]);

    adsr.updateParameters(attack.load(), decay.load(), sustain.load(), release.load());
}

juce::AudioProcessorValueTreeState::ParameterLayout Wave5AudioProcessor::createParams() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // ADSR
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ModifiedAdsrData::Parameters::idList[ModifiedAdsrData::Parameters::ParametersIDs::attackTimeSeconds],
        "Attack Time", 
        juce::NormalisableRange<float> { 0.01f, 5.0f, 0.01f, 0.3f }, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ModifiedAdsrData::Parameters::idList[ModifiedAdsrData::Parameters::ParametersIDs::decayTimeSeconds],
        "Decay Time",
        juce::NormalisableRange<float> { 0.01f, 5.0f, 0.01f, 0.3f }, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ModifiedAdsrData::Parameters::idList[ModifiedAdsrData::Parameters::ParametersIDs::sustainLevelNormalised],
        "Sustain Level",
        juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f, 0.3f }, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ModifiedAdsrData::Parameters::idList[ModifiedAdsrData::Parameters::ParametersIDs::releaseTimeSeconds],
        "Release Time",
        juce::NormalisableRange<float> { 0.0f, 5.0f, 0.01f, 0.3f }, 0.0f));

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
