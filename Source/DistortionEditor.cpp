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
	: distortionEngine(distortionEngine)
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

	modulationSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
	modulationSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
	modulationSliderAttachment.reset(new SliderAttachment(vts, "wtPosition", modulationSlider));

	modulationSliderLabel.attachToComponent(&modulationSlider, false);
	modulationSliderLabel.setText("Wavetable Position", juce::dontSendNotification);
	modulationSliderLabel.setJustificationType(juce::Justification::centred);
}

DistortionEditor::~DistortionEditor()
{
	
}

void DistortionEditor::paint(juce::Graphics& g)
{
	juce::ignoreUnused(g);
}

void DistortionEditor::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..
	auto bounds = getLocalBounds();
	auto quarterHeight = bounds.getHeight() / 4;
	auto halfWidth = bounds.getWidth() / 2;
	auto sliderWidth = getWidth() / 3;
	bounds.removeFromTop(20); // for the slider text
	preGainSlider.setBounds(bounds.removeFromLeft(sliderWidth));
	postGainSlider.setBounds(bounds.removeFromLeft(sliderWidth));
	modulationSlider.setBounds(bounds);
}


