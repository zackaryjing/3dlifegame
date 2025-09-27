#pragma once
#include <chrono>
#include <random>
using std::chrono::system_clock;


class Rand {
    Rand() = default;
    static inline auto dist = std::uniform_real_distribution(0.0f, 1.0f);
    static inline unsigned seed =
            system_clock::now().time_since_epoch().count();
    static inline auto generator = std::mt19937(seed);

public:
    static float gen_float() { return dist(generator); }
};
