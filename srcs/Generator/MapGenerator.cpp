//
// Created by jeff2310 on 5/17/18.
//

#include "Generator/MapGenerator.h"

MapGenerator::MapGenerator() : random_device(), random_engine(random_device()), random_float(0.0f, 1.0f) {
    map = new Map();
}

MapGenerator::MapGenerator(Coordinate height, Coordinate width)
        : random_device(), random_engine(random_device()), random_float(0.0f, 1.0f) {
    map = new Map(height, width);
}

MapGenerator::~MapGenerator(){
    delete map;
}

float MapGenerator::random() {
    return random_float(random_engine);
}

void MapGenerator::random_fill(float possibility) {
    int height = map->getHeight();
    int width = map->getWidth();
    float rand;
    for(Coordinate y=0; y<height; y++){
        for(Coordinate x=0; x<width; x++){
            rand = random() * 0.5f + 0.5f;
            if(rand < possibility)
                map->tile(x, y).setType(FLOOR);
            else
                map->tile(x, y).setType(WALL);
        }
    }
}

void MapGenerator::generateMap(Map *map){
    random_fill(0.5f);
    *map = *this->map;
}