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
    auto& waveshaper = processorChain.get<waveshaperIndex>();
    waveshaper.functionToUse = [](float x)
    {
        return std::tanh(x);
    };

    auto& preGain = processorChain.template get<preGainIndex>();
    preGain.setGainDecibels(30.0f);

    auto& postGain = processorChain.template get<postGainIndex>();
    postGain.setGainDecibels(-20.0f);
}
