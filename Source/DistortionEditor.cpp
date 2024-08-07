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
DistortionEditor::DistortionEditor(DistortionEngine& distortionEngine) : distortionEngine(distortionEngine)
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.

	addAndMakeVisible(preGainSlider);
	addAndMakeVisible(postGainSlider);
	addAndMakeVisible(modulationSlider);
	addAndMakeVisible(preGainSliderLabel);
	addAndMakeVisible(postGainSliderLabel);
	addAndMakeVisible(modulationSliderLabel);

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

	modulationSlider.setRange(0, 1);
	modulationSlider.setValue(0);
	modulationSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
	modulationSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
	modulationSlider.onValueChange = [this]()
	{
		this->distortionEngine.setModulationParameter(static_cast<float>(modulationSlider.getValue()));
		this->distortionEngine.getCurrentWavetable(currentWaveform);
		this->repaint();
	};
	modulationSliderLabel.attachToComponent(&modulationSlider, false);
	modulationSliderLabel.setText("Wavetable Position", juce::dontSendNotification);
	modulationSliderLabel.setJustificationType(juce::Justification::centred);

	distortionEngine.getCurrentWavetable(currentWaveform);
}

DistortionEditor::~DistortionEditor()
{
}

void DistortionEditor::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

	g.setColour(juce::Colours::red);
	const int increment = 8;
	juce::Path path;

	auto bounds = getBounds();
	float widthScale = static_cast<float>(bounds.getWidth()) / MORPHSHAPER_WAVETABLE_RESOLUTION;
	float heightScale = bounds.getHeight() / 2.0f;
	for (int i = 0; i < MORPHSHAPER_WAVETABLE_RESOLUTION - increment; i += increment)
	{
		path.addLineSegment(
			juce::Line<float>(
				i * widthScale,
				-(currentWaveform[i] * heightScale) + heightScale,
				(i + increment) * widthScale,
				-(currentWaveform[i + increment] * heightScale) + heightScale),
			1.0f);
	}
	g.strokePath(path, juce::PathStrokeType(2.0f));
}

void DistortionEditor::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..
	auto sliderWidth = getWidth() / 3;
	preGainSlider.setBounds(0, 20, sliderWidth, sliderWidth + 50);
	postGainSlider.setBounds(sliderWidth, 20, sliderWidth, sliderWidth + 50);
	modulationSlider.setBounds(sliderWidth * 2, 20, sliderWidth, sliderWidth + 50);
}

