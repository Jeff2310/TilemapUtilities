//
// Created by jeff2310 on 5/8/18.
//

#ifndef TILEGENERATOR_MAP_H
#define TILEGENERATOR_MAP_H

#include "TileMap/Tile.h"


typedef int Coordinate;
typedef int Distance;

static const Distance movements = 4;
static const Distance movement_dirs[4][2] = {{1, 0},{0, 1},{-1, 0}, {0, -1}};

class Map {
private:
    Tile *tiles;
    Coordinate width, height;
public:
    struct Point{
        Coordinate x, y;
        Point():x(-1),y(-1){}
        Point(Coordinate x, Coordinate y): x(x), y(y){}
        Point(const Point& p) = default;
    };

    Map();
    Map(const Map &m);
    Map(Coordinate height, Coordinate width);
    ~Map();
    Map& operator =(const Map &m);
    Tile& tile(Coordinate x, Coordinate y) const;
    inline Coordinate getHeight() const{ return height;}
    inline Coordinate getWidth() const{ return width;}
    inline void wrap(Coordinate *x, Coordinate *y) const{
        if(*x < 0) *x=0;
        if(*x >= width) *x=width-1;
        if(*y < 0) *y=0;
        if(*y >= height) *y=height-1;
    }
    void dump() const;
};


#endif //TILEGENERATOR_MAP_H
