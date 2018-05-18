//
// Created by jeff2310 on 5/10/18.
//

#include "Noise.h"

Noise::Noise(unsigned int size) : size(size), random_device(), random_engine(random_device()),
                                  randomFloat(-1.0f, 1.0f) {
    noises = new float[size*size];
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            noises[i*size+j] = rand();
        }
    }
}

Noise::~Noise(){
    delete noises;
};

float Noise::rand(){
    return randomFloat(random_engine);
}

float Noise::noiseValue(float x, float y) {
    return noises[((int)y*size+(int)x)%size];
}