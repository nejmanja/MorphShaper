/*
  ==============================================================================

	Wavetable.h
	Created: 7 Aug 2024 2:27:32pm
	Author:  Nemanja

  ==============================================================================
*/

#pragma once
#define MORPHSHAPER_WAVETABLE_RESOLUTION 512

#include <vector>
#include "JuceHeader.h"

class WavetableFunction
{
public:
	WavetableFunction(const float* waveform);
	WavetableFunction(std::vector<float> waveform);

	// Get the value of the wavefom for a given input x. The input gets clamped between -1 and 1
	float get(float x);
	// Get the entire waveform
	const float* const get();
private:
	// buffer corresponding to a function that returns values for inputs between -1 and 1
	float waveform[MORPHSHAPER_WAVETABLE_RESOLUTION];
};

class Wavetable
{
public:
	// TODO delete
	// Default constructor, creates 2 frames, one being linear, other being tanh, for testing
	Wavetable();
	// Construct a wavetable from a set of wavetable functions, to be linearly interpolated between
	Wavetable(std::vector<WavetableFunction> wavetableBuffer);
	// Get the value of the wavetable function for a given input x and interpolation parameter t
	const float get(float x, float t);

	void set(std::vector<WavetableFunction> wavetableFunctions);
	// Get the whole wavetable for an interpolation parameter t
	const std::array<float, MORPHSHAPER_WAVETABLE_RESOLUTION> get(float t);
private:
	std::vector<WavetableFunction> wavetable;

	std::array<float, MORPHSHAPER_WAVETABLE_RESOLUTION> interpolatedWaveform;

	float previousParamValue = 0;
	void updateInterpolatedBuffer(float newValue);
};