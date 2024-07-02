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
DistortionEditor::DistortionEditor(DistortionEngine& distortionEngine): distortionEngine(distortionEngine)
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
        this->distortionEngine.setPreGain(preGainSlider.getValue());
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
        this->distortionEngine.setPostGain(postGainSlider.getValue());
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
        this->distortionEngine.setModulationParameter(modulationSlider.getValue());
    };
    modulationSliderLabel.attachToComponent(&modulationSlider, false);
    modulationSliderLabel.setText("Wavetable Position", juce::dontSendNotification);
    modulationSliderLabel.setJustificationType(juce::Justification::centred);

}

DistortionEditor::~DistortionEditor()
{
}

void DistortionEditor::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
}

void DistortionEditor::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    auto sliderWidth = getWidth() / 3;
    preGainSlider.setBounds(0, 20, sliderWidth, sliderWidth + 50);
    postGainSlider.setBounds(sliderWidth, 20, sliderWidth, sliderWidth + 50);
    modulationSlider.setBounds(sliderWidth * 2.0f, 20, sliderWidth, sliderWidth + 50);
}

