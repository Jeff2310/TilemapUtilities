//
// Created by jeff2310 on 5/18/18.
//

#include <iostream>
#include "Generator/NeoGenerator.h"
#include <queue>

using std::queue;

NeoGenerator::NeoGenerator(Coordinate height, Coordinate width, Variables variables) : MapGenerator(height, width),
                                                                                       _currentVariables(variables) {
}

NeoGenerator::~NeoGenerator() {

}

void NeoGenerator::generateMap(Map *map) {
    random_fill(-1.0f);
    spawnRoom(RECTANGLE);
//    chambers.back().x = Coordinate(random()*this->map->getWidth());
//    chambers.back().y = Coordinate(random()*this->map->getHeight());
    chambers.back().x = this->map->getWidth() / 2;
    chambers.back().y = this->map->getHeight() / 2;
    for (auto pos: chambers.back().tiles) {
        Coordinate x = chambers.back().x + pos.x;
        Coordinate y = chambers.back().y + pos.y;
        if (!this->map->in_bound(x, y)) continue;
        this->map->tile(x, y).setType(FLOOR);
    }
    this->map->dump();
    RoomType currentType;
    Chamber currentChamber;
    for (int i = 0; i < _currentVariables.room_count;) {
        currentChamber = chambers.back();
        currentType = currentChamber.type;
        for (int j = 0; j < _currentVariables.max_connection; j++) {
            // 房间与房间直接相连，也有几率连接一个走廊
            if (currentType == ROOM && random() > 0.8f)
                attachRoom(currentChamber, STRAIGHT);
                // 但走廊只能连续生成一次
            else
                attachRoom(currentChamber, RECTANGLE), i++;
        }
    }
    // 把走廊没被连接凸出来的部分去掉。
    removeDeadends();
    *map = *(this->map);
}

void NeoGenerator::_spawnRectangle() {
    Size height = rand_size2(_currentVariables.roomSize_min, _currentVariables.roomSize_max);
    Size width = rand_size2(_currentVariables.roomSize_min, _currentVariables.roomSize_max);
    chambers.emplace_back(height, width, ROOM);
    Chamber &newChamber = chambers.back();
    // 一圈外围所有点都可能成为门
    for (Coordinate x = 0; x < width; x++) {
        newChamber.connectors.emplace_back(x, -1);
        newChamber.connectors.emplace_back(x, height);
    }
    for (Coordinate y = 0; y < height; y++) {
        newChamber.connectors.emplace_back(-1, y);
        newChamber.connectors.emplace_back(width, y);
    }
    for (Coordinate y = 0; y < height; y++) {
        for (Coordinate x = 0; x < width; x++) {
            newChamber.tiles.emplace_back(x, y);
        }
    }
}

void NeoGenerator::spawnRoom(RoomType type) {
    switch (type) {
        case RECTANGLE:
            _spawnRectangle();
            return;
        default:
            return;
    }
}

void NeoGenerator::_spawnCorridor() {

}

void NeoGenerator::_spawnStraight() {
    Coordinate primaryLength = rand_size1(_currentVariables.roomSize_max);
    Coordinate secondaryLength = _currentVariables.corridorWidth;
    float rand = random();
    bool x_primary = rand > 0.5f;
    if (x_primary)
        chambers.emplace_back(primaryLength, secondaryLength, PATH);
    else
        chambers.emplace_back(secondaryLength, primaryLength, PATH);

    Chamber &newChamber = chambers.back();
    for (Coordinate primary = 0; primary < primaryLength; primary++) {
        for (Coordinate secondary = 0; secondary < secondaryLength; secondary++) {
            if (x_primary)
                newChamber.tiles.emplace_back(primary, secondary);
            else
                newChamber.tiles.emplace_back(secondary, primary);
        }
    }
    for (Coordinate primary = 0; primary < primaryLength; primary++) {
        if (x_primary) {
            newChamber.connectors.emplace_back(primary, -1);
            newChamber.connectors.emplace_back(primary, secondaryLength);
        } else {
            newChamber.connectors.emplace_back(-1, primary);
            newChamber.connectors.emplace_back(secondaryLength, primary);
        }
    }
//    for(Coordinate secondary = 0; secondary<secondaryLength; secondary++){
//        if(x_primary) {
//            newChamber.connectors.emplace_back(0, secondary);
//            newChamber.connectors.emplace_back(primaryLength-1, secondary);
//        }else{
//            newChamber.connectors.emplace_back(secondary, 0);
//            newChamber.connectors.emplace_back(secondary, primaryLength-1);
//        }
//    }
}

void NeoGenerator::spawnPath(RoomType type) {
    switch (type) {
        case STRAIGHT:
            _spawnStraight();
            return;
        default:
            return;
    }
}

bool NeoGenerator::checkChamber(const Chamber &chamber, const Map::Point &connector) {
    for (auto pos : chamber.tiles) {
        Coordinate x = chamber.x + pos.x;
        Coordinate y = chamber.y + pos.y;
        if (!this->map->in_bound(x, y))
            return false;
        if (map->tile(x, y).getType() != WALL)
            return false;
    }
    for (auto pos : chamber.connectors) {
        Coordinate x = chamber.x + pos.x;
        Coordinate y = chamber.y + pos.y;
        if (!this->map->in_bound(x, y))
            return false;
        if (x == connector.x && y == connector.y) continue;
        if (map->tile(x, y).getType() != WALL)
            return false;
    }
    return true;
}

bool NeoGenerator::attachRoom(Chamber &chamber, RoomType type) {
    if (type >= ROOM && type < PATH)
        spawnRoom(type);
    else if (type >= PATH)
        spawnPath(type);
    for (int i = 0; i < _currentVariables.attempts; i++) {
        Chamber &newChamber = chambers.back();
        int from_connector = int(random() * (chamber.connectors.size() - 1));
        int to_connector = int(random() * (newChamber.connectors.size() - 1));
        if (newChamber.connectors.size() == 0) break;
        Map::Point from = chamber.connectors[from_connector];
        Map::Point to = newChamber.connectors[to_connector];
        newChamber.x = chamber.x + from.x - to.x;
        newChamber.y = chamber.y + from.y - to.y;
        if (checkChamber(newChamber, Map::Point(chamber.x + from.x, chamber.y + from.y))) {
            Coordinate x = chamber.x + from.x;
            Coordinate y = chamber.y + from.y;
            if (!this->map->in_bound(x, y)) continue;
            map->tile(x, y).setType(FLOOR);
            for (auto pos: newChamber.tiles) {
                x = newChamber.x + pos.x;
                y = newChamber.y + pos.y;
                if (!this->map->in_bound(x, y)) continue;
                this->map->tile(x, y).setType(FLOOR);
            }
            return true;
        }
    }
    chambers.pop_back();
    return false;
}

void NeoGenerator::removeDeadends() {
    queue<Map::Point> flood;
    int height = map->getHeight(), width = map->getWidth();

    Coordinate nx, ny, x, y;
    int ends;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (this->map->tile(x, y).getType() != FLOOR) continue;
            ends = 0;
            for (int dir = 0; dir < movements; dir++) {
                nx = x + movement_dirs[dir][0];
                ny = y + movement_dirs[dir][1];
                if (!this->map->in_bound(nx, ny)) {
                    ends++;
                    continue;
                }
                if (this->map->tile(nx, ny).getType() == WALL) ends++;
            }
            if (ends == movements - 1)
                flood.push(Map::Point(x, y));
        }
    }
    Map::Point currentPoint;
    while (!flood.empty()) {
        currentPoint = flood.front();
        flood.pop();
        ends = 0;
        for (int dir = 0; dir < movements; dir++) {
            nx = currentPoint.x + movement_dirs[dir][0];
            ny = currentPoint.y + movement_dirs[dir][1];
            if (!this->map->in_bound(nx, ny)) {
                ends++;
                continue;
            }
            if (this->map->tile(nx, ny).getType() == WALL) ends++;
        }
        if (ends != movements - 1) continue;

        this->map->tile(currentPoint.x, currentPoint.y).setType(WALL);
        for (int dir = 0; dir < movements; dir++) {
            nx = currentPoint.x + movement_dirs[dir][0];
            ny = currentPoint.y + movement_dirs[dir][1];
            if (!this->map->in_bound(nx, ny)) continue;
            if (this->map->tile(nx, ny).getType() != FLOOR) continue;
            flood.push(Map::Point(nx, ny));
        }
    }
}