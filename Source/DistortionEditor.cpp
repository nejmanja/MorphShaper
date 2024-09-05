/*
  ==============================================================================

	DistortionEditor.cpp
	Created: 23 Mar 2024 9:36:24pm
	Author:  Nemanja

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DistortionEditor.h"

//==============================================================================
DistortionEditor::DistortionEditor(DistortionEngine& distortionEngine, juce::AudioProcessorValueTreeState& vts) 
	: distortionEngine(distortionEngine), wavetableLibraryPicker(distortionEngine)
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.

	addAndMakeVisible(preGainSlider);
	addAndMakeVisible(postGainSlider);
	addAndMakeVisible(modulationSlider);
	addAndMakeVisible(preGainSliderLabel);
	addAndMakeVisible(postGainSliderLabel);
	addAndMakeVisible(modulationSliderLabel);
	addAndMakeVisible(wavetableLibraryPicker);

	preGainSlider.setRange(-100, 30);
	preGainSlider.setValue(30);
	preGainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
	preGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
	preGainSlider.onValueChange = [this]()
	{
		this->distortionEngine.setPreGain(static_cast<float>(preGainSlider.getValue()));
	};
	preGainSliderLabel.attachToComponent(&preGainSlider, false);
	preGainSliderLabel.setText("Pre Gain", juce::dontSendNotification);
	preGainSliderLabel.setJustificationType(juce::Justification::centred);

	postGainSlider.setRange(-100, 30);
	postGainSlider.setValue(-20);
	postGainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
	postGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
	postGainSlider.onValueChange = [this]()
	{
		this->distortionEngine.setPostGain(static_cast<float>(postGainSlider.getValue()));
	};
	postGainSliderLabel.attachToComponent(&postGainSlider, false);
	postGainSliderLabel.setText("Post Gain", juce::dontSendNotification);
	postGainSliderLabel.setJustificationType(juce::Justification::centred);

	modulationSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
	modulationSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
	modulationSliderAttachment.reset(new SliderAttachment(vts, "wtPosition", modulationSlider));

	modulationSliderLabel.attachToComponent(&modulationSlider, false);
	modulationSliderLabel.setText("Wavetable Position", juce::dontSendNotification);
	modulationSliderLabel.setJustificationType(juce::Justification::centred);

	distortionEngine.getCurrentWavetable(currentWaveform);

	vts.addParameterListener("wtPosition", this);
}

DistortionEditor::~DistortionEditor()
{
}

void DistortionEditor::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

	g.setColour(getLookAndFeel().findColour(juce::Slider::thumbColourId));

	auto bounds = getLocalBounds();
	g.strokePath(wavetablePath, juce::PathStrokeType(2.0f));

	g.setColour(getLookAndFeel().findColour(juce::Slider::rotarySliderFillColourId));
	g.drawRect(bounds.removeFromTop(getHeight() / 4).removeFromRight(getWidth() / 2), 2);
}

void DistortionEditor::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..
	auto bounds = getLocalBounds();
	auto quarterHeight = bounds.getHeight() / 4;
	auto halfWidth = bounds.getWidth() / 2;
	wavetableLibraryPicker.setBounds(bounds.removeFromTop(quarterHeight).removeFromLeft(halfWidth));
	auto sliderWidth = getWidth() / 3;
	bounds.removeFromTop(20); // for the slider text
	preGainSlider.setBounds(bounds.removeFromLeft(sliderWidth));
	postGainSlider.setBounds(bounds.removeFromLeft(sliderWidth));
	modulationSlider.setBounds(bounds);

	bounds = getLocalBounds();
	bounds = bounds.removeFromTop(quarterHeight).removeFromRight(halfWidth);
	generateWavetablePath(bounds);
}

void DistortionEditor::parameterChanged(const juce::String& parameterID, float newValue)
{
	this->distortionEngine.getCurrentWavetable(currentWaveform, newValue);

	auto bounds = getLocalBounds();
	auto quarterHeight = bounds.getHeight() / 4;
	auto halfWidth = bounds.getWidth() / 2;
	
	generateWavetablePath(bounds.removeFromTop(quarterHeight).removeFromRight(halfWidth));
	repaint();
}

juce::Point<float> DistortionEditor::getPointInWavetable(int sampleIdx, juce::Rectangle<int> bounds, float widthScale, float heightScale)
{
	return { sampleIdx * widthScale + bounds.getX() + 2,
		-(currentWaveform[sampleIdx] * (heightScale - 4)) + heightScale };
}

void DistortionEditor::generateWavetablePath(juce::Rectangle<int> bounds)
{
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

