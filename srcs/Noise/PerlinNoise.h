//
// Created by jeff2310 on 5/8/18.
//

#ifndef TILEGENERATOR_PERLINNOISE_H
#define TILEGENERATOR_PERLINNOISE_H

#include "Noise.h"
#include <cmath>
#include <functional>
#include <iostream>

struct Vector {
    float x, y;

    float dot(const Vector v) const {
        return x * v.x + y * v.y;
    }

    void normalize() {
        if (x * x + y * y == 0) return;
        float length_inv = 1.0f / sqrtf(x * x + y * y);
        x *= length_inv, y *= length_inv;
    }
};

Vector lerp(const Vector &v1, const Vector &v2, float t);

float lerp(float x1, float x2, float t);

class PerlinNoise : Noise {
private:
    NoiseCoordinate *RandomArrangement;
    Vector *Gradients;

    void initRandomLists();

    std::function<float(float)> _ease_curve;
    std::uniform_int_distribution<NoiseCoordinate> randomCoordinate;
public:
    explicit PerlinNoise(NoiseCoordinate size);

    ~PerlinNoise();

    float noiseValue(float x, float y) override;
};

#endif //TILEGENERATOR_PERLINNOISE_H
