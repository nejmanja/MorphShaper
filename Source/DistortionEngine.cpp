/*
  ==============================================================================

    DistortionEngine.cpp
    Created: 23 Mar 2024 7:43:10pm
    Author:  Nemanja

  ==============================================================================
*/

#include "DistortionEngine.h"

DistortionEngine::DistortionEngine()
{
    auto& preGain = processorChain.template get<preGainIndex>();
    preGain.setGainDecibels(30.0f);

    auto& postGain = processorChain.template get<postGainIndex>();
    postGain.setGainDecibels(-20.0f);
}

void DistortionEngine::setPreGain(float gainValue)
{
    preGain = gainValue;
}

void DistortionEngine::setPostGain(float gainValue)
{
    postGain = gainValue;
}

void DistortionEngine::setModulationParameter(float modulationValue)
{
    this->modulationParameter = modulationValue;
}
