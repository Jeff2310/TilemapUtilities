//
// Created by jeff2310 on 5/16/18.
//

#ifndef TILEGENERATOR_PATHFINDER_H
#define TILEGENERATOR_PATHFINDER_H

#include "Map.h"
#include "../PriorityQueue.h"
#include <queue>
#include <iostream>

enum Algorithm{
    Astar,
    Dijkstra,
};
enum Heuristic{
    Manhattan,
    Diagnose,
};

class PathFinder {
private:
    Map *currentMap;
    Heuristic currentHeuristic;
    Algorithm currentAlgorithm;

    class Node{
    public:
        Map::Point coord;
        Map::Point parent;
        Distance f, g, h;
        Node():coord(), f(-1), g(-1), h(-1){}
        Node(const Map::Point &parent, const Map::Point &coord, Distance g, Distance h): parent(parent), coord(coord), g(g), h(h), f(g+h){}
        void update(Distance g, Distance h){
            this->g = g;
            this->h = h;
            this->f = g+h;
        }
        bool operator < (const Node& n) const{
            return this->f < n.f;
        }
        bool operator > (const Node& n) const{
            return this->f > n.f;
        }
        bool operator == (const Node& n) const{
            return this->coord.x == n.coord.x && this->coord.y == n.coord.y;
        }
    };

    inline Distance calc_distance(const Map::Point &p1, const Map::Point &p2){
        Distance d = 0;
        switch(currentHeuristic){
            case Manhattan:
                d += (p1.x - p2.x) > 0? (p1.x - p2.x) : (p2.x - p1.x);
                d += (p1.y - p2.y) > 0? (p1.y - p2.y) : (p2.y - p1.y);
                break;
            case Diagnose:
                d = -1;
                break;
            default:
                d = -1;
                break;
        }
        return d;
    }

    void algo_Astar(const Map &map, const Map::Point &start, const Map::Point end){
        PriorityQueue<Node> open_list;
        if(map.tile(start.x, start.y).getType() == WALL)
            return;
        if(map.tile(end.x, end.y).getType() == WALL)
            return;
        int height = map.getHeight();
        int width = map.getWidth();
        bool *closed = new bool[width*height];
        Distance *distance = new Distance[width*height];
        Map::Point *parent = new Map::Point[width*height];
        for(Coordinate y=0; y<height; y++){
            for(Coordinate x=0; x<width; x++){
                distance[y*height + x] = width+height+1;
                closed[y*height + x] = false;
                parent[y*height + x] = Map::Point(x, y);
            }
        }

        Distance currentDistance;
        Node currentNode;
        Node nextNode;
        currentDistance = calc_distance(start, end);
        open_list.push(Node(start, start, 0, currentDistance));
        distance[start.y * width + start.x] = 0;
        while(!open_list.empty()){
            currentNode = open_list.top(); open_list.pop();
            Map::Point &currentCoord = currentNode.coord;
            closed[currentCoord.y * width + currentCoord.x] = true;
            if(currentCoord.x==end.x && currentCoord.y==end.y) break;
            for(int dir=0; dir<movements; dir++){
                nextNode = currentNode;
                Map::Point &nextCoord = nextNode.coord;
                nextCoord.x += movement_dirs[dir][0];
                nextCoord.y += movement_dirs[dir][1];
                map.wrap(&nextCoord.x, &nextCoord.y);
                if(map.tile(nextCoord.x, nextCoord.y).getType() == WALL)
                    continue;
                if(closed[nextCoord.y * width + nextCoord.x])
                    continue;
                currentDistance = calc_distance(nextCoord, end);
                nextNode.update(nextNode.g+1, currentDistance);
                nextNode.parent = currentCoord;

                if(nextNode.g < distance[nextCoord.y * width + nextCoord.x]){
                    open_list.find_and_remove(nextNode);
                    open_list.push(nextNode);
                    distance[nextCoord.y * width + nextCoord.x] = nextNode.g;
                    parent[nextCoord.x + nextCoord.y * width] = currentCoord;
                    map.tile(nextCoord.x, nextCoord.y).setType(QUEUED);
                }
            }
        }

        Map::Point currentCoord = end;
        while(true){
            map.tile(currentCoord.x, currentCoord.y).setType(PATH);
            if(currentCoord.x == start.x && currentCoord.y == start.y) break;
            currentCoord = parent[currentCoord.y*width + currentCoord.x];
        }
        delete closed;
        delete distance;
    }
public:
    PathFinder() : currentMap(nullptr){}
    void FindPath(const Map &map, const Map::Point &start, const Map::Point &end, Algorithm a = Astar, Heuristic h = Manhattan) {
        currentAlgorithm = a;
        currentHeuristic = h;
        switch(a){
            case Astar:
                algo_Astar(map, start, end);
                break;
            case Dijkstra:
                break;
            default:
                break;
        }
    }
};


#endif //TILEGENERATOR_PATHFINDER_H
