#include "generate.h"
#include "jrombk.h"


const int tileTypeToColor[NUMBER_OF_TILE_TYPES] = {
    COLOR_BLACK,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_YELLOW,
    COLOR_RED,
    COLOR_WHITE
};

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
            attron(COLOR_PAIR(tileTypeToColor[map[y][x].type]));
            mvprintw(y, x, mapTileTypeToChar[map[y][x].type]);
        }
    }
    attrset(A_NORMAL);
}

void printPlayer(Player* player) {
    attron(COLOR_PAIR(player->color));
    mvprintw(player->y, player->x, "@"); 
    attrset(A_NORMAL);
}

void printScreen() {
    printMap();

    printPlayer(&player1); 
    printPlayer(&player2);
}
