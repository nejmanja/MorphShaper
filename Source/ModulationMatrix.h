/*
  ==============================================================================

	ModulationMatrix.h
	Created: 21 Sep 2024 6:06:15pm
	Author:  Nemanja

  ==============================================================================
*/

#pragma once
#include <array>
#include <atomic>

class ModulationMatrix
{
public:
	ModulationMatrix()
	{
		modulationMatrix[static_cast<int>(ModulationSource::LFO1)] = ModulationDestination::None;
		modulationMatrix[static_cast<int>(ModulationSource::LFO2)] = ModulationDestination::None;
	}

	// What can be the source of modulation? (just LFOs for now :D)
	enum class ModulationSource
	{
		LFO1,
		LFO2
	};
	// What can be modulated by the sources?
	enum class ModulationDestination
	{
		None, // the source isn't modulating anything
		WavetablePosition,
		PreFilterFrequency,
		PostFilterFrequency
	};

	void setModulationDestination(ModulationSource source, ModulationDestination destination)
	{
		// route source to new destination
		modulationMatrix[static_cast<int>(source)] = destination;
	}

	void assignModulationSource(ModulationSource source, std::atomic<float>* valuePtr)
	{
		modulationSourceValues[static_cast<int>(source)] = valuePtr;
	}

	void setModulationSource(ModulationSource source, float value)
	{
		*modulationSourceValues[static_cast<int>(source)] = value;
	}

	// returns a sum of all modulations being sent by all sources for given destination
	float getModulationValue(ModulationDestination destination)
	{
		float totalModulation = 0;
		for (int i = 0; i < MORPHSHAPER_NUM_SOURCES; i++)
		{
			if (modulationMatrix[i] == destination)
				totalModulation += *modulationSourceValues[i];
		}

		return totalModulation;
	}
private:
	static constexpr size_t MORPHSHAPER_NUM_SOURCES = 2;
	static constexpr size_t MORPHSHAPER_NUM_DESTINATIONS = 3;

	// for each source, which destination is it modulating
	std::array<ModulationDestination, MORPHSHAPER_NUM_SOURCES> modulationMatrix;
	std::array<std::atomic<float>*, MORPHSHAPER_NUM_SOURCES> modulationSourceValues{ 0 };
};