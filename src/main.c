#include "jrombk.h"

#define MS_PER_FRAME (CLOCKS_PER_SEC / 60)

clock_t frameStart;

int main() {

    createPlayer(&player1, 100, COLOR_RED, MAP_WIDTH/2 - MAP_RADIUS, MAP_HEIGHT / 2);
    createPlayer(&player2, 100, COLOR_BLUE, MAP_WIDTH/2 + MAP_RADIUS, MAP_HEIGHT / 2);

    initNcurses();

    srand(time(NULL));

    generateMap();


    int command = 0;
    while (command != 'q') {
        frameStart = clock();
        command = getch();
        if (command == 'r') {
            generateMap();
        }
        gameLoop(command, 0);
        printScreen();

        clock_t frameLen = 0;
        do {
            frameLen = clock() - frameStart; 
        } while (frameLen < MS_PER_FRAME);
    }

    endwin();
}
