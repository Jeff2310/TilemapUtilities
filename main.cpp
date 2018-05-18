#include "srcs/Noise/FactralNoise.h"
#include "srcs/Generator/CaveGenerator.h"
#include "PriorityQueue.h"
#include "TileMap/PathFinder.h"
#include "srcs/Generator/DungeonGenerator.h"
#include <iostream>

int main() {
//    CaveGenerator generator(200, 200);
//    Map map;
//    generator.generateMap(&map, 50, 0.54f);
//    map.dump();

    DungeonGenerator::Variables variable{};
    variable.roomSize_min = 25; variable.roomSize_max = 90;
    variable.attempts = 500; variable.corridorWidth = 1;
    DungeonGenerator generator(1000, 1000, variable);
    Map map;
    generator.generateMap(&map);
//    map.dump();
    PathFinder astar;
    int x1= 28, y1 = 10, x2 = 51, y2 = 16;
//    std::cin>>x1>>y1>>x2>>y2;
    Noise noise(100);
    while(true){
        x1 = int((noise.noiseValue(x1, y1)*0.5f + 0.5f) * 1000);
        y1 = int((noise.noiseValue(x1, y2)*0.5f + 0.5f) * 1000);
        x2 = int((noise.noiseValue(x2, y1)*0.5f + 0.5f) * 1000);
        y2 = int((noise.noiseValue(x2, y2)*0.5f + 0.5f) * 1000);
        if(map.tile(x1, y1).getType()!=WALL && map.tile(x2, y2).getType()!=WALL)
            break;
    }
    astar.FindPath(map, {x1, y1}, {x2, y2});
    map.dump();
}