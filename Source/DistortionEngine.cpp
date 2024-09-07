/*
  ==============================================================================

    DistortionEngine.cpp
    Created: 23 Mar 2024 7:43:10pm
    Author:  Nemanja

  ==============================================================================
*/

#include "DistortionEngine.h"

DistortionEngine::DistortionEngine(std::atomic<float>* wavetablePositionParameter, std::atomic<float>* preGainParameter, std::atomic<float>* postGainParameter)
{
    this->modulationParameter = wavetablePositionParameter;
    this->preGainParameter = preGainParameter;
    this->postGainParameter = postGainParameter;

    auto& waveshaperProcessor = processorChain.template get<waveshaperIndex>();
    waveshaperProcessor.setModulationParameter(*modulationParameter);

    auto& preGainProcessor = processorChain.template get<preGainIndex>();
    preGainProcessor.setGainDecibels(*preGainParameter);

    auto& postGainProcessor = processorChain.template get<postGainIndex>();
    postGainProcessor.setGainDecibels(*postGainParameter);
}