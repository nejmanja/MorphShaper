/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MorphShaperAudioProcessorEditor::MorphShaperAudioProcessorEditor (MorphShaperAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), distortionEditor(audioProcessor.getDistortionEngine(), vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    addAndMakeVisible(distortionEditor);
    addAndMakeVisible(wavetableLibraryPicker);
    addAndMakeVisible(titleLabel);
    titleLabel.setText("MorphShaper", juce::dontSendNotification);
    titleLabel.setJustificationType(juce::Justification::centred);
}

MorphShaperAudioProcessorEditor::~MorphShaperAudioProcessorEditor()
{
}

//==============================================================================
void MorphShaperAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MorphShaperAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getBounds();
    titleLabel.setBounds(bounds.removeFromTop(12));
    auto quarterHeight = bounds.getHeight() / 4;
    wavetableLibraryPicker.setBounds(bounds.removeFromTop(quarterHeight));
    distortionEditor.setBounds(bounds);
}
