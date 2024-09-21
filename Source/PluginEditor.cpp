/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MorphShaperAudioProcessorEditor::MorphShaperAudioProcessorEditor(MorphShaperAudioProcessor& p, juce::AudioProcessorValueTreeState& vts, ModulationMatrix& modulationMatrix)
	: AudioProcessorEditor(&p),
	audioProcessor(p),
	lfo1Editor(vts, modulationMatrix, ModulationMatrix::ModulationSource::LFO1),
	lfo2Editor(vts, modulationMatrix, ModulationMatrix::ModulationSource::LFO2),
	distortionEditor(audioProcessor.getDistortionEngine(), vts),
	wavetableLibraryPicker(audioProcessor.getDistortionEngine(), wavetableDrawer, vts.state.getChild(0)),
	valueTreeState(vts),
	preFilterEditor("Filter (pre)", vts, "preFilterCutoff", "preFilterResonance", "preFilterEnabled", "preFilterType"),
	postFilterEditor("Filter (post)", vts, "postFilterCutoff", "postFilterResonance", "postFilterEnabled", "postFilterType")
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(500, 350);

	addAndMakeVisible(distortionEditor);
	addAndMakeVisible(wavetableLibraryPicker);
	addAndMakeVisible(titleLabel);
	addAndMakeVisible(preFilterEditor);
	addAndMakeVisible(postFilterEditor);
	addAndMakeVisible(lfo1Editor);
	addAndMakeVisible(lfo2Editor);
	addAndMakeVisible(lfoSectionTitleLabel);
	addAndMakeVisible(wavetableSectionTitleLabel);
	addAndMakeVisible(distortionSectionTitleLabel);

	titleLabel.setText("MorphShaper", juce::dontSendNotification);
	titleLabel.setJustificationType(juce::Justification::centred);

	lfoSectionTitleLabel.setText("LFO", juce::dontSendNotification);
	lfoSectionTitleLabel.setFont(lfoSectionTitleLabel.getFont().boldened());

	wavetableSectionTitleLabel.setText("Wavetable", juce::dontSendNotification);
	wavetableSectionTitleLabel.setFont(wavetableSectionTitleLabel.getFont().boldened());

	distortionSectionTitleLabel.setText("Distortion", juce::dontSendNotification);
	distortionSectionTitleLabel.setFont(distortionSectionTitleLabel.getFont().boldened());

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

	int separatorY = (getHeight() / 2 + 3 + 20);
	g.setColour(getLookAndFeel().findColour(juce::Slider::thumbColourId).darker(0.9f));
	g.fillRect(0, 0, getWidth(), 20);
	g.fillRect(0, separatorY, getWidth(), getHeight());
}

void MorphShaperAudioProcessorEditor::resized()
{
	constexpr int titleHeight = 20;

	auto bounds = getBounds();
	auto halfWidth = getWidth() / 2;
	auto quarterWidth = halfWidth / 2;
	auto upperSectionHeight = getHeight() / 2;
	titleLabel.setBounds(bounds.removeFromTop(titleHeight));
	auto upperHalf = bounds.removeFromTop(upperSectionHeight);
	auto lfoSectionBounds = upperHalf.removeFromLeft(halfWidth);

	lfoSectionTitleLabel.setBounds(lfoSectionBounds.removeFromTop(titleHeight));
	lfo1Editor.setBounds(lfoSectionBounds.removeFromLeft(lfoSectionBounds.getWidth() / 2));
	lfo2Editor.setBounds(lfoSectionBounds);

	wavetableSectionTitleLabel.setBounds(upperHalf.removeFromTop(titleHeight));
	wavetableLibraryPicker.setBounds(upperHalf);

	bounds.removeFromTop(6); // some padding between sections
	preFilterEditor.setBounds(bounds.removeFromLeft(quarterWidth));
	postFilterEditor.setBounds(bounds.removeFromRight(quarterWidth));

	distortionSectionTitleLabel.setBounds(bounds.removeFromTop(titleHeight));
	distortionEditor.setBounds(bounds);
}

void MorphShaperAudioProcessorEditor::modulationParameterChanged(float newValue)
{
	currentWavetable = audioProcessor.getDistortionEngine().getCurrentWavetable(newValue);
	wavetableDrawer.onWavetableChanged(currentWavetable.data());
}
