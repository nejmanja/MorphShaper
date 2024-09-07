/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MorphShaperAudioProcessorEditor::MorphShaperAudioProcessorEditor(MorphShaperAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
	: AudioProcessorEditor(&p),
	audioProcessor(p),
	distortionEditor(audioProcessor.getDistortionEngine(), vts),
	wavetableLibraryPicker(audioProcessor.getDistortionEngine(), vts.state.getChild(0)),
	valueTreeState(vts),
	preFilterEditor("Filter (pre)", vts, "preFilterCutoff", "preFilterResonance", "preFilterEnabled", "preFilterType"),
	postFilterEditor("Filter (post)", vts, "postFilterCutoff", "postFilterResonance", "postFilterEnabled", "postFilterType")
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(500, 300);

	addAndMakeVisible(distortionEditor);
	addAndMakeVisible(wavetableLibraryPicker);
	addAndMakeVisible(wavetableDrawer);
	addAndMakeVisible(titleLabel);
	addAndMakeVisible(preFilterEditor);
	addAndMakeVisible(postFilterEditor);

	titleLabel.setText("MorphShaper", juce::dontSendNotification);
	titleLabel.setJustificationType(juce::Justification::centred);

	auto* wtPostiionParam = vts.getParameter("wtPosition");
	modulationParameterChanged(wtPostiionParam->getValue());

	// This is the only way I could find to do this
	// The parameter attachment is required to do repainting in a thread-safe manner...
	modulationParamChangedCallback = std::bind(&MorphShaperAudioProcessorEditor::modulationParameterChanged, this, std::placeholders::_1);
	modulationParamAttachment.reset(new juce::ParameterAttachment(*wtPostiionParam, modulationParamChangedCallback));
}

MorphShaperAudioProcessorEditor::~MorphShaperAudioProcessorEditor()
{
}

//==============================================================================
void MorphShaperAudioProcessorEditor::paint(juce::Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MorphShaperAudioProcessorEditor::resized()
{
	auto bounds = getBounds();
	auto halfWidth = getWidth() / 2;
	auto quarterWidth = halfWidth / 2;
	auto upperSectionHeight = getHeight() / 3;
	titleLabel.setBounds(bounds.removeFromTop(12));
	auto upperHalf = bounds.removeFromTop(upperSectionHeight);
	wavetableDrawer.setBounds(upperHalf.removeFromRight(halfWidth));
	wavetableLibraryPicker.setBounds(upperHalf);
	
	preFilterEditor.setBounds(bounds.removeFromLeft(quarterWidth));
	postFilterEditor.setBounds(bounds.removeFromRight(quarterWidth));
	distortionEditor.setBounds(bounds);
}

void MorphShaperAudioProcessorEditor::modulationParameterChanged(float newValue)
{
	currentWavetable = audioProcessor.getDistortionEngine().getCurrentWavetable(newValue);
	wavetableDrawer.onWavetableChanged(currentWavetable.data());
}
