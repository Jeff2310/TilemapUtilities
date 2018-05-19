//
// Created by jeff2310 on 5/8/18.
//

#ifndef TILEGENERATOR_TILE_H
#define TILEGENERATOR_TILE_H

enum FloorType{

};

enum WallType{

};

enum TileType{
    FLOOR,
    WALL,
    PATH,
    QUEUED,
    CORRIDOR
};

class Tile {
private:
    TileType type;
public:
    friend class Map;
    friend class PathFinder;
    TileType getType();
    void setType(TileType type);
};


#endif //TILEGENERATOR_TILE_H
