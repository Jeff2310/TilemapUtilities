//
// Created by jeff2310 on 5/8/18.
//

#include "Tilemap/Map.h"
#include <fstream>
#include <FreeImage.h>

Map::Map(): tiles(){
    tiles = nullptr;
}

Map::Map(const Map &m): height(m.height), width(m.width){
    tiles = new Tile[m.width * m.height];
    // copy tiles
    for(Coordinate y=0; y<height; y++)
        for(Coordinate x=0; x<width; x++)
            tiles[y*width+x] = m.tiles[y*width+x];
}

Map::Map(Coordinate height, Coordinate width): height(height), width(width){
    tiles = new Tile[width * height];
}

Map::~Map(){
    delete[] tiles;
}

Map& Map::operator=(const Map &m) {
    tiles = new Tile[m.width * m.height];
    width = m.width;
    height = m.height;
    // copy tiles
    for(Coordinate y=0; y<height; y++)
        for(Coordinate x=0; x<width; x++)
            tiles[y*width+x] = m.tiles[y*width+x];
    return *this;
}

Tile& Map::tile(Coordinate x, Coordinate y) const{
    if (x < 0) x = 0;
    if (x > width) x = width;
    if (y < 0) y = 0;
    if (y > height) y = height;
    return tiles[y*width + x];
}

void Map::dump() const{
//    std::ofstream file("map.txt");
//    for(Coordinate y=0; y<height; y++) {
//        for (Coordinate x = 0; x <width; x++) {
//            if(tile(x, y).type == WALL)
//                file<<'#';
//            else
//                file<<'.';
//        }
//        file<<std::endl;
//    }
//    file.close();
    FreeImage_Initialise();
    FIBITMAP *bitmap = FreeImage_AllocateT(FIT_BITMAP, width, height);
    BYTE *bits;
    int bytespp = FreeImage_GetLine(bitmap) / FreeImage_GetWidth(bitmap);
    for(int y=0; y<height; y++){
        bits = FreeImage_GetScanLine(bitmap, y);
        for(int x=0; x<width; x++){
            BYTE color;
            switch(tile(x,y).type){
                case WALL:
                    color = 125;
                    break;
                case FLOOR:
                    color = 255;
                    break;
                case PATH:
                    color = 80;
                    break;
                case QUEUED:
                    color = 160;
                    break;
                case CORRIDOR:
                    color = 255;
                    break;
                default:
                    color = 60;
                    break;
            }
            bits[FI_RGBA_RED] = bits[FI_RGBA_BLUE] = bits[FI_RGBA_GREEN] = color;
            bits+=bytespp;
        }
    }
    FreeImage_Save(FIF_PNG, bitmap, "map.png", 0);
    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();
}