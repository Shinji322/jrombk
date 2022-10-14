#include "jrombk.h"


void generateMap() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map[y][x].y = y;
            map[y][x].x = x;
            map[y][x].type = DEFAULT_TILE;
            
            map[y][x].partOfMap = (sqrt(pow((x - MAP_WIDTH / 2)/2, 2) + pow(y - MAP_HEIGHT/2, 2)) < MAP_RADIUS);
        }
    }
}
