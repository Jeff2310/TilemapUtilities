//
// Created by jeff2310 on 5/17/18.
//

#include "MapGenerator.h"

MapGenerator::MapGenerator() {
    map = new Map();
}

MapGenerator::MapGenerator(Coordinate height, Coordinate width):random_device(), random_engine(random_device()){
    map = new Map(height, width);
}

MapGenerator::~MapGenerator(){
    delete map;
}

void MapGenerator::random_fill(float possibility) {
    int height = map->getHeight();
    int width = map->getWidth();
    Noise noise(height > width? height: width);
    float rand;
    for(Coordinate y=0; y<height; y++){
        for(Coordinate x=0; x<width; x++){
            rand = noise.noiseValue((float)x, (float)y) * 0.5f + 0.5f;
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