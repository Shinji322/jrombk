#pragma once

#include <ncurses.h>
#include <stdio.h>
#include <math.h>

#define MAP_HEIGHT 24
#define MAP_WIDTH 80

#define MAP_RADIUS 12


#define NUMBER_OF_TILE_TYPES 1
#define DEFAULT_TILE 0

struct Tile {
    int y;
    int x;
    int type;
    bool partOfMap;
};


extern const char* mapTileTypeToChar[NUMBER_OF_TILE_TYPES];


extern struct Tile map[MAP_HEIGHT][MAP_WIDTH];

//Functions

// main.c

// game.c


// generation.c
void generateMap();

// io.c
void initNcurses();
void printMap();

// network.c


// misc.c
