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
LFOEditor::LFOEditor(juce::AudioProcessorValueTreeState& vts, ModulationMatrix& modulationMatrix):
    modulationMatrix(modulationMatrix)
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
    frequencySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    frequencySlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 20);
    frequencySliderAttachment.reset(new SliderAttachment(vts, "lfo1Frequency", frequencySlider));

    addAndMakeVisible(modulationIntensitySlider);
    modulationIntensitySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    modulationIntensitySlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 20);
    modulationIntensitySliderAttachment.reset(new SliderAttachment(vts, "lfo1Intensity", modulationIntensitySlider));
}

LFOEditor::~LFOEditor()
{
}

void LFOEditor::paint (juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void LFOEditor::resized()
{
    auto bounds = getLocalBounds();
    auto halfHeight = bounds.getHeight() / 2;
    auto halfWidth = bounds.getWidth() / 2;

    auto leftHalf = bounds.removeFromLeft(halfWidth);
    lfoTypeComboBox.setBounds(leftHalf.removeFromTop(halfHeight));
    frequencySlider.setBounds(leftHalf);

    lfoTargetComboBox.setBounds(bounds.removeFromTop(halfHeight));
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
        this->modulationMatrix.setModulationDestination(ModulationMatrix::ModulationSource::LFO1, newDestination);
}
