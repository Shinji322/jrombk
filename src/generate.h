#pragma once
#include "jrombk.h"

#define MAP_HEIGHT 40
#define MAP_WIDTH 120

#define MAP_RADIUS 16

#define MAP_SIZE (MAP_WIDTH * MAP_HEIGHT * 2)


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
    int hasPlayer;
    Bomb* bomb;
    int isWall;
    TileType type;
} Tile;

extern const char* mapTileTypeToChar[NUMBER_OF_TILE_TYPES];

extern struct Tile map[MAP_HEIGHT][MAP_WIDTH];

extern void generateMap();
