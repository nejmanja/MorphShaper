/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MorphShaperAudioProcessorEditor::MorphShaperAudioProcessorEditor (MorphShaperAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), distortionEditor(audioProcessor.getDistortionEngine())
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    addAndMakeVisible(distortionEditor);
    addAndMakeVisible(wavetableLibraryPicker);
}

MorphShaperAudioProcessorEditor::~MorphShaperAudioProcessorEditor()
{
}

//==============================================================================
void MorphShaperAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Morph Shaper", getLocalBounds().removeFromTop(20), juce::Justification::centred, 1);
}

void MorphShaperAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getBounds();
    bounds.removeFromTop(30); // for the header
    wavetableLibraryPicker.setBounds(bounds.removeFromTop(bounds.getHeight() / 4));
    distortionEditor.setBounds(bounds);
}
