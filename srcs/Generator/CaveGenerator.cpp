//
// Created by jeff2310 on 5/10/18.
//

#include "Generator/CaveGenerator.h"
#include "Noise/Noise.h"


#include <queue>

using std::queue;

CaveGenerator::CaveGenerator() : MapGenerator(){
    mapBuffer = nullptr;
}
CaveGenerator::CaveGenerator(Coordinate height, Coordinate width) : MapGenerator(height, width) {
    mapBuffer = nullptr;
}
CaveGenerator::~CaveGenerator() {
    if(mapBuffer != nullptr)
        delete mapBuffer;
}


int countNeighbours(const Map& map, Coordinate x, Coordinate y, Coordinate radius){
    int count = 0;
    for(Coordinate ny=y-radius; ny<=y+radius && ny<map.getHeight(); ny++){
        for(Coordinate nx=x-radius; nx<=x+radius && nx<map.getWidth(); nx++){
            map.wrap(&nx, &ny);
            if(map.tile(nx, ny).getType()==FLOOR)
                count++;
        }
    }
    return count;
}

int countHorizionalNeighbours(const Map& map, Coordinate x, Coordinate y, Coordinate radius){
    int count = 0;
    Coordinate ny1 = y+1;
    Coordinate ny2 = y-1;
    for(Coordinate nx=x-radius; nx<=x+radius && nx<map.getWidth(); nx++){
        map.wrap(&nx, &ny1);
        map.wrap(&nx, &ny2);
        if(map.tile(nx, ny1).getType()==FLOOR)
            count++;
        if(map.tile(nx, ny2).getType()==FLOOR)
            count++;
        if(map.tile(nx, y).getType()==FLOOR)
            count++;
    }
    return count;
}

// a single iteration of Cellular Automation
void CaveGenerator::automation_iteration() {
    int height = map->getHeight();
    int width = map->getWidth();
    for(Coordinate y=0; y<height; y++){
        for(Coordinate x=0; x<width; x++) {
            int count1=countNeighbours(*map, x, y, 1);
            int count2=countNeighbours(*map, x, y, 2);
            if(count1>=5 || count2==0){
                mapBuffer->tile(x, y).setType(FLOOR);
            }else{
                mapBuffer->tile(x, y).setType(WALL);
            }
        }
    }
    Map *temp;
    temp = map, map = mapBuffer, mapBuffer = temp;
}

void CaveGenerator::flood_fill(){
    queue<Map::Point> flood;
    int height = map->getHeight(), width = map->getWidth();
    int *region = new int[height * width];
    std::vector<int> size;
    int region_count = 0;

    for(Coordinate y = 0; y < height; y++){
        for(Coordinate x = 0; x < width; x++){
            if(map->tile(x, y).getType() == FLOOR)
                region[y*width+x] = -1;
            else
                region[y*width+x] = -2;
        }
    }

    Map::Point currentPoint, nextPoint;
    for(Coordinate y = 0; y < height; y++){
        for(Coordinate x = 0; x < width; x++){
            if(region[y*width+x] == -1){
                flood = queue<Map::Point>();
                flood.push(Map::Point(x, y));
                region[y*width+x] = region_count;
                size.push_back(0);
                while(!flood.empty()){
                    currentPoint = flood.front(); flood.pop();
                    size.back() += 1;
                    for(int dir=0; dir<movements; dir++){
                        nextPoint = currentPoint;
                        nextPoint.x += movement_dirs[dir][0];
                        nextPoint.y += movement_dirs[dir][1];
                        map->wrap(&nextPoint.x, &nextPoint.y);
                        if(region[nextPoint.y * width + nextPoint.x] == -1){
                            region[nextPoint.y * width + nextPoint.x] = region_count;
                            flood.push(nextPoint);
                        }
                    }
                }
                region_count++;
            }
        }
    }

    int index=0;
    int biggest_index = 0;
    int biggest_size = 0;
    for(index=0; index<size.size(); index++){
        if(size[index] > biggest_size) biggest_size = size[index], biggest_index = index;
    }
    for(Coordinate y = 0; y < height; y++) {
        for (Coordinate x = 0; x < width; x++) {
            if(region[y*width+x]!=biggest_index)
                map->tile(x,y).setType(WALL);
        }
    }

    delete region;
}

void CaveGenerator::generateMap(Map *map, unsigned int iterations, float init_poss){
    random_fill(init_poss);
    mapBuffer = new Map(*this->map);
    for(unsigned int iter = 1; iter <= iterations; iter++){
        automation_iteration();
    }
    flood_fill();
    *map = *(this->map);
}