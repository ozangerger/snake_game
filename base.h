#ifndef BASE_H
#define BASE_H
#include <random>
#include <chrono>

constexpr static float segmentRadius = 4.0F;
constexpr static float moveStep = segmentRadius * 2.0F;
constexpr static unsigned int gameSize = 10;
static std::mt19937 gen{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) };
std::uniform_int_distribution<> xDistrib{ 0U, gameSize - 1U };
std::uniform_int_distribution<> yDistrib{ 0U, gameSize - 1U };

enum direction {
    none,
    left,
    right,
    up,
    down
};
#endif