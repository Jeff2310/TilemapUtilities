//
// Created by jeff2310 on 5/17/18.
//

#ifndef TILEGENERATOR_DUNGEONGENERATOR_H
#define TILEGENERATOR_DUNGEONGENERATOR_H

#include "MapGenerator.h"
#include <vector>

using std::vector;

/*
 * (DEPRECATED)
 * 用生成树的方法生成地牢。生成树为最小生成树的时候效果可以，走廊之间不会交叉。
 * 但是如果生成树是普通生成树，那么不能保证在走廊不交叉的情况下，各个房间都联通。
 * 因为上述缘故生成的地牢走廊很短，所以不用这种方法了。
 */
class DEPRECATED_DungeonGenerator : MapGenerator {
public:
    typedef unsigned int Size;
    enum PathType{
        Corridor,
        Labyrinth
    };
    struct Path{
        PathType type;
        unsigned int length;
        int from, to;
        bool operator < (const Path &p) const{
            return length < p.length;
        }
        bool operator > (const Path &p) const{
            return length > p.length;
        }
        Path(unsigned int length, int from, int to, PathType type=Corridor): length(length), from(from), to(to), type(type) {}
    };
    struct Room{
        Coordinate x, y;
        Size height, width;
        vector<Map::Point> connectors;
        Room(Coordinate x, Coordinate y, Size height, Size width): x(x), y(y), height(height), width(width) {}
    };
    struct Variables{
        Size roomSize_min;
        Size roomSize_max;
        unsigned int attempts;
        Size corridorWidth;
        Variables(){
            roomSize_min = 10,
            roomSize_max = 80,
            attempts = 100,
            corridorWidth = 1;
        }
    };

    DEPRECATED_DungeonGenerator() = default;

    DEPRECATED_DungeonGenerator(Coordinate height, Coordinate width,
                                Variables variables = DEPRECATED_DungeonGenerator::Variables());

    ~DEPRECATED_DungeonGenerator() override;
    void generateMap(Map *map) override;
private:
    void spawnRoom();
    void connectRooms();
    bool __carveCorridor(Coordinate x1, Coordinate y1, Coordinate x2, Coordinate y2);
    bool carveCorridor(const Path &path);
    Variables _currentVariable;
    vector<Room> rooms;
    std::uniform_int_distribution<Coordinate> random_x, random_y;
    std::uniform_int_distribution<Size> random_size;
#define rand_x (random_x(random_engine))
#define rand_y (random_y(random_engine))
#define rand_size (random_size(random_engine))
};

#endif //TILEGENERATOR_DUNGEONGENERATOR_H
