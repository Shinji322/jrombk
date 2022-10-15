#include "generate.h"
#include "jrombk.h"
#include <stdlib.h>

const char* mapTileTypeToChar[NUMBER_OF_TILE_TYPES] = {
    " ",
    ".",
    "~",
    "^",
    "-",
    "+",
    "#",
};


Tile map[MAP_HEIGHT][MAP_WIDTH];

void placeCircle(int centerY, int centerX, int radius, int type) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            double currentDistanceFromCenter = sqrt(pow((x - centerX)/2, 2) + pow(y - centerY, 2));
            if (map[y][x].type != NONE && map[y][x].type != WALL && currentDistanceFromCenter <= radius) { 
                map[y][x].type = type;
            }
        }
    }
}

void generateMap() {

    // Init Defaults
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map[y][x].y = y;
            map[y][x].x = x;
           
            double currentDistanceFromCenter = sqrt(pow((x - MAP_WIDTH / 2)/2, 2) + pow(y - MAP_HEIGHT/2, 2));
            if (ceil(currentDistanceFromCenter) == MAP_RADIUS) {
                map[y][x].type = WALL;
            } else if (currentDistanceFromCenter < MAP_RADIUS) { 
                map[y][x].type = GRASS;
            } else {
                map[y][x].type = NONE;
            }
        }
    }

    for (int i = 0; i < 100; i++) {
       placeCircle(rand() % MAP_HEIGHT, rand() % MAP_WIDTH, rand() % 4 + 2, 1 + rand() % (NUMBER_OF_TILE_TYPES - 2)); 
    }


}
