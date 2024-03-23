/*
  ==============================================================================

    DistortionEngine.h
    Created: 23 Mar 2024 7:43:10pm
    Author:  Nemanja

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DistortionEngine
{
public:
    DistortionEngine();
    void prepare(const juce::dsp::ProcessSpec& spec) { processorChain.prepare(spec); };

    template <typename ProcessContext>
    void process(const ProcessContext& context) noexcept;

    void reset() noexcept { processorChain.reset(); };

    void setPreGain(float gainValue);
    void setPostGain(float gainValue);

private:
    enum
    {
        preGainIndex,
        waveshaperIndex,
        postGainIndex
    };
    juce::dsp::ProcessorChain<juce::dsp::Gain<float>, juce::dsp::WaveShaper<float>, juce::dsp::Gain<float>> processorChain;
    std::atomic<float> preGain, postGain;
};

template<typename ProcessContext>
inline void DistortionEngine::process(const ProcessContext& context) noexcept
{
    auto& preGain = processorChain.template get<preGainIndex>();
    preGain.setGainDecibels(this->preGain);

    auto& postGain = processorChain.template get<postGainIndex>();
    postGain.setGainDecibels(this->postGain);
    processorChain.process(context);
}
