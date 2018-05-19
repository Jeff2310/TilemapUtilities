//
// Created by jeff2310 on 5/17/18.
//

#include "Generator/DEPRECATED_DungeonGenerator.h"
#include <queue>
#include <algorithm>
#include <iostream>

using std::sort;
using std::less;
using std::queue;

DEPRECATED_DungeonGenerator::DEPRECATED_DungeonGenerator(Coordinate height, Coordinate width, Variables variables)
        : MapGenerator(height, width), _currentVariable(variables), random_x(0, width-1),
          random_y(0, height - 1), random_size(variables.roomSize_min, variables.roomSize_max) {
}

DEPRECATED_DungeonGenerator::~DEPRECATED_DungeonGenerator() {

}

void DEPRECATED_DungeonGenerator::spawnRoom() {
    Coordinate room_x = rand_x, room_y = rand_y;
    Size room_width = rand_size, room_height = rand_size;
    Coordinate width = map->getWidth(), height = map->getHeight();
    for (Coordinate y = -3; y < (Coordinate) room_height + 3; y++) {
        for (Coordinate x = -3; x < (Coordinate) room_width + 3; x++) {
            {
                Coordinate location_x = room_x + x, location_y = room_y + y;
                if (location_x < 0 || location_x > width) return;
                if (location_y < 0 || location_y > height) return;
                if (map->tile(location_x, location_y).getType() != WALL)
                    return;
            }
        }
    }
    for (Coordinate y = 0; y < room_height; y++) {
        for (Coordinate x = 0; x < room_width; x++) {
            {
                Coordinate location_x = room_x + x, location_y = room_y + y;
                map->tile(location_x, location_y).setType(FLOOR);
            }
        }
    }
    rooms.emplace_back(room_x, room_y, room_height, room_width);
    rooms.back().connectors.emplace_back(room_x + room_width-1, room_y + (room_height - 1) / 2);
    rooms.back().connectors.emplace_back(room_x, room_y + (room_height - 1) / 2);
    rooms.back().connectors.emplace_back(room_x + (room_width - 1) / 2, room_y);
    rooms.back().connectors.emplace_back(room_x + (room_width - 1) / 2, room_y + room_height-1);
}

bool testCorridorXY(Map *map, Coordinate x1, Coordinate y1, Coordinate x2, Coordinate y2){
    Coordinate y_inc = y2 > y1? 1 : -1;
    Coordinate x_inc = x2 > x1? 1 : -1;
    for(Coordinate x=x1;; x+= x_inc){
        if(x!=x1 && x!=x2 && map->tile(x, y1).getType() == FLOOR){
            return false;
        }
        if (x == x2) break;
    }
    for (Coordinate y = y1;; y += y_inc) {
        if (y!=y1 && y!= y2 && map->tile(x2, y).getType() == FLOOR) {
            return false;
        }
        if (y == y2) break;
    }
    return true;
}

bool testCorridorYX(Map *map, Coordinate x1, Coordinate y1, Coordinate x2, Coordinate y2){
    Coordinate y_inc = y2 > y1? 1 : -1;
    Coordinate x_inc = x2 > x1? 1 : -1;
    for (Coordinate y = y1;; y += y_inc) {
        if (y!=y1 && y!= y2 && map->tile(x1, y).getType() == FLOOR) {
            return false;
        }
        if (y == y2) break;
    }
    for(Coordinate x=x1;; x+= x_inc){
        if(x!=x1 && x!=x2 && map->tile(x, y2).getType() == FLOOR){
            return false;
        }
        if (x == x2) break;
    }

    return true;
}

bool testCorridorXX(Map *map, Coordinate x1, Coordinate y1, Coordinate x2, Coordinate y2){
    Coordinate y_inc = y2 > y1? 1 : -1;
    Coordinate x_inc = x2 > x1? 1 : -1;
    Coordinate x_middle = (x1+x2)/2;
    for(Coordinate x=x1;; x+= x_inc){
        if(x!=x1 && x!=x2 && map->tile(x, y1).getType() == FLOOR){
            return false;
        }
        if (x == x_middle) break;
    }
    for(Coordinate x=x_middle;; x+= x_inc){
        if(x!=x1 && x!=x2 && map->tile(x, y2).getType() == FLOOR){
            return false;
        }
        if (x == x2) break;
    }
    for (Coordinate y = y1;; y += y_inc) {
        if (y!=y1 && y!= y2 && map->tile(x_middle, y).getType() == FLOOR) {
            return false;
        }
        if (y == y2) break;
    }
    return true;
}

bool testCorridorYY(Map *map, Coordinate x1, Coordinate y1, Coordinate x2, Coordinate y2){
    Coordinate y_inc = y2 > y1? 1 : -1;
    Coordinate x_inc = x2 > x1? 1 : -1;
    Coordinate y_middle = (y1+y2)/2;
    for(Coordinate x=x1;; x+= x_inc){
        if(x!=x1 && x!=x2 && map->tile(x, y_middle).getType() == FLOOR){
            return false;
        }
        if (x == x2) break;
    }
    for (Coordinate y = y1;; y += y_inc) {
        if (y!=y1 && y!= y2 && map->tile(x1, y).getType() == FLOOR) {
            return false;
        }
        if (y == y_middle) break;
    }
    for (Coordinate y = y_middle;; y += y_inc) {
        if (y!=y1 && y!= y2 && map->tile(x2, y).getType() == FLOOR) {
            return false;
        }
        if (y == y2) break;
    }
    return true;
}

void carveCorridorXY(Map *map, Coordinate x1, Coordinate y1, Coordinate x2, Coordinate y2){
    Coordinate y_inc = y2 > y1? 1 : -1;
    Coordinate x_inc = x2 > x1? 1 : -1;
    for(Coordinate x=x1;; x+= x_inc){
        map->tile(x, y1).setType(CORRIDOR);
        map->tile(x-1, y1+1).setType(CORRIDOR);
        map->tile(x+1, y1-1).setType(CORRIDOR);
        map->tile(x-1, y1-1).setType(CORRIDOR);
        map->tile(x+1, y1+1).setType(CORRIDOR);
        if (x == x2) break;
    }
    for (Coordinate y = y1;; y += y_inc) {
        map->tile(x2, y).setType(CORRIDOR);
        map->tile(x2-1, y+1).setType(CORRIDOR);
        map->tile(x2+1, y-1).setType(CORRIDOR);
        map->tile(x2-1, y-1).setType(CORRIDOR);
        map->tile(x2+1, y+1).setType(CORRIDOR);
        if (y == y2) break;
    }
}

void carveCorridorYX(Map *map, Coordinate x1, Coordinate y1, Coordinate x2, Coordinate y2){
    Coordinate y_inc = y2 > y1? 1 : -1;
    Coordinate x_inc = x2 > x1? 1 : -1;
    for (Coordinate y = y1;; y += y_inc) {
        map->tile(x1, y).setType(CORRIDOR);
        map->tile(x1-1, y+1).setType(CORRIDOR);
        map->tile(x1+1, y-1).setType(CORRIDOR);
        map->tile(x1-1, y-1).setType(CORRIDOR);
        map->tile(x1+1, y+1).setType(CORRIDOR);
        if (y == y2) break;
    }
    for(Coordinate x=x1;; x+= x_inc){
        map->tile(x, y2).setType(CORRIDOR);
        map->tile(x-1, y2+1).setType(CORRIDOR);
        map->tile(x+1, y2-1).setType(CORRIDOR);
        map->tile(x-1, y2-1).setType(CORRIDOR);
        map->tile(x+1, y2+1).setType(CORRIDOR);
        if (x == x2) break;
    }
}

void carveCorridorXX(Map *map, Coordinate x1, Coordinate y1, Coordinate x2, Coordinate y2){
    Coordinate y_inc = y2 > y1? 1 : -1;
    Coordinate x_inc = x2 > x1? 1 : -1;
    Coordinate x_middle = (x1+x2)/2;
    for(Coordinate x=x1;; x+= x_inc){
        map->tile(x, y1).setType(CORRIDOR);
        map->tile(x-1, y1+1).setType(CORRIDOR);
        map->tile(x+1, y1-1).setType(CORRIDOR);
        map->tile(x-1, y1-1).setType(CORRIDOR);
        map->tile(x+1, y1+1).setType(CORRIDOR);
        if (x == x_middle) break;
    }
    for(Coordinate x=x_middle;; x+= x_inc){
        map->tile(x, y2).setType(CORRIDOR);
        map->tile(x-1, y2+1).setType(CORRIDOR);
        map->tile(x+1, y2-1).setType(CORRIDOR);
        map->tile(x-1, y2-1).setType(CORRIDOR);
        map->tile(x+1, y2+1).setType(CORRIDOR);
        if (x == x2) break;
    }
    for (Coordinate y = y1;; y += y_inc) {
        map->tile(x_middle, y).setType(CORRIDOR);
        map->tile(x_middle-1, y+1).setType(CORRIDOR);
        map->tile(x_middle+1, y-1).setType(CORRIDOR);
        map->tile(x_middle-1, y-1).setType(CORRIDOR);
        map->tile(x_middle+1, y+1).setType(CORRIDOR);
        if (y == y2) break;
    }
}

void carveCorridorYY(Map *map, Coordinate x1, Coordinate y1, Coordinate x2, Coordinate y2){
    Coordinate y_inc = y2 > y1? 1 : -1;
    Coordinate x_inc = x2 > x1? 1 : -1;
    Coordinate y_middle = (y1+y2)/2;
    for(Coordinate x=x1;; x+= x_inc){
        map->tile(x, y_middle).setType(CORRIDOR);
        map->tile(x-1, y_middle+1).setType(CORRIDOR);
        map->tile(x+1, y_middle-1).setType(CORRIDOR);
        map->tile(x-1, y_middle-1).setType(CORRIDOR);
        map->tile(x+1, y_middle+1).setType(CORRIDOR);
        if (x == x2) break;
    }
    for (Coordinate y = y_middle;; y += y_inc) {
        map->tile(x2, y).setType(CORRIDOR);
        map->tile(x2-1, y+1).setType(CORRIDOR);
        map->tile(x2+1, y-1).setType(CORRIDOR);
        map->tile(x2-1, y-1).setType(CORRIDOR);
        map->tile(x2+1, y+1).setType(CORRIDOR);
        if (y == y2) break;
    }
    for (Coordinate y = y1;; y += y_inc) {
        map->tile(x1, y).setType(CORRIDOR);
        map->tile(x1-1, y+1).setType(CORRIDOR);
        map->tile(x1+1, y-1).setType(CORRIDOR);
        map->tile(x1-1, y-1).setType(CORRIDOR);
        map->tile(x1+1, y+1).setType(CORRIDOR);
        if (y == y_middle) break;
    }
}

bool DEPRECATED_DungeonGenerator::__carveCorridor(Coordinate x1, Coordinate y1, Coordinate x2, Coordinate y2) {
    bool canCarveXY=false, canCarveYX=false;
    bool canCarveXX=false, canCarveYY=false;
    if(testCorridorXY(map, x1, y1, x2, y2)){
        canCarveXY = true;
    }
    if(testCorridorYX(map, x1, y1, x2, y2)){
        canCarveYX = true;
    }
    if(testCorridorXX(map, x1, y1, x2, y2)){
        canCarveXX = true;
    }
    if(testCorridorYY(map, x1, y1, x2, y2)){
        canCarveYY = true;
    }

    if(canCarveXY&&canCarveYX){
        float rand = random();
        if(rand > 0.5f){
            carveCorridorXY(map, x1, y1, x2, y2);
        }else{
            carveCorridorYX(map, x1, y1, x2, y2);
        }
    }else if(canCarveXY){
        carveCorridorXY(map, x1, y1, x2, y2);
    }else if(canCarveYX){
        carveCorridorYX(map, x1, y1, x2, y2);
    }else{
        if(canCarveXX&&canCarveYY){
            float rand = random();
            if(rand > 0.5f){
                carveCorridorXX(map, x1, y1, x2, y2);
            }else{
                carveCorridorYY(map, x1, y1, x2, y2);
            }
        }else if(canCarveXX){
            carveCorridorXX(map, x1, y1, x2, y2);
        }else if(canCarveYY){
            carveCorridorYY(map, x1, y1, x2, y2);
        }else{
            return false;
        }
    }
    return true;
}

bool DEPRECATED_DungeonGenerator::carveCorridor(const Path &path) {
    int from_region = path.from, to_region = path.to;
    Room &from = rooms[from_region], &to = rooms[to_region];

    for(auto from_connector : from.connectors){
        for(auto to_connector : to.connectors){
            if(__carveCorridor(from_connector.x, from_connector.y, to_connector.x, to_connector.y)) return true;
        }
    }

    return false;
}

int findParent(int *parent, int x){
    while(x!=parent[x]) {
        x = parent[x];
    }
    return x;
}

void joinUnion(int *parent, int x, int y){
    int fx=findParent(parent, x),fy=findParent(parent, y);
    if(fx!=fy) parent[fx]=fy;
}

void DEPRECATED_DungeonGenerator::connectRooms() {
    vector<Path> paths;
    int remain_regions = (int)rooms.size();
    int *parent = new int[remain_regions];
    for(int i=0; i<remain_regions; i++)
        parent[i] = i;
    for(int i=0; i<rooms.size(); i++){
        for(int j=0; j<i; j++){
            int length = 0;
            length += (rooms[i].x - rooms[j].x > 0? rooms[i].x - rooms[j].x : rooms[j].x - rooms[i].x);
            length += (rooms[i].y - rooms[j].y > 0? rooms[i].y - rooms[j].y : rooms[j].y - rooms[i].y);
            paths.emplace_back(length, i ,j ,Corridor);
        }
    }
//    sort(paths.begin(), paths.end(), std::less<Path>());
    std::random_shuffle(paths.begin(), paths.end());
    for(auto path: paths){
        if(remain_regions == 1) {
            std::cout<< "construction complete"<<std::endl;
            break;
        }
        int region_from = findParent(parent, path.from);
        int region_to = findParent(parent, path.to);
        if(region_from == region_to) continue;
        if(path.type == Corridor) {
            std::cout<< "try connecting "<<path.from << " and " << path.to<<std::endl;
            bool result = carveCorridor(path);
            if(!result) {
                std::cout<< "##Failed" << std::endl;
                continue;
            }
        }
        joinUnion(parent, region_from, region_to);
        std::cout<< ":)            connected "<<path.from << " and " << path.to<<std::endl;

//        map->dump();
        remain_regions--;
    }

    delete parent;
}

void DEPRECATED_DungeonGenerator::generateMap(Map *map) {
    random_fill(0.0f);
    Coordinate height = map->getHeight(), width = map->getWidth();
    auto bool_buffer = new bool[height * width];
    bool *covered = bool_buffer;
    for(Coordinate y=0; y<height; y++){
        for(Coordinate x=0; x<width; x++){
            covered[y*width+x] = false;
        }
    }
    rooms = vector<Room>();
    for(int count = 1; count <= _currentVariable.attempts; count ++){
        spawnRoom();
//        this->map->dump();
    }
    for(Coordinate y=0; y<height; y++){
        for(Coordinate x=0; x<width; x++){
            if(map->tile(x, y).getType() == CORRIDOR)
                map->tile(x, y).setType(FLOOR);
        }
    }
    connectRooms();
    this->map->dump();
    *map = *this->map;
    delete bool_buffer;
}
