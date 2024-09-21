/*
  ==============================================================================

	LFOEditor.cpp
	Created: 21 Sep 2024 4:21:58pm
	Author:  Nemanja

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LFOEditor.h"

//==============================================================================
LFOEditor::LFOEditor(juce::AudioProcessorValueTreeState& vts, ModulationMatrix& modulationMatrix, ModulationMatrix::ModulationSource modSource) :
	modulationMatrix(modulationMatrix), modSource(modSource)
{
	addAndMakeVisible(lfoTypeComboBox);
	lfoTypeComboBox.addItemList({ "Sine", "Triangle", "Saw", "Square" }, 1);
	lfoTypeComboBox.setSelectedId(1, juce::dontSendNotification);

	addAndMakeVisible(lfoTargetComboBox);
	lfoTargetComboBox.addItemList({ "Wavetable Position", "Pre-filter freq", "Post-filter freq" }, 1);
	lfoTargetComboBox.setSelectedId(1, juce::dontSendNotification);
	lfoTargetComboBox.onChange = [this]()
		{
			changeModulationDestination();
		};

	addAndMakeVisible(frequencySlider);
	frequencySlider.setSliderStyle(juce::Slider::LinearVertical);
	frequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
	
	addAndMakeVisible(frequencySliderLabel);
	frequencySliderLabel.setText("Frequency", juce::dontSendNotification);

	addAndMakeVisible(modulationIntensitySlider);
	modulationIntensitySlider.setSliderStyle(juce::Slider::LinearVertical);
	modulationIntensitySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
	
	addAndMakeVisible(modulationIntensitySliderLabel);
	modulationIntensitySliderLabel.setText("Intensity", juce::dontSendNotification);

	switch (modSource)
	{
	case ModulationMatrix::ModulationSource::LFO1:
		frequencySliderAttachment.reset(new SliderAttachment(vts, "lfo1Frequency", frequencySlider));
		modulationIntensitySliderAttachment.reset(new SliderAttachment(vts, "lfo1Intensity", modulationIntensitySlider));
		break;

	case ModulationMatrix::ModulationSource::LFO2:
		frequencySliderAttachment.reset(new SliderAttachment(vts, "lfo2Frequency", frequencySlider));
		modulationIntensitySliderAttachment.reset(new SliderAttachment(vts, "lfo2Intensity", modulationIntensitySlider));
		break;
	}
}

LFOEditor::~LFOEditor()
{
}

void LFOEditor::paint(juce::Graphics& g)
{
	juce::ignoreUnused(g);
}

void LFOEditor::resized()
{
	auto bounds = getLocalBounds();
	auto comboBoxHeight = bounds.getHeight() / 8;
	auto halfWidth = bounds.getWidth() / 2;

	lfoTypeComboBox.setBounds(bounds.removeFromTop(comboBoxHeight));
	lfoTargetComboBox.setBounds(bounds.removeFromTop(comboBoxHeight));


	auto freqSectionBounds = bounds.removeFromLeft(halfWidth);
	frequencySliderLabel.setBounds(freqSectionBounds.removeFromTop(20));
	frequencySlider.setBounds(freqSectionBounds);

	modulationIntensitySliderLabel.setBounds(bounds.removeFromTop(20));
	modulationIntensitySlider.setBounds(bounds);
}

void LFOEditor::changeModulationDestination()
{
	ModulationMatrix::ModulationDestination newDestination;
	bool valid = false;
	switch (lfoTargetComboBox.getSelectedId())
	{
	case 1:
		newDestination = ModulationMatrix::ModulationDestination::WavetablePosition;
		valid = true;
		break;
	case 2:
		newDestination = ModulationMatrix::ModulationDestination::PreFilterFrequency;
		valid = true;
		break;
	case 3:
		newDestination = ModulationMatrix::ModulationDestination::PostFilterFrequency;
		valid = true;
		break;
	default:
		valid = false;
		break;
	}

	if (valid)
		this->modulationMatrix.setModulationDestination(modSource, newDestination);
}
