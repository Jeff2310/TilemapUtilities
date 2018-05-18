//
// Created by jeff2310 on 5/10/18.
//

#ifndef TILEGENERATOR_NOISE_H
#define TILEGENERATOR_NOISE_H

#include <random>

typedef unsigned int NoiseCoordinate;

class Noise{
protected:
    NoiseCoordinate size;
    std::random_device random_device;
    std::default_random_engine random_engine;
    std::uniform_real_distribution<float> randomFloat;
    float rand();

    float *noises;
public:
    explicit Noise(unsigned int size);
    ~Noise();
    virtual float noiseValue(float x, float y);
};

#endif //TILEGENERATOR_NOISE_H
