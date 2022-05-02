#pragma once
#include <JuceHeader.h>

    class  ModifiedAdsrData
    {
    public:
        //==============================================================================
        ModifiedAdsrData();

        //==============================================================================
        /**
            Holds the parameters being used by an ADSR object.
            @tags{Audio}
        */
        struct  Parameters
        {
            Parameters() = default;

            Parameters(float attackTimeSeconds,
                float decayTimeSeconds,
                float sustainLevel,
                float releaseTimeSeconds)
                : attack(attackTimeSeconds),
                decay(decayTimeSeconds),
                sustain(sustainLevel),
                release(releaseTimeSeconds)
            {
            }

            float attack = 0.1f, decay = 0.1f, sustain = 1.0f, release = 0.1f;
        };
     
        void setParameters(const Parameters& newParameters);

        /** Returns the parameters currently being used by an ADSR object.
            @see setParameters
        */
        const Parameters& getParameters() const noexcept { return parameters; }

        /** Returns true if the envelope is in its attack, decay, sustain or release stage. */
        bool isActive() const noexcept { return state != State::idle; }

        //==============================================================================
        /** Sets the sample rate that will be used for the envelope.
            This must be called before the getNextSample() or setParameters() methods.
        */
        void setSampleRate(double newSampleRate) noexcept
        {
            jassert(newSampleRate > 0.0);
            sampleRate = newSampleRate;
        }

        //==============================================================================
        /** Resets the envelope to an idle state. */
        void reset() noexcept
        {
            envelopeVal = 0.0f;
            state = State::idle;
        }

        void noteOn() noexcept;
        void noteOff() noexcept;

        //==============================================================================
        float getNextSample() noexcept;

        /** This method will conveniently apply the next numSamples number of envelope values
            to an AudioBuffer.
            @see getNextSample
        */
        template <typename FloatType>
        void applyEnvelopeToBuffer(juce::AudioBuffer<FloatType>& buffer, int startSample, int numSamples)
        {
            jassert(startSample + numSamples <= buffer.getNumSamples());

            if (state == State::idle)
            {
                buffer.clear(startSample, numSamples);
                return;
            }

            if (state == State::sustain)
            {
                buffer.applyGain(startSample, numSamples, parameters.sustain);
                return;
            }

            auto numChannels = buffer.getNumChannels();

            while (--numSamples >= 0)
            {
                auto env = getNextSample();

                for (int i = 0; i < numChannels; ++i)
                    buffer.getWritePointer(i)[startSample] *= env;

                ++startSample;
            }
        }

    private:
        //==============================================================================
        void recalculateRates() noexcept;
        void goToNextState() noexcept;

        //==============================================================================
        enum class State { idle, attack, decay, sustain, release };

        State state = State::idle;
        Parameters parameters;

        double sampleRate = 44100.0;
        float envelopeVal = 0.0f, attackRate = 0.0f, decayRate = 0.0f, releaseRate = 0.0f;
    };
