#pragma once
#include "jrombk.h"

#define MAP_HEIGHT 24
#define MAP_WIDTH 80

#define MAP_RADIUS 10


#define NUMBER_OF_TILE_TYPES 7

typedef enum TileType {
    NONE,
    GRASS,
    WATER,
    MOUNTAINS,
    DESERT,
    LAVA,
    WALL
} TileType;

typedef struct Tile {
    int y;
    int x;
    TileType type;
} Tile;

extern const char* mapTileTypeToChar[NUMBER_OF_TILE_TYPES];

extern struct Tile map[MAP_HEIGHT][MAP_WIDTH];

extern void generateMap();
