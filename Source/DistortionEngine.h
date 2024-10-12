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
#include "ModulationMatrix.h"

class DistortionEngine
{
public:
	DistortionEngine(ModulationMatrix& modulationMatrix, std::atomic<float>* wavetablePositionParameter, std::atomic<float>* preGainParameter, std::atomic<float>* postGainParameter, FilterPluginParameters preFilterParams, FilterPluginParameters postFilterParams);
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

	void setFilterCutoff(juce::dsp::StateVariableTPTFilter<float>& filter, float value) { filter.setCutoffFrequency(value); }
	void setFilterResonance(juce::dsp::StateVariableTPTFilter<float>& filter, float value) { filter.setResonance(value); }
	void setFilterType(juce::dsp::StateVariableTPTFilter<float>& filter, juce::dsp::StateVariableTPTFilterType value) { filter.setType(value); }

	// This gets passed along from the constructor, it's unique state that exists on the plugin-level.
	std::atomic<float>* modulationParameter, * preGainParameter, * postGainParameter;
	FilterPluginParameters preFilterParams, postFilterParams;
	ModulationMatrix& modulationMatrix;
};

template<typename ProcessContext>
inline void DistortionEngine::process(const ProcessContext& context) noexcept
{
	auto& waveshaperProcessor = processorChain.get<waveshaperIndex>();
	auto wtPosModulationValue = modulationMatrix.getModulationValue(ModulationMatrix::ModulationDestination::WavetablePosition);
	waveshaperProcessor.setModulationParameter(juce::jlimit(0.0f, 1.0f, *modulationParameter + wtPosModulationValue));

	auto & preGainProcessor = processorChain.template get<preGainIndex>();
	preGainProcessor.setGainDecibels(*preGainParameter);

	auto & postGainProcessor = processorChain.template get<postGainIndex>();
	postGainProcessor.setGainDecibels(*postGainParameter);

	auto & preFilterProcessor = processorChain.template get<preFilterIndex>();
	auto preCutoffModulationValue = modulationMatrix.getModulationValue(ModulationMatrix::ModulationDestination::PreFilterFrequency);
	setFilterCutoff(preFilterProcessor, juce::jlimit(20.0f, 20000.0f, preFilterParams.getFrequency() + preCutoffModulationValue * 10000.0f));
	setFilterResonance(preFilterProcessor, preFilterParams.getResonance());
	setFilterType(preFilterProcessor, preFilterParams.getType());

	processorChain.setBypassed<preFilterIndex>(!preFilterParams.getEnabled());

	auto & postFilterProcessor = processorChain.template get<postFilterIndex>();
	auto postCutoffModulationValue = modulationMatrix.getModulationValue(ModulationMatrix::ModulationDestination::PostFilterFrequency);
	setFilterCutoff(postFilterProcessor, juce::jlimit(20.0f, 20000.0f, postFilterParams.getFrequency() + postCutoffModulationValue * 10000.0f));
	setFilterResonance(postFilterProcessor, postFilterParams.getResonance());
	setFilterType(postFilterProcessor, postFilterParams.getType());
	processorChain.setBypassed<postFilterIndex>(!postFilterParams.getEnabled());

	processorChain.process(context);
}
