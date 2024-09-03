/*
  ==============================================================================

	WavetableLibraryPicker.cpp
	Created: 2 Jul 2024 10:40:22pm
	Author:  Nemanja

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WavetableLibraryPicker.h"

//==============================================================================
WavetableLibraryPicker::WavetableLibraryPicker()
{
	addAndMakeVisible(wavetableLibraryLoadButton);
	addAndMakeVisible(wavetableLibraryFolderLabel);
	addAndMakeVisible(currentWavetableFileLabel);
	addAndMakeVisible(prevWavetableButton);
	addAndMakeVisible(nextWavetableButton);

	audioFormatManager.registerBasicFormats();

	currentWavetableFileIndex = 0;

	wavetableLibraryLoadButton.setButtonText("Choose wavetable library folder...");
	wavetableLibraryLoadButton.onClick = [this]()
		{
			chooseWavetableLibraryFolder();
		};

	prevWavetableButton.setButtonText("<");
	prevWavetableButton.onClick = [this]()
		{
			prevWavetable();
		};
	nextWavetableButton.setButtonText(">");
	nextWavetableButton.onClick = [this]()
		{
			nextWavetable();
		};
}

WavetableLibraryPicker::~WavetableLibraryPicker()
{
	delete[] wavetableBuffer;
}

void WavetableLibraryPicker::paint(juce::Graphics& g)
{
	float halfWidth = static_cast<float>(getWidth()) / 2.0f;

	if (wavetableBuffer != nullptr)
	{
		g.setColour(juce::Colours::aqua);
		for (int i = 0; i < 128; i++)
		{
			g.fillEllipse(static_cast<float>(i) / 128.0f * halfWidth + halfWidth, (wavetableBuffer[i * 4] * 0.5f + 0.5f) * getHeight(), 2, 2);
		}
	}

	g.setColour(getLookAndFeel().findColour(juce::Slider::rotarySliderFillColourId));
	g.drawRect(getLocalBounds().removeFromRight(static_cast<int>(halfWidth)), 2);
}

void WavetableLibraryPicker::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..
	auto bounds = getLocalBounds().removeFromLeft(getWidth() / 2);
	auto thirdHeight = bounds.getHeight() / 3;
	auto smallButtonWidth = bounds.getWidth() / 8;
	wavetableLibraryLoadButton.setBounds(bounds.removeFromTop(thirdHeight));
	wavetableLibraryFolderLabel.setBounds(bounds.removeFromTop(thirdHeight));
	prevWavetableButton.setBounds(bounds.removeFromLeft(smallButtonWidth));
	nextWavetableButton.setBounds(bounds.removeFromRight(smallButtonWidth));
	currentWavetableFileLabel.setBounds(bounds);
}

void WavetableLibraryPicker::chooseWavetableLibraryFolder()
{
	wavetableLibraryFolderChooser = std::make_unique<juce::FileChooser>("Select wavetable library dir...", juce::File::getSpecialLocation(juce::File::SpecialLocationType::commonDocumentsDirectory));

	auto folderChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;
	wavetableLibraryFolderChooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser)
		{
			wavetableLibraryFolder = chooser.getResult();
			wavetableFiles = wavetableLibraryFolder.findChildFiles(juce::File::TypesOfFileToFind::findFiles, false);
			wavetableLibraryFolderLabel.setText(wavetableLibraryFolder.getFullPathName(), juce::dontSendNotification);

			if (!wavetableFiles.isEmpty())
			{
				currentWavetableFileIndex = 0;
				currentWavetableFileLabel.setText(wavetableFiles[currentWavetableFileIndex].getFileName(), juce::dontSendNotification);
				loadWavetableFromFile();
			}
		});
}

void WavetableLibraryPicker::loadWavetableFromFile()
{
	auto* reader = audioFormatManager.createReaderFor(wavetableFiles[currentWavetableFileIndex]);
	if (reader != nullptr)
	{
		if (wavetableBuffer == nullptr)
			wavetableBuffer = new float[512];
		reader->read(&wavetableBuffer, 1, 0LL, juce::jmin(reader->lengthInSamples, 512LL));

		// The caller is responsible to free the reader
		delete reader;
		// TODO handle errors...
		repaint();
	}
	// TODO tell the user the file makes no sense otherwise!
}

void WavetableLibraryPicker::prevWavetable()
{
	if (!wavetableFiles.isEmpty())
	{
		currentWavetableFileIndex = (currentWavetableFileIndex - 1);
		if (currentWavetableFileIndex < 0) currentWavetableFileIndex = wavetableFiles.size() - 1;
		loadWavetableFromFile();
		currentWavetableFileLabel.setText(wavetableFiles[currentWavetableFileIndex].getFileName(), juce::dontSendNotification);
	}
}

void WavetableLibraryPicker::nextWavetable()
{
	if (!wavetableFiles.isEmpty())
	{
		currentWavetableFileIndex = (currentWavetableFileIndex + 1) % wavetableFiles.size();
		loadWavetableFromFile();
		currentWavetableFileLabel.setText(wavetableFiles[currentWavetableFileIndex].getFileName(), juce::dontSendNotification);
	}
}

