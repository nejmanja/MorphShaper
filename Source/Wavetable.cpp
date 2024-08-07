/*
  ==============================================================================

	Wavetable.cpp
	Created: 7 Aug 2024 2:27:32pm
	Author:  Nemanja

  ==============================================================================
*/

#include "Wavetable.h"
#include <JuceHeader.h>
#include <algorithm>

WavetableFunction::WavetableFunction(const float* waveform)
{
	// Unsafe approach, but gets the job done
	memcpy(this->waveform, waveform, MORPHSHAPER_WAVETABLE_RESOLUTION * sizeof(float));
}

WavetableFunction::WavetableFunction(std::vector<float> waveform)
{
	// TODO throw something here or log some sort of error
	if (waveform.size() != MORPHSHAPER_WAVETABLE_RESOLUTION) return;

	std::copy(waveform.begin(), waveform.end(), this->waveform);
}

float WavetableFunction::get(float x)
{
	float actualInputValue = std::clamp((x + 1.0f) * MORPHSHAPER_WAVETABLE_RESOLUTION / 2.0f, 0.0f, MORPHSHAPER_WAVETABLE_RESOLUTION - 1.0f);

	int inputValueLow = static_cast<int>(actualInputValue);
	if (inputValueLow == MORPHSHAPER_WAVETABLE_RESOLUTION - 1)
	{
		return waveform[inputValueLow];
	}
	else
	{
		int inputValueHigh = inputValueLow + 1;

		float lerpFactor = std::min(actualInputValue - inputValueLow, 1.0f);

		return waveform[inputValueLow] * (1.0f - lerpFactor) + waveform[inputValueHigh] * lerpFactor;
	}
}

const float* const WavetableFunction::get()
{
	return waveform;
}

Wavetable::Wavetable()
{
	float linearBuff[MORPHSHAPER_WAVETABLE_RESOLUTION];
	float tanhBuff[MORPHSHAPER_WAVETABLE_RESOLUTION];
	for (int i = 0; i < MORPHSHAPER_WAVETABLE_RESOLUTION; i++) {
		linearBuff[i] = juce::jmap((float)i, 0.0f, (float)MORPHSHAPER_WAVETABLE_RESOLUTION, -1.0f, 1.0f);
	}

	for (int i = 0; i < MORPHSHAPER_WAVETABLE_RESOLUTION; i++) {
		tanhBuff[i] = std::cbrtf(linearBuff[i]);
	}

	wavetable.push_back(WavetableFunction(linearBuff));
	wavetable.push_back(WavetableFunction(tanhBuff));
}

const float Wavetable::get(float x, float t)
{
	// TODO cache lerped values somewhere - the value of t will change relatively sparsely compared to the sample rate
	int maxIdx = wavetable.size() - 1;

	float actualInputValue = std::clamp(t, 0.0f, 1.0f) * maxIdx;
	int inputValueLow = static_cast<int>(actualInputValue);
	if (inputValueLow == maxIdx)
	{
		return wavetable[inputValueLow].get(x);
	}
	else
	{
		int inputValueHigh = inputValueLow + 1;

		float lerpFactor = std::min(actualInputValue - inputValueLow, 1.0f);

		return wavetable[inputValueLow].get(x) * (1.0f - lerpFactor) + wavetable[inputValueHigh].get(x) * lerpFactor;
	}
}

void Wavetable::get(float t, float* buff)
{
	// TODO cache lerped values somewhere - the value of t will change relatively sparsely compared to the sample rate
	// this would allow to just return that instead of calculating lerp'd values here
	int maxIdx = wavetable.size() - 1;

	float actualInputValue = std::clamp(t, 0.0f, 1.0f) * maxIdx;
	int inputValueLow = static_cast<int>(actualInputValue);
	if (inputValueLow == maxIdx)
	{
		memcpy(buff, wavetable[inputValueLow].get(), MORPHSHAPER_WAVETABLE_RESOLUTION);
	}
	else
	{
		int inputValueHigh = inputValueLow + 1;

		float lerpFactor = std::min(actualInputValue - inputValueLow, 1.0f);

		auto* wavetableLow = wavetable[inputValueLow].get();
		auto* wavetableHigh = wavetable[inputValueHigh].get();
		for (int i = 0; i < MORPHSHAPER_WAVETABLE_RESOLUTION; i++)
		{
			buff[i] = wavetableLow[i] * (1.0f - lerpFactor) + wavetableHigh[i] * lerpFactor;
		}
	}
}
