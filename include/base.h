#ifndef BASE_H
#define BASE_H

constexpr static float segmentRadius = 4.0F;
constexpr static float moveStep = segmentRadius * 2.0F;
constexpr static unsigned int gameSize = 20;

enum direction {
    none,
    left,
    right,
    up,
    down
};
#endif