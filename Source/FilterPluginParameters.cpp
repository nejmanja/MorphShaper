/*
  ==============================================================================

    FilterPluginParameters.cpp
    Created: 7 Sep 2024 8:53:16pm
    Author:  Nemanja

  ==============================================================================
*/

#include "FilterPluginParameters.h"

const juce::dsp::StateVariableTPTFilterType FilterPluginParameters::getType() const
{ 
    switch (static_cast<int>(*type))
    {
    case 0:
    default:
        return juce::dsp::StateVariableTPTFilterType::lowpass;
    case 1:
        return juce::dsp::StateVariableTPTFilterType::highpass;
    case 2:
        return juce::dsp::StateVariableTPTFilterType::bandpass;
    }
}
