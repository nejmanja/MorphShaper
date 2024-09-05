/*
  ==============================================================================

	WavetableDrawer.cpp
	Created: 5 Sep 2024 9:52:23pm
	Author:  Nemanja

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WavetableDrawer.h"

//==============================================================================

WavetableDrawer::WavetableDrawer()
{
	wavetable = nullptr;
}

WavetableDrawer::~WavetableDrawer()
{
}

void WavetableDrawer::paint(juce::Graphics& g)
{
	if (!wavetable) return;

	g.setColour(getLookAndFeel().findColour(juce::Slider::thumbColourId));

	g.strokePath(wavetablePath, juce::PathStrokeType(2.0f));

	g.setColour(getLookAndFeel().findColour(juce::Slider::rotarySliderFillColourId));
	g.drawRect(getLocalBounds(), 2);
}

void WavetableDrawer::resized()
{
	generateWavetablePath(getLocalBounds());
}

void WavetableDrawer::onWavetableChanged(float* newWavetable)
{
	wavetable = newWavetable;
	auto bounds = getLocalBounds();

	generateWavetablePath(bounds);

	// Make sure to get exclusive rights before sending stuff to components!
	const juce::MessageManagerLock mmLock;
	repaint();
}

void WavetableDrawer::generateWavetablePath(juce::Rectangle<int> bounds)
{
	if (!wavetable) return;

	const int increment = 8;
	float widthScale = static_cast<float>(bounds.getWidth() - 2) / MORPHSHAPER_WAVETABLE_RESOLUTION;
	float heightScale = bounds.getHeight() / 2;

	wavetablePath.clear();
	wavetablePath.startNewSubPath(getPointInWavetable(0, bounds, widthScale, heightScale));
	for (int i = 1; i < MORPHSHAPER_WAVETABLE_RESOLUTION; i += increment)
	{
		wavetablePath.lineTo(getPointInWavetable(i, bounds, widthScale, heightScale));
	}
}


juce::Point<float> WavetableDrawer::getPointInWavetable(int sampleIdx, juce::Rectangle<int> bounds, float widthScale, float heightScale)
{
	return { sampleIdx * widthScale + bounds.getX() + 2,
		-(wavetable[sampleIdx] * (heightScale - 4)) + heightScale };
}