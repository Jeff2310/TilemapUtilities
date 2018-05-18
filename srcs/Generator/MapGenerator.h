//
// Created by jeff2310 on 5/17/18.
//

#ifndef TILEGENERATOR_MAPGENERATOR_H
#define TILEGENERATOR_MAPGENERATOR_H

#include "../TileMap/Map.h"
#include "../Noise/Noise.h"
#include <random>

class MapGenerator {
protected:
    Map *map;
    std::random_device random_device;
    std::default_random_engine random_engine;
    void random_fill(float possibility);
public:
    MapGenerator();
    MapGenerator(Coordinate height, Coordinate width);
    virtual ~MapGenerator();

    virtual void generateMap(Map *map);
};

#endif //TILEGENERATOR_MAPGENERATOR_H
