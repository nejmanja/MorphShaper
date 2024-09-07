/*
  ==============================================================================

	FilterPluginParameters.h
	Created: 7 Sep 2024 8:53:16pm
	Author:  Nemanja

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <atomic>


struct FilterPluginParameters
{
public:
	FilterPluginParameters(std::atomic<float>* frequency, std::atomic<float>* resonance, std::atomic<float>* enabled, std::atomic<float>* type) :
		frequency(frequency), resonance(resonance), enabled(enabled), type(type)
	{
	}

	const float getFrequency() const { return *frequency; }
	const float getResonance() const { return *resonance; }
	const bool getEnabled() const { return (*enabled) > 0; }
	const juce::dsp::StateVariableTPTFilterType getType() const;
private:
	std::atomic<float>* frequency;
	std::atomic<float>* resonance;
	std::atomic<float>* enabled;
	std::atomic<float>* type;
};