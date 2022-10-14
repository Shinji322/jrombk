#include "jrombk.h"
#include <curses.h>


const char* mapTileTypeToChar[NUMBER_OF_TILE_TYPES] = {
    "#",
};


struct Tile map[MAP_HEIGHT][MAP_WIDTH];

int main() {

    initNcurses();

    generateMap();

    printMap();

    while (getch() != 'q') {
        mvprintw(0,0, "hi");
    }

    endwin();
}
