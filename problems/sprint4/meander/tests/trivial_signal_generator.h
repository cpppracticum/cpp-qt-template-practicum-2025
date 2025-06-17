#pragma once
#include <algorithm>
#include <cstdint>
#include <vector>
#include <numbers>
#include "signal-processing.h"
#include <cmath>


inline void replicateSignal(std::vector<int16_t>& signal, const size_t desired_length) {
    const size_t wave_length = signal.size();
    while (signal.size() < desired_length) {
        const size_t max_insert = desired_length - signal.size();
        const size_t to_insert = std::min(wave_length, max_insert);
        std::copy_n(signal.begin(), to_insert, std::back_inserter(signal));
    }
}


inline std::vector<int16_t> generateSin(const SignalData& data, bool with_replication = true)
{
    std::vector<int16_t> signal;
    signal.reserve(data.samples);
    auto generator = [=, i = 0]() mutable {
        // Возвратим значение. Алгоритм generate_n добавит его
        // в вектор.
        return data.amplitude * std::sin(2 * i++ * std::numbers::pi / data.wave_length);
    };

    std::generate_n(std::back_inserter(signal), data.wave_length, generator);
    if (with_replication) {
        replicateSignal(signal, data.samples);
    }
    return signal;

}

inline std::vector<int16_t> generateCos(const SignalData& data, bool with_replication = true)
{
    std::vector<int16_t> signal;
    signal.reserve(data.samples);
    auto generator = [=, i = 0]() mutable {
        // Возвратим значение. Алгоритм generate_n добавит его
        // в вектор.
        return data.amplitude * std::cos(2 * i++ * std::numbers::pi / data.wave_length);
    };

    std::generate_n(std::back_inserter(signal), data.wave_length, generator);
    if (with_replication) {
        replicateSignal(signal, data.samples);
    }
    return signal;
}

inline std::vector<int16_t> generateMeander(const SignalData& data, bool with_replication = true)
{
    std::vector<int16_t> signal;
    signal.reserve(data.samples);
    const int ids = static_cast<int>(data.cycle * data.wave_length);
    std::fill_n(std::back_inserter(signal), ids, data.amplitude);
    std::fill_n(std::back_inserter(signal), data.wave_length - ids, -data.amplitude);
    if (with_replication) {
        replicateSignal(signal, data.samples);
    }
    return signal;
}

inline void applyDecay(std::vector<int16_t>& signal, size_t max_length) {
    const size_t decay_length = std::min(signal.size(), max_length);

    for(size_t i = decay_length; i > 0; --i) {
        int16_t& sample = signal[signal.size() - i];
        const double factor = static_cast<double>(i) / static_cast<double>(decay_length);
        sample = static_cast<int16_t>(sample * factor);
    }
}

inline void applyAttack(std::vector<int16_t>& signal, size_t max_length) {
    const size_t attack_length = std::min(signal.size(), max_length);

    for(size_t i = 0; i < attack_length; ++i) {
        int16_t& sample = signal[i];
        const double factor = static_cast<double>(i) / static_cast<double>(attack_length);
        sample = static_cast<int16_t>(sample * factor);
    }
}
