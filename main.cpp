#include "Noise/FactralNoise.h"
#include "Generator/CaveGenerator.h"
#include "Utility/PriorityQueue.h"
#include "Tilemap/PathFinder.h"
#include "Generator/DEPRECATED_DungeonGenerator.h"
#include "Generator/NeoGenerator.h"
#include <iostream>

int main() {
    NeoGenerator::Variables variable{};
    variable.roomSize_min = 10;
    variable.roomSize_max = 20;
    variable.attempts = 100;
    variable.corridorWidth = 1;
    variable.room_count = 100;

    NeoGenerator generator(200, 200, variable);
    Map map;
    generator.generateMap(&map);

    PathFinder astar;
    int x1= 28, y1 = 10, x2 = 51, y2 = 16;
    std::cin >> x1 >> y1 >> x2 >> y2;
    astar.FindPath(map, {x1, y1}, {x2, y2});

    map.dump();
}