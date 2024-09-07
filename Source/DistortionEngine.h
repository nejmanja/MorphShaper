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
	DistortionEngine(std::atomic<float>* wavetablePositionParameter, std::atomic<float>* preGainParameter, std::atomic<float>* postGainParameter);
	void prepare(const juce::dsp::ProcessSpec& spec) { processorChain.prepare(spec); };

	template <typename ProcessContext>
	void process(const ProcessContext& context) noexcept;

	void reset() noexcept { processorChain.reset(); };

	const std::array<float, MORPHSHAPER_WAVETABLE_RESOLUTION> getCurrentWavetable() { return processorChain.get<waveshaperIndex>().getCurrentWavetable(); }
	const std::array<float, MORPHSHAPER_WAVETABLE_RESOLUTION> getCurrentWavetable(float modulationParam) { return processorChain.get<waveshaperIndex>().getCurrentWavetable(modulationParam); }
	void setWavetable(std::vector<WavetableFunction> wavetableFunctions) { processorChain.get<waveshaperIndex>().setWavetable(wavetableFunctions); }
private:
	enum
	{
		preGainIndex,
		waveshaperIndex,
		postGainIndex
	};
	juce::dsp::ProcessorChain<juce::dsp::Gain<float>, WavetableWaveshaper, juce::dsp::Gain<float>> processorChain;
	// This gets passed along from the constructor, it's unique state that exists on the plugin-level.
	std::atomic<float>* modulationParameter, *preGainParameter, *postGainParameter;
};

template<typename ProcessContext>
inline void DistortionEngine::process(const ProcessContext& context) noexcept
{
	auto& waveshaperProcessor = processorChain.get<waveshaperIndex>();
	waveshaperProcessor.setModulationParameter(*modulationParameter);

	auto& preGainProcessor = processorChain.template get<preGainIndex>();
	preGainProcessor.setGainDecibels(*preGainParameter);

	auto& postGainProcessor = processorChain.template get<postGainIndex>();
	postGainProcessor.setGainDecibels(*postGainParameter);
	processorChain.process(context);
}
