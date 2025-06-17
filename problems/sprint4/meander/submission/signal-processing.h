#pragma once
#include <cstdint>
#include <vector>
#include <numbers>
#include <cassert>

enum class SignalType {
    kSin,
    kCos,
    kMeander
};

struct SignalData {
    SignalType signal_type; // Тип сигнала.
    int samples;            // Длина сигнала.
    int wave_length;        // Длина волны.
    int16_t amplitude;      // Максимальное значение.
    double cycle;           // Скважность меандра.
};

inline void ReplicateSignal(std::vector<int16_t>& signal, size_t desired_length) {
    assert(signal.size() > 0);
    // Код этой функции можете взять из урока.
}

inline std::vector<int16_t> GenerateSignal(SignalData data) {
    // Создайте сигнал, имеющий нужные параметры.
    // Формула синусоидального сигнала:
    // data.amplitude * sin(i * 2 * std::numbers::pi / data.wave_length).
    //
    // Формула косинусоидального сигнала:
    // data.amplitude * cos(i * 2 * std::numbers::pi / data.wave_length).
    //
    // Меандр создайте через std::fill_n.
    // Количество положительных отсчётов для меандра можно вычислть как
    // static_cast<int>(data.cycle * data.wave_length).
}

inline void ApplyDecayAndAttack(std::vector<int16_t>& signal,
                                size_t max_length) {
    // Примените затухание и атаку сигнала.
    // Используйте принцип, данный в уроке.
    // Затухание применяется к концу сигнала, а
    // атака симметрично - к началу.
    // Используйте алгоритм transform.
}
