//
// Created by jeff2310 on 5/8/18.
//

#include "Noise/PerlinNoise.h"
#include <math.h>
#include <random>

//const float eps = 0.000001f;

Vector lerp(const Vector &v1, const Vector &v2, float t){
    Vector result{v1.x + (v2.x-v1.x)*t, v1.y + (v2.y-v1.y)*t};
    return result;
}

float lerp(float x1, float x2, float t){
    return x1+(x2-x1)*t;
}

PerlinNoise::PerlinNoise(NoiseCoordinate size):Noise(size), randomCoordinate(0, size-1){
    RandomArrangement = new NoiseCoordinate[size];
    Gradients = new Vector[size];
    initRandomLists();
    _ease_curve = [](float x)->float{return 3*x*x-2*x*x*x;};
}

PerlinNoise::~PerlinNoise(){
    delete[] RandomArrangement;
    delete[] Gradients;
}

void PerlinNoise::initRandomLists(){
    for(int i=0; i<size; i++){
        RandomArrangement[i] = randomCoordinate(random_engine);
        Gradients[i].x = randomFloat(random_engine);
        Gradients[i].y = randomFloat(random_engine);
        Gradients[i].normalize();
    }
}

float PerlinNoise::noiseValue(float x, float y){
    NoiseCoordinate x1, y1, x2, y2;
    x1 = (NoiseCoordinate) floor(x), y1 = (NoiseCoordinate) floor(y);
    x2 = x1+1, y2 = y1+1;
    // 到所在方格四个端点的向量
    Vector dir11{x-(float)x1, y-(float)y1};
    Vector dir12{x-(float)x1, y-(float)y2};
    Vector dir21{x-(float)x2, y-(float)y1};
    Vector dir22{x-(float)x2, y-(float)y2};

    dir11.normalize();
    dir12.normalize();
    dir21.normalize();
    dir22.normalize();


    // 所在方格四个端点的梯度
    Vector &grad11 = Gradients[(x1 + RandomArrangement[y1])%size];
    Vector &grad12 = Gradients[(x1 + RandomArrangement[y2])%size];
    Vector &grad21 = Gradients[(x2 + RandomArrangement[y1])%size];
    Vector &grad22 = Gradients[(x2 + RandomArrangement[y2])%size];

    float value11 = dir11.dot(grad11);
    float value12 = dir12.dot(grad12);
    float value21 = dir21.dot(grad21);
    float value22 = dir22.dot(grad22);

    // 利用缓和曲线得到比例系数，进行线性插值
    float sx=_ease_curve(x-x1), sy=_ease_curve(y-y1);
    float a, b, result;
    a = lerp(value11, value21, sx);
    b = lerp(value12, value22, sx);
    result = lerp(a, b, sy);
    return result;
}