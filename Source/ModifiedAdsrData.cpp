#include "ModifiedAdsrData.h"
const juce::StringArray ModifiedAdsrData::Parameters::idList = {
                "ATTACKTIME",
                "DECAYTIME",
                "SUSTAINLEVEL",
                "RELEASETIME"
};

ModifiedAdsrData::ModifiedAdsrData()
{

    recalculateRates();
}

void ModifiedAdsrData::updateParameters(float attack, float decay, float sustain, float release) {
    parameters.attackTime = attack;
    parameters.decayTime = decay;
    parameters.sustainLevel = sustain;
    parameters.releaseTime = release;

    recalculateRates();
}

/** Sets the parameters that will be used by an ADSR object.
            You must have called setSampleRate() with the correct sample rate before
            this otherwise the values may be incorrect!
            @see getParameters
*/
/*void ModifiedAdsrData::setParameters(const Parameters& newParameters)
{
    // need to call setSampleRate() first!
    jassert(sampleRate > 0.0);

    parameters = newParameters;
    recalculateRates();
}*/

void ModifiedAdsrData::recalculateRates() noexcept
{
    auto getRate = [](float distance, float timeInSeconds, double sr)
    {
        return timeInSeconds > 0.0f ? (float)(distance / (timeInSeconds * sr)) : -1.0f;
    };

    attackRate = getRate(1.0f, parameters.attackTime, sampleRate);
    decayRate = getRate(1.0f - parameters.sustainLevel, parameters.decayTime, sampleRate);
    releaseRate = getRate(parameters.sustainLevel, parameters.releaseTime, sampleRate);

    if ((state == State::attack && attackRate <= 0.0f)
        || (state == State::decay && (decayRate <= 0.0f || envelopeVal <= parameters.sustainLevel))
        || (state == State::release && releaseRate <= 0.0f))
    {
        goToNextState();
    }
}

void ModifiedAdsrData::goToNextState() noexcept
{
    if (state == State::attack)
        state = (decayRate > 0.0f ? State::decay : State::sustain);
    else if (state == State::decay)
        state = State::sustain;
    else if (state == State::release)
        reset();
}

/** Returns the next sample value for an ADSR object.
            @see applyEnvelopeToBuffer
*/
float ModifiedAdsrData::getNextSample() noexcept
{
    if (state == State::idle)
        return 0.0f;

    if (state == State::attack)
    {
        envelopeVal += attackRate;

        if (envelopeVal >= 1.0f)
        {
            envelopeVal = 1.0f;
            goToNextState();
        }
    }
    else if (state == State::decay)
    {
        envelopeVal -= decayRate;

        if (envelopeVal <= parameters.sustainLevel)
        {
            envelopeVal = parameters.sustainLevel;
            goToNextState();
        }
    }
    else if (state == State::sustain)
    {
        envelopeVal = parameters.sustainLevel;
    }
    else if (state == State::release)
    {
        envelopeVal -= releaseRate;

        if (envelopeVal <= 0.0f)
            goToNextState();
    }

    return envelopeVal;
}

/** Starts the attack phase of the envelope. */
void ModifiedAdsrData::noteOn() noexcept
{
    if (attackRate > 0.0f)
    {
        state = State::attack;
    }
    else if (decayRate > 0.0f)
    {
        envelopeVal = 1.0f;
        state = State::decay;
    }
    else
    {
        envelopeVal = parameters.sustainLevel;
        state = State::sustain;
    }
}

/** Starts the release phase of the envelope. */
void ModifiedAdsrData::noteOff() noexcept
{
    if (state != State::idle)
    {
        if (parameters.releaseTime > 0.0f)
        {
            releaseRate = (float)(envelopeVal / (parameters.releaseTime * sampleRate));
            state = State::release;
        }
        else
        {
            reset();
        }
    }
}