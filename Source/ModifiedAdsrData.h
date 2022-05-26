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
                : attackTime(attackTimeSeconds),
                decayTime(decayTimeSeconds),
                sustainLevel(sustainLevel),
                releaseTime(releaseTimeSeconds)
            {
            }

            float attackTime = 1.0f, decayTime = 0.1f, sustainLevel = 0.5f, releaseTime = 0.1f;
        };
        
        //void attachADSRState(juce::AudioProcessorValueTreeState& apvts, const juce::String& choiseID);
     
        //void setParameters(const Parameters& newParameters);

        /** Returns the parameters currently being used by an ADSR object.
            @see setParameters
        */
        //const Parameters& getParameters() const noexcept { return parameters; }

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

        void updateParameters(float attack, float decay, float sustain, float release);

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
                buffer.applyGain(startSample, numSamples, parameters.sustainLevel);
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
        
        /*int getState(){
            int stateId = 0;
            switch (state) {
                case State::attack:
                    stateId = 1;
                    break;
                    
                default:
                    break;
            }
            
            return stateId;
        }*/

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
