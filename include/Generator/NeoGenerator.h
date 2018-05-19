//
// Created by jeff2310 on 5/17/18.
//

#ifndef TILEGENERATOR_NEOGENERATOR_H
#define TILEGENERATOR_NEOGENERATOR_H

#include "MapGenerator.h"
#include <vector>

using std::vector;

/*
 * 用依次往一个房间周围连接房间的方式生成地牢
 * 目前的缺点是，生成的时候可能会形成一个环，类似贪吃蛇被自己堵住那样，造成死局
 * （可能要改变连接的顺序）
 * 这个生成方式的优点在于，可以自定义固定的房间模式。
 */
class NeoGenerator : MapGenerator {
public:
    typedef unsigned int Size;
    enum RoomType {
        ROOM,
        RECTANGLE,
        CIRCLE,
        PATH,
        CORRIDOR,
        LABYRINTH,
        STRAIGHT
    };

    class Chamber {
    protected:
        friend class NeoGenerator;

        // 房间左下角的坐标
        Coordinate x, y;
        Size height, width;
        // 房间的形状
        vector<Map::Point> tiles;
        // 房间/走廊的门
        vector<Map::Point> connectors;
        RoomType type;
    public:
        Chamber() : x(-1), y(-1), height(0), width(0), tiles(), connectors() {}

        Chamber(Size height, Size width, RoomType type) : x(-1), y(-1), height(height), width(width), tiles(),
                                                          connectors(), type(type) {}

        Chamber(Coordinate x, Coordinate y, Size height, Size width, RoomType type) : x(x), y(y), height(height),
                                                                                      width(width), tiles(),
                                                                                      connectors(), type(type) {}
    };

    struct Variables {
        Size roomSize_min, roomSize_max;
        Size corridorWidth;
        // 最大房间数量（然而由于死局的情况一般会小很多）
        unsigned int room_count;
        // 一个房间周围连接房间的最大数量
        unsigned int max_connection;
        // 连接房间时的尝试次数
        unsigned int attempts;

        Variables() {
            roomSize_min = 10,
            roomSize_max = 20,
            attempts = 10;
            room_count = 10;
            corridorWidth = 1;
            max_connection = 4;
        }
    };

    NeoGenerator() = default;

    NeoGenerator(Coordinate height, Coordinate width, Variables variables = NeoGenerator::Variables());

    ~NeoGenerator() override;

    void generateMap(Map *map) override;

private:
    Variables _currentVariables;
    vector<Chamber> chambers;

    void _spawnCorridor();

    void _spawnStraight();

    void spawnPath(RoomType type = CORRIDOR);

    void _spawnRectangle();

    void spawnRoom(RoomType type = RECTANGLE);

    bool checkChamber(const Chamber &chamber, const Map::Point &connector);

    bool attachRoom(Chamber &chamber, RoomType type = ROOM);

    void removeDeadends();

#define rand_coord1(x) (Coordinate)(random()*(x))
#define rand_coord2(x, y) (Coordinate)(random() * ((y)-(x)) + (x))
#define rand_size1(x) (Size)(random()*(x))
#define rand_size2(x, y) (Size)(random()* ((y)-(x)) + (x))
};

#endif //TILEGENERATOR_NEOGENERATOR_H
