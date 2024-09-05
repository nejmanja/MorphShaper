/*
  ==============================================================================

	WavetableDrawer.h
	Created: 5 Sep 2024 9:52:23pm
	Author:  Nemanja

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DistortionEngine.h"

//==============================================================================
/*
*/
class WavetableDrawer : public juce::Component
{
public:
	WavetableDrawer();
	~WavetableDrawer() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	void onWavetableChanged(float* newWavetable);

private:
	juce::Point<float> getPointInWavetable(int sampleIdx, juce::Rectangle<int> bounds, float widthScale, float heightScale);
	void generateWavetablePath(juce::Rectangle<int> bounds);

	// Read-only access for drawing
	const float* wavetable;
	juce::Path wavetablePath;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavetableDrawer)
};
