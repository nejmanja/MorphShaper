/*
  ==============================================================================

	WavetableWaveshaper.h
	Created: 30 Mar 2024 10:10:16pm
	Author:  Nemanja

	A class that works similarly to juce::dsp::WaveShaper, with much greater
	control with multiple parameters for the function applied to the buffer.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Wavetable.h"

class WavetableWaveshaper
{
public:
	WavetableWaveshaper()
	{
	}

	void reset() noexcept
	{
	}

	template <typename ProcessContext>
	void process(const ProcessContext& context) noexcept
	{
		if (context.isBypassed)
		{
			if (context.usesSeparateInputAndOutputBlocks())
				context.getOutputBlock().copyFrom(context.getInputBlock());
		}
		else
		{
			auto inBlock = context.getInputBlock();
			auto outBlock = context.getOutputBlock();

			auto len = inBlock.getNumSamples();
			auto numChans = inBlock.getNumChannels();

			jassert(len == outBlock.getNumSamples());
			jassert(numChans == outBlock.getNumChannels());
			jassert(numChans == 2); // TODO work around this assert!

			auto* srcL = inBlock.getChannelPointer(0);
			auto* dstL = outBlock.getChannelPointer(0);

			auto* srcR = inBlock.getChannelPointer(1);
			auto* dstR = outBlock.getChannelPointer(1);

			if (!juce::approximatelyEqual(modulationParameter, modulationParameterTarget))
			{
				auto increment = (modulationParameterTarget - modulationParameter) / len;
				for (size_t i = 0; i < len; ++i)
				{
					modulationParameter += increment;
					dstL[i] = waveshaperFunction(srcL[i], modulationParameter);
					dstR[i] = waveshaperFunction(srcR[i], modulationParameter);
				}
				modulationParameter = modulationParameterTarget;
			}
			else
			{
				for (size_t i = 0; i < len; ++i)
				{
					dstL[i] = waveshaperFunction(srcL[i], modulationParameter);
					dstR[i] = waveshaperFunction(srcR[i], modulationParameter);
				}
			}
		}
	}

	void setModulationParameter(float newValue)
	{
		modulationParameterTarget = newValue;
	}

	void prepare(const juce::dsp::ProcessSpec& spec)
	{
		juce::ignoreUnused(spec);
	}

	void getCurrentWavetable(float* buff) 
	{
		wavetable.get(modulationParameterTarget, buff);
	}
private:
	Wavetable wavetable;

	// a single modulation parameter, for prototyping purposes
	// TODO: replace with something like a lerp factor for going between two functions
	float modulationParameterTarget = 0.0, modulationParameter = 0.0;

	float waveshaperFunction(float inputSample, float modulationParam)
	{
		return wavetable.get(inputSample, modulationParam);
	}

	float waveshaperStartFunction(float inputSample)
	{
		return inputSample;
	}

	float waveshaperEndFunction(float inputSample)
	{
		return tanh(inputSample);
	}
};