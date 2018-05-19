//
// Created by jeff2310 on 5/16/18.
//

#include "Tilemap/PathFinder.h"
#include "Utility/PriorityQueue.h"

void PathFinder::algo_Astar(const Map &map, const Map::Point &start, const Map::Point end) {
    PriorityQueue<Node> open_list;
    if (map.tile(start.x, start.y).getType() == WALL)
        return;
    if (map.tile(end.x, end.y).getType() == WALL)
        return;
    int height = map.getHeight();
    int width = map.getWidth();
    bool *closed = new bool[width * height];
    Distance *distance = new Distance[width * height];
    Map::Point *parent = new Map::Point[width * height];
    for (Coordinate y = 0; y < height; y++) {
        for (Coordinate x = 0; x < width; x++) {
            distance[y * height + x] = width + height + 1;
            closed[y * height + x] = false;
            parent[y * height + x] = Map::Point(x, y);
        }
    }

    Distance currentDistance;
    Node currentNode;
    Node nextNode;
    currentDistance = calc_distance(start, end);
    open_list.push(Node(start, start, 0, currentDistance));
    distance[start.y * width + start.x] = 0;
    while (!open_list.empty()) {
        currentNode = open_list.top();
        open_list.pop();
        Map::Point &currentCoord = currentNode.coord;
        closed[currentCoord.y * width + currentCoord.x] = true;
        if (currentCoord.x == end.x && currentCoord.y == end.y) break;
        for (int dir = 0; dir < movements; dir++) {
            nextNode = currentNode;
            Map::Point &nextCoord = nextNode.coord;
            nextCoord.x += movement_dirs[dir][0];
            nextCoord.y += movement_dirs[dir][1];
            map.wrap(&nextCoord.x, &nextCoord.y);
            if (map.tile(nextCoord.x, nextCoord.y).getType() == WALL)
                continue;
            if (closed[nextCoord.y * width + nextCoord.x])
                continue;
            currentDistance = calc_distance(nextCoord, end);
            nextNode.update(nextNode.g + 1, currentDistance);
            nextNode.parent = currentCoord;

            if (nextNode.g < distance[nextCoord.y * width + nextCoord.x]) {
                open_list.find_and_remove(nextNode);
                open_list.push(nextNode);
                distance[nextCoord.y * width + nextCoord.x] = nextNode.g;
                parent[nextCoord.x + nextCoord.y * width] = currentCoord;
                map.tile(nextCoord.x, nextCoord.y).setType(QUEUED);
            }
        }
    }

    Map::Point currentCoord = end;
    while (true) {
        map.tile(currentCoord.x, currentCoord.y).setType(PATH);
        if (currentCoord.x == start.x && currentCoord.y == start.y) break;
        currentCoord = parent[currentCoord.y * width + currentCoord.x];
    }
    delete closed;
    delete distance;
}