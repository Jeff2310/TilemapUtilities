//
// Created by jeff2310 on 5/10/18.
//

#ifndef TILEGENERATOR_CELLULARAUTOMATION_H
#define TILEGENERATOR_CELLULARAUTOMATION_H

#include "MapGenerator.h"

class CaveGenerator : MapGenerator{
private:
    Map *mapBuffer;
    void automation_iteration();
    void flood_fill();
public:
    CaveGenerator();
    CaveGenerator(Coordinate height, Coordinate width);
    ~CaveGenerator();
    void generateMap(Map *map, unsigned int iterations = 5, float init_poss = 0.5f);
};


#endif //TILEGENERATOR_CELLULARAUTOMATION_H
