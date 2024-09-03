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
WavetableLibraryPicker::WavetableLibraryPicker(DistortionEngine& distortionEngine)
	: distortionEngine(distortionEngine)
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
	juce::ignoreUnused(g);
}

void WavetableLibraryPicker::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..
	auto bounds = getLocalBounds();
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
			wavetableLibraryFolderLabel.setText(wavetableLibraryFolder.getFileName(), juce::dontSendNotification);

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
			wavetableBuffer = new float[MORPHSHAPER_WAVETABLE_RESOLUTION];

		int numFullFrames = juce::jmin(static_cast<int>(reader->lengthInSamples / MORPHSHAPER_WAVETABLE_RESOLUTION / 4), 32);
		std::vector<WavetableFunction> funcs;
		for (int i = 0; i < numFullFrames; i++)
		{
			// TODO - delete this! this is just temporary shenanigans to ensure compatibility
			// with serum wavetables for initial testing, which happen to be 2048 samples per frame
			float* tempBuffer = new float[MORPHSHAPER_WAVETABLE_RESOLUTION * 4];
			reader->read(&tempBuffer, 1, MORPHSHAPER_WAVETABLE_RESOLUTION * i * 4, MORPHSHAPER_WAVETABLE_RESOLUTION * 4);
			for (int j = 0; j < MORPHSHAPER_WAVETABLE_RESOLUTION; j++)
			{
				wavetableBuffer[j] = tempBuffer[j * 4];
			}
			delete[] tempBuffer;
			funcs.push_back(WavetableFunction(wavetableBuffer));
		}
		// The caller is responsible to free the reader
		delete reader;
		// TODO handle errors...

		distortionEngine.setWavetable(funcs);

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

