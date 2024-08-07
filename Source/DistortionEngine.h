/*
  ==============================================================================

    DistortionEngine.h
    Created: 23 Mar 2024 7:43:10pm
    Author:  Nemanja

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "WavetableWaveshaper.h"

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
    void setModulationParameter(float modulationValue);

    void getCurrentWavetable(float* buff) { processorChain.get<waveshaperIndex>().getCurrentWavetable(buff); }

private:
    enum
    {
        preGainIndex,
        waveshaperIndex,
        postGainIndex
    };
    juce::dsp::ProcessorChain<juce::dsp::Gain<float>, WavetableWaveshaper, juce::dsp::Gain<float>> processorChain;
    std::atomic<float> preGain, postGain, modulationParameter;
};

template<typename ProcessContext>
inline void DistortionEngine::process(const ProcessContext& context) noexcept
{
    auto& waveshaperProcessor = processorChain.get<waveshaperIndex>();
    waveshaperProcessor.setModulationParameter(modulationParameter);

    auto& preGainProcessor = processorChain.template get<preGainIndex>();
    preGainProcessor.setGainDecibels(this->preGain);

    auto& postGainProcessor = processorChain.template get<postGainIndex>();
    postGainProcessor.setGainDecibels(this->postGain);
    processorChain.process(context);
}
