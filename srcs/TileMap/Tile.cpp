//
// Created by jeff2310 on 5/8/18.
//

#include "Tilemap/Tile.h"

TileType Tile::getType() {
    return type;
}
void Tile::setType(TileType type) {
    this->type = type;
}