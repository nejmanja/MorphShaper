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
        modulationMatrix[static_cast<int>(ModulationDestination::WavetablePosition)] = ModulationSource::None;
        modulationMatrix[static_cast<int>(ModulationDestination::PreFilterFrequency)] = ModulationSource::None;
        modulationMatrix[static_cast<int>(ModulationDestination::PostFilterFrequency)] = ModulationSource::None;
    }

    // What can be the source of modulation? (just LFOs for now :D)
    enum class ModulationSource
    {
        None, // the destination is unmodulated
        LFO1,
        LFO2
    };
    // What can be modulated by the sources?
    enum class ModulationDestination
    {
        WavetablePosition,
        PreFilterFrequency,
        PostFilterFrequency
    };

    void setModulationDestination(ModulationSource source, ModulationDestination destination)
    {
        modulationMatrix[static_cast<int>(destination)] = source;
    }

    void assignModulationSource(ModulationSource source, std::atomic<float>* valuePtr)
    {
        modulationSourceValues[static_cast<int>(source)] = valuePtr;
    }

    void setModulationSource(ModulationSource source, float value)
    {
        *modulationSourceValues[static_cast<int>(source)] = value;
    }

    float getModulationValue(ModulationDestination destination)
    {
        auto source = modulationMatrix[static_cast<int>(destination)];
        if (source == ModulationSource::None) return 0;

        return *modulationSourceValues[static_cast<int>(source)];
    }
private:
    static constexpr size_t MORPHSHAPER_NUM_SOURCES = 2;
    static constexpr size_t MORPHSHAPER_NUM_DESTINATIONS = 3;

    // for each destination, which source is modulating it
    std::array<ModulationSource, MORPHSHAPER_NUM_DESTINATIONS> modulationMatrix;
    std::array<std::atomic<float>*, MORPHSHAPER_NUM_SOURCES> modulationSourceValues{ 0 };
};