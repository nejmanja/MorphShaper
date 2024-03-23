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
    void process(const ProcessContext& context) noexcept { processorChain.process(context); }

    void reset() noexcept { processorChain.reset(); };
private:
    enum
    {
        preGainIndex,
        waveshaperIndex,
        postGainIndex
    };
    juce::dsp::ProcessorChain<juce::dsp::Gain<float>, juce::dsp::WaveShaper<float>, juce::dsp::Gain<float>> processorChain;
};