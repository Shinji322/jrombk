#include "generate.h"
#include "jrombk.h"

const char* mapTileTypeToChar[NUMBER_OF_TILE_TYPES] = {
    " ",
    ".",
    "~",
    "^",
    "-",
    "+",
    "#",
};


struct Tile map[MAP_HEIGHT][MAP_WIDTH];

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
}
