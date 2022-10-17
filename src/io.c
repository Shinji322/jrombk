#include "io.h"
#include <curses.h>
#include <stdarg.h>
//#include <libioP.h>



char screenAsChar[2][MAP_HEIGHT][MAP_WIDTH] = {COLOR_WHITE};

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

void mvprintWrapper(bool toScreen, int y, int x, const char* format, ...) {
    if (toScreen) {
        //mvprintw(y, x, format, );
        va_list ap;
        int ret;

        if (move(y, x) != OK) return;
        va_start(ap, format);
        ret = vw_printw(stdscr, format, ap);
        va_end(ap);
    } 
    else {
        char s[100];
        va_list arg;
        int done;
        va_start (arg, format);
        done = vsprintf(s, format, arg);
        va_end (arg);

        screenAsChar[0][y][x] = s[0];
    }
}


void printMap(bool toScreen) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            //attron(COLOR_PAIR(tileTypeToColor[map[y][x].type]));
            screenAsChar[1][y][x] = tileTypeToColor[map[y][x].type];
            if (map[y][x].isWall) {
                mvprintWrapper(toScreen, y, x, "#");
            } else {
                mvprintWrapper(toScreen, y, x, mapTileTypeToChar[map[y][x].type]);
            }
        }
    }
    //attrset(A_NORMAL);
}

#define NUMBER_OF_BOMB_BLOW_CHARS 9
const char* bombBlowCycle[NUMBER_OF_BOMB_BLOW_CHARS] = {
    "%",
    "%",
    "\\",
    "-",
    "/",
    "|",
    "*",
    "*",
    "*"
};

void printPlayerBombs(Player* player, bool toScreen) {
    for (int i = 0; i < 3; i++) {
        if (player->bombs[i].isPlaced) {
            if (player->bombs[i].isBlowing != 0) {            
                const char* charToPrint = bombBlowCycle[(int) floor(player->bombs[i].isBlowing / ((60) / (NUMBER_OF_BOMB_BLOW_CHARS - 1)))];

                for (int y = 0; y < MAP_HEIGHT; y++) {
                    for (int x = 0; x < MAP_WIDTH; x++) {
                        double currentDistanceFromCenter = sqrt(pow((x - player->bombs[i].x)/2, 2) + pow(y - player->bombs[i].y, 2));
                        if (player->bombs[i].radius >= currentDistanceFromCenter) {
                            mvprintWrapper(toScreen, y, x, "%s", charToPrint);
                        }
                    }
                }
            } else {
                int secondsLeft = floor((player->bombs[i].cyclesToBlow - (frameStart - player->bombs[i].startCycles)) / CLOCKS_PER_SEC); 
                mvprintWrapper(toScreen, player->bombs[i].y, player->bombs[i].x, "%i", secondsLeft);
            }
        }
    }
}

void printPlayer(Player* player, bool toScreen) {
    //attron(COLOR_PAIR(player->color));
    screenAsChar[1][player->y][player->x] = player->color;
    mvprintWrapper(toScreen, player->y, player->x, "%c", player->sprite); 
    //attrset(A_NORMAL);
}

void printScreen(bool toScreen) {
    printMap(toScreen);

    printPlayer(&player1, toScreen); 
    printPlayer(&player2, toScreen);

    printPlayerBombs(&player1, toScreen);
    printPlayerBombs(&player2, toScreen);
}

void printScreenCharArray() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            attron(COLOR_PAIR(screenAsChar[1][y][x]));
            mvprintw(y,x, "%c", screenAsChar[0][y][x]);
            attrset(A_NORMAL);
        }
    }
}

char* getScreenArray() {
    return &screenAsChar[0][0][0];
}
