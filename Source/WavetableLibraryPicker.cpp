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
WavetableLibraryPicker::WavetableLibraryPicker(DistortionEngine& distortionEngine, WavetableDrawer& drawer, juce::ValueTree libraryPathParam)
	: distortionEngine(distortionEngine), libraryPathParam(libraryPathParam), symmetricMode(true), ascendingMode(false), drawer(drawer)
{
	addAndMakeVisible(wavetableLibraryLoadButton);
	addAndMakeVisible(wavetableLibraryFolderLabel);
	addAndMakeVisible(currentWavetableFileLabel);
	addAndMakeVisible(prevWavetableButton);
	addAndMakeVisible(nextWavetableButton);
	addAndMakeVisible(symmetricModeButton);
	addAndMakeVisible(ascendingModeButton);
	addAndMakeVisible(symmetricModeLabel);
	addAndMakeVisible(ascendingModeLabel);
	addAndMakeVisible(drawer);

	audioFormatManager.registerBasicFormats();

	currentWavetableFileIndex = 0;

	wavetableLibraryLoadButton.setButtonText("...");
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

	symmetricModeButton.setClickingTogglesState(true);
	symmetricModeButton.setToggleState(symmetricMode, juce::dontSendNotification);
	symmetricModeButton.onClick = [this]()
		{
			symmetricMode = symmetricModeButton.getToggleState();
			// the ascending mode makes sense only when using symmetric
			ascendingModeButton.setEnabled(symmetricMode);
			ascendingModeLabel.setEnabled(symmetricMode);
			loadWavetableFromFile();
		};
	symmetricModeLabel.setText("Symmetric mode", juce::dontSendNotification);

	ascendingModeButton.setClickingTogglesState(true);
	ascendingModeButton.setToggleState(ascendingMode, juce::dontSendNotification);
	ascendingModeButton.onClick = [this]()
		{
			ascendingMode = ascendingModeButton.getToggleState();
			loadWavetableFromFile();
		};
	ascendingModeLabel.setText("Ascending mode", juce::dontSendNotification);

	updateWavetableLibraryFolder(libraryPathParam.getProperty("path").toString());
	loadWavetableFromFile(libraryPathParam.getProperty("fileName").toString());
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
	auto bounds = getLocalBounds();
	const int footerHeight = 20;
	const int checkboxWidth = 24;
	const int headerHeight = bounds.getHeight() / 5;
	const int thirdHeight = bounds.getHeight() / 3;
	const int smallButtonWidth = bounds.getWidth() / 8;

	auto togglesBounds = bounds.removeFromBottom(footerHeight);
	auto headerBounds = bounds.removeFromTop(headerHeight);

	prevWavetableButton.setBounds(headerBounds.removeFromLeft(smallButtonWidth));
	wavetableLibraryLoadButton.setBounds(headerBounds.removeFromRight(thirdHeight));
	headerBounds.removeFromRight(6); // some spacing
	nextWavetableButton.setBounds(headerBounds.removeFromRight(smallButtonWidth));
	currentWavetableFileLabel.setBounds(headerBounds.removeFromRight(headerBounds.getWidth() / 2));
	wavetableLibraryFolderLabel.setBounds(headerBounds);

	drawer.setBounds(bounds);

	auto symmetricModeBounds = togglesBounds.removeFromLeft(togglesBounds.getWidth() / 2);
	symmetricModeButton.setBounds(symmetricModeBounds.removeFromLeft(checkboxWidth));
	symmetricModeLabel.setBounds(symmetricModeBounds);

	ascendingModeButton.setBounds(togglesBounds.removeFromLeft(checkboxWidth));
	ascendingModeLabel.setBounds(togglesBounds);
}

void WavetableLibraryPicker::chooseWavetableLibraryFolder()
{
	wavetableLibraryFolderChooser = std::make_unique<juce::FileChooser>("Select wavetable library dir...", juce::File::getSpecialLocation(juce::File::SpecialLocationType::commonDocumentsDirectory));

	auto folderChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;
	wavetableLibraryFolderChooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser)
		{
			updateWavetableLibraryFolder(chooser.getResult());
			resetActiveWavetableFile();
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

			if (symmetricMode)
			{
				transformDataToSymmetric();
			}

			funcs.push_back(WavetableFunction(wavetableBuffer));
		}
		// The caller is responsible to free the reader
		delete reader;
		// TODO handle errors...

		distortionEngine.setWavetable(funcs);

		drawer.onWavetableChanged(distortionEngine.getCurrentWavetable().data());
	}
	// TODO tell the user the file makes no sense otherwise!
}

void WavetableLibraryPicker::transformDataToSymmetric()
{
	// fill the upper half of the buffer with "squished" signal
	for (int i = MORPHSHAPER_WAVETABLE_RESOLUTION - 1, j = i - 1; i >= MORPHSHAPER_WAVETABLE_RESOLUTION / 2; i--, j -= 2)
	{
		wavetableBuffer[i] = wavetableBuffer[j];
		if (ascendingMode) wavetableBuffer[i] = wavetableBuffer[i] * 0.5f + 0.5f;
	}

	// symmetrically copy over right side of the buffer
	for (int i = 0; i < MORPHSHAPER_WAVETABLE_RESOLUTION / 2; i++)
	{
		wavetableBuffer[i] = -wavetableBuffer[MORPHSHAPER_WAVETABLE_RESOLUTION - 1 - i];
	}

	// zero out the middle part
	wavetableBuffer[MORPHSHAPER_WAVETABLE_RESOLUTION / 2] = 0;
}

void WavetableLibraryPicker::loadWavetableFromFile(const juce::String& wtFile)
{
	for (int i = 0; i < wavetableFiles.size(); ++i)
	{
		if (!wavetableFiles[i].getFileName().compare(wtFile))
		{
			currentWavetableFileIndex = i;
			currentWavetableFileLabel.setText(wavetableFiles[currentWavetableFileIndex].getFileName(), juce::dontSendNotification);
			loadWavetableFromFile();
			return;
		}
	}
}

void WavetableLibraryPicker::updateWavetableLibraryFolder(juce::File newWtFolder)
{
	wavetableLibraryFolder = newWtFolder;
	wavetableFiles = wavetableLibraryFolder.findChildFiles(juce::File::TypesOfFileToFind::findFiles, false);
	wavetableLibraryFolderLabel.setText(wavetableLibraryFolder.getFileName(), juce::dontSendNotification);
}

void WavetableLibraryPicker::resetActiveWavetableFile()
{
	if (!wavetableFiles.isEmpty())
	{
		libraryPathParam.setProperty("path", wavetableLibraryFolder.getFullPathName(), nullptr);
		currentWavetableFileIndex = 0;
		libraryPathParam.setProperty("fileName", wavetableFiles[currentWavetableFileIndex].getFileName(), nullptr);
		currentWavetableFileLabel.setText(wavetableFiles[currentWavetableFileIndex].getFileName(), juce::dontSendNotification);
		loadWavetableFromFile();
	}
}

void WavetableLibraryPicker::prevWavetable()
{
	if (!wavetableFiles.isEmpty())
	{
		currentWavetableFileIndex = (currentWavetableFileIndex - 1);
		if (currentWavetableFileIndex < 0) currentWavetableFileIndex = wavetableFiles.size() - 1;
		loadWavetableFromFile();
		libraryPathParam.setProperty("fileName", wavetableFiles[currentWavetableFileIndex].getFileName(), nullptr);
		currentWavetableFileLabel.setText(wavetableFiles[currentWavetableFileIndex].getFileName(), juce::dontSendNotification);
	}
}

void WavetableLibraryPicker::nextWavetable()
{
	if (!wavetableFiles.isEmpty())
	{
		currentWavetableFileIndex = (currentWavetableFileIndex + 1) % wavetableFiles.size();
		loadWavetableFromFile();
		libraryPathParam.setProperty("fileName", wavetableFiles[currentWavetableFileIndex].getFileName(), nullptr);
		currentWavetableFileLabel.setText(wavetableFiles[currentWavetableFileIndex].getFileName(), juce::dontSendNotification);
	}
}

