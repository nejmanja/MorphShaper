/*
  ==============================================================================

	DistortionEngine.cpp
	Created: 23 Mar 2024 7:43:10pm
	Author:  Nemanja

  ==============================================================================
*/

#include "DistortionEngine.h"

DistortionEngine::DistortionEngine(ModulationMatrix& modulationMatrix, std::atomic<float>* wavetablePositionParameter, std::atomic<float>* preGainParameter, std::atomic<float>* postGainParameter, FilterPluginParameters preFilterParams, FilterPluginParameters postFilterParams) :
	modulationParameter(wavetablePositionParameter), preGainParameter(preGainParameter), postGainParameter(postGainParameter), preFilterParams(preFilterParams), postFilterParams(postFilterParams), modulationMatrix(modulationMatrix)
{
	auto& waveshaperProcessor = processorChain.template get<waveshaperIndex>();
	waveshaperProcessor.setModulationParameter(*modulationParameter);

	auto& preGainProcessor = processorChain.template get<preGainIndex>();
	preGainProcessor.setGainDecibels(*preGainParameter);

	auto& postGainProcessor = processorChain.template get<postGainIndex>();
	postGainProcessor.setGainDecibels(*postGainParameter);

	auto& preFilterProcessor = processorChain.template get<preFilterIndex>();
	setFilterCutoff(preFilterProcessor, preFilterParams.getFrequency());
	setFilterResonance(preFilterProcessor, preFilterParams.getResonance());
	setFilterType(preFilterProcessor, preFilterParams.getType());

	processorChain.setBypassed<preFilterIndex>(!preFilterParams.getEnabled());

	auto& postFilterProcessor = processorChain.template get<postFilterIndex>();
	setFilterCutoff(postFilterProcessor, postFilterParams.getFrequency());
	setFilterResonance(postFilterProcessor, postFilterParams.getResonance());
	setFilterType(postFilterProcessor, postFilterParams.getType());
	processorChain.setBypassed<postFilterIndex>(!postFilterParams.getEnabled());
}
