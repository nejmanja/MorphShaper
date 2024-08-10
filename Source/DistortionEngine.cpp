/*
  ==============================================================================

    DistortionEngine.cpp
    Created: 23 Mar 2024 7:43:10pm
    Author:  Nemanja

  ==============================================================================
*/

#include "DistortionEngine.h"

DistortionEngine::DistortionEngine(std::atomic<float>* wavetablePositionParameter)
{
    this->modulationParameter = wavetablePositionParameter;

    auto& preGainProcessor = processorChain.template get<preGainIndex>();
    preGainProcessor.setGainDecibels(30.0f);

    auto& postGainProcessor = processorChain.template get<postGainIndex>();
    postGainProcessor.setGainDecibels(-20.0f);
}

void DistortionEngine::setPreGain(float gainValue)
{
    preGain = gainValue;
}

void DistortionEngine::setPostGain(float gainValue)
{
    postGain = gainValue;
}