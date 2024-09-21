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
LFOEditor::LFOEditor(juce::AudioProcessorValueTreeState& vts)
{
    addAndMakeVisible(lfoTypeComboBox);
    lfoTypeComboBox.addItemList({ "Sine", "Triangle", "Saw", "Square" }, 1);

    addAndMakeVisible(frequencySlider);
    frequencySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    frequencySlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 20);
    frequencySlider.setRange(1.0, 20.0);
    frequencySliderAttachment.reset(new SliderAttachment(vts, "lfoFrequency", frequencySlider));
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

    lfoTypeComboBox.setBounds(bounds.removeFromTop(halfHeight));
    frequencySlider.setBounds(bounds);
}
