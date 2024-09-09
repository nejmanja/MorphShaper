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

	float width = static_cast<float>(getWidth());
	float height = static_cast<float>(getHeight());

	g.setColour(getLookAndFeel().findColour(juce::Slider::rotarySliderFillColourId));
	g.drawLine(width / 2, 0, width / 2, height, 1);
	g.drawLine(0, height / 2, width, height / 2, 1);


	g.setColour(getLookAndFeel().findColour(juce::Slider::thumbColourId));
	g.strokePath(wavetablePath, juce::PathStrokeType(2.0f));

	g.setColour(getLookAndFeel().findColour(juce::Slider::rotarySliderFillColourId));
	g.drawRect(getLocalBounds(), 2);
}

void WavetableDrawer::resized()
{
	generateWavetablePath(getLocalBounds());
}

void WavetableDrawer::onWavetableChanged(const float* newWavetable)
{
	wavetable = newWavetable;
	auto bounds = getLocalBounds();

	generateWavetablePath(bounds);

	repaint();
}

void WavetableDrawer::generateWavetablePath(juce::Rectangle<int> bounds)
{
	if (!wavetable) return;

	const int increment = 4;
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