//
// Created by jeff2310 on 5/16/18.
//

#ifndef TILEGENERATOR_PATHFINDER_H
#define TILEGENERATOR_PATHFINDER_H

#include "Map.h"
#include <queue>
#include <iostream>

enum Algorithm {
    Astar,
    Dijkstra,
};
enum Heuristic {
    Manhattan,
    Diagnose,
};

class PathFinder {
private:
    Map *currentMap;
    Heuristic currentHeuristic;
    Algorithm currentAlgorithm;

    class Node {
    public:
        Map::Point coord;
        Map::Point parent;
        Distance f, g, h;

        Node() : coord(), f(-1), g(-1), h(-1) {}

        Node(const Map::Point &parent, const Map::Point &coord, Distance g, Distance h) : parent(parent), coord(coord),
                                                                                          g(g), h(h), f(g + h) {}

        void update(Distance g, Distance h) {
            this->g = g;
            this->h = h;
            this->f = g + h;
        }

        bool operator<(const Node &n) const {
            return this->f < n.f;
        }

        bool operator>(const Node &n) const {
            return this->f > n.f;
        }

        bool operator==(const Node &n) const {
            return this->coord.x == n.coord.x && this->coord.y == n.coord.y;
        }
    };

    inline Distance calc_distance(const Map::Point &p1, const Map::Point &p2) {
        Distance d = 0;
        switch (currentHeuristic) {
            case Manhattan:
                d += (p1.x - p2.x) > 0 ? (p1.x - p2.x) : (p2.x - p1.x);
                d += (p1.y - p2.y) > 0 ? (p1.y - p2.y) : (p2.y - p1.y);
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

    void algo_Astar(const Map &map, const Map::Point &start, const Map::Point end);

public:
    PathFinder() : currentMap(nullptr) {}

    void FindPath(const Map &map, const Map::Point &start, const Map::Point &end, Algorithm a = Astar,
                  Heuristic h = Manhattan) {
        currentAlgorithm = a;
        currentHeuristic = h;
        switch (a) {
            case Astar:
                algo_Astar(map, start, end);
                break;
            case Dijkstra:
                // 摸了
                break;
            default:
                break;
        }
    }
};


#endif //TILEGENERATOR_PATHFINDER_H
