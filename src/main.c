#include "jrombk.h"
#include <curses.h>

void initColor(){
    // Alow the use of color
    use_default_colors();
    start_color();
    init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
}

// Set up terminal with ncurses library
void initNcurses(){
    // Init curses
    initscr(); 
    // Disable buffering and get one character-at-a-time input
    cbreak(); 
    // Get Special keys
    keypad(stdscr, TRUE);
    // Suppress echoing of typed keys
    noecho();
    // Hide cursor
    curs_set(0);
    initColor();

    nodelay(stdscr, true);
}


void printMap() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map[y][x].partOfMap) {
                mvprintw(y, x, mapTileTypeToChar[map[y][x].type]);
            } else {
                mvprintw(y, x, " "); 
            }
        }
    }
}


int main() {

    initNcurses();


    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map[y][x].y = y;
            map[y][x].x = x;
            map[y][x].type = DEFAULT_TILE;
            
            map[y][x].partOfMap = (sqrt(pow((x - MAP_WIDTH / 2)/2, 2) + pow(y - MAP_HEIGHT/2, 2)) < MAP_RADIUS);
        }
    }

    printMap();

    while (getch() != 'q') {
        mvprintw(0,0, "hi");
    }

    endwin();
}
