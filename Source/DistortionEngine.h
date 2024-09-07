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
#include "FilterPluginParameters.h"

class DistortionEngine
{
public:
	DistortionEngine(std::atomic<float>* wavetablePositionParameter, std::atomic<float>* preGainParameter, std::atomic<float>* postGainParameter, FilterPluginParameters preFilterParams, FilterPluginParameters postFilterParams);
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
		preFilterIndex,
		waveshaperIndex,
		postFilterIndex,
		postGainIndex
	};
	juce::dsp::ProcessorChain
		<
			juce::dsp::Gain<float>,
			juce::dsp::StateVariableTPTFilter<float>,
			WavetableWaveshaper,
			juce::dsp::StateVariableTPTFilter<float>, 
			juce::dsp::Gain<float>
		>
		processorChain;

	void setFilterParams(juce::dsp::StateVariableTPTFilter<float>& filter, FilterPluginParameters& params);

	// This gets passed along from the constructor, it's unique state that exists on the plugin-level.
	std::atomic<float>* modulationParameter, * preGainParameter, * postGainParameter;
	FilterPluginParameters preFilterParams, postFilterParams;
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

	auto& preFilterProcessor = processorChain.template get<preFilterIndex>();
	setFilterParams(preFilterProcessor, preFilterParams);
	processorChain.setBypassed<preFilterIndex>(!preFilterParams.getEnabled());

	auto& postFilterProcessor = processorChain.template get<postFilterIndex>();
	setFilterParams(postFilterProcessor, postFilterParams);
	processorChain.setBypassed<postFilterIndex>(!postFilterParams.getEnabled());

	processorChain.process(context);
}
