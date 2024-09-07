/*
  ==============================================================================

    FilterEditor.cpp
    Created: 7 Sep 2024 7:44:59pm
    Author:  Nemanja

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterEditor.h"

//==============================================================================
FilterEditor::FilterEditor(const juce::String& title, juce::AudioProcessorValueTreeState& vts, const juce::String& frequencyParamID, const juce::String& resonanceParamID, const juce::String& enabledParamID, const juce::String& typeParamID):
    vts(vts), enabledParamID(enabledParamID)
{
    addAndMakeVisible(filterTypeComboBox);
    filterTypeComboBox.addItemList({ "Lowpass", "Highpass", "Bandpass" }, 1);
    filterTypeComboBox.setSelectedId(1, juce::dontSendNotification);
    filterTypeAttachment.reset(new ComboBoxAttachment(vts, typeParamID, filterTypeComboBox));
    
    addAndMakeVisible(frequencySlider);
    frequencySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    frequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    frequencySliderAttachment.reset(new SliderAttachment(vts, frequencyParamID, frequencySlider));
    addAndMakeVisible(frequencyLabel);
    frequencyLabel.attachToComponent(&frequencySlider, false);
    
    addAndMakeVisible(resonanceSlider);
    resonanceSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    resonanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    resonanceSliderAttachment.reset(new SliderAttachment(vts, resonanceParamID, resonanceSlider));
    addAndMakeVisible(resonanceLabel);
    resonanceLabel.attachToComponent(&resonanceSlider, false);

    addAndMakeVisible(titleLabel);
    titleLabel.setText(title, juce::dontSendNotification);

    addAndMakeVisible(enabledToggle);
    enabledToggle.setClickingTogglesState(true);
    enabledToggle.onClick = [this]()
        {
            this->vts.getParameter(this->enabledParamID)->setValueNotifyingHost(enabledToggle.getToggleState());
        };
    //vts.getParameter("test")->setValue TODO fix this
    //enabledToggleAttachment.reset(new ButtonAttachment(vts, enabledParamID, enabledToggle));
}

FilterEditor::~FilterEditor()
{
}

void FilterEditor::paint (juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void FilterEditor::resized()
{
    auto bounds = getLocalBounds();
    constexpr int titleHeight = 20;
    constexpr int checkboxWidth = 24;
    auto comboBoxHeight = bounds.getHeight() / 8;
    auto sliderWidth = bounds.getWidth() / 2;

    auto titleStripBounds = bounds.removeFromTop(titleHeight);
    enabledToggle.setBounds(titleStripBounds.removeFromLeft(checkboxWidth));
    titleLabel.setBounds(titleStripBounds);

    filterTypeComboBox.setBounds(bounds.removeFromTop(comboBoxHeight));
    bounds.removeFromTop(titleHeight); // for slider labels
    frequencySlider.setBounds(bounds.removeFromLeft(sliderWidth));
    resonanceSlider.setBounds(bounds);
}
