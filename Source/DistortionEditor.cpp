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

    preGainSlider.setRange(-100, 30);
    preGainSlider.setValue(30);
    preGainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    preGainSlider.onValueChange = [this]()
    {
        this->distortionEngine.setPreGain(preGainSlider.getValue());
    };

    postGainSlider.setRange(-100, 30);
    postGainSlider.setValue(-20);
    postGainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    postGainSlider.onValueChange = [this]()
    {
        this->distortionEngine.setPostGain(postGainSlider.getValue());
    };
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
    auto halfWidth = getWidth() / 2;
    auto height = getHeight();
    preGainSlider.setBounds(0, 0, halfWidth, height);
    postGainSlider.setBounds(halfWidth, 0, halfWidth, height);
}
