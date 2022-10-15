#include "jrombk.h"

int main() {

    createPlayer(&player1, 100, 10.0, COLOR_RED, MAP_WIDTH/2 - MAP_RADIUS, MAP_HEIGHT / 2);
    createPlayer(&player2, 100, 10.0, COLOR_BLUE, MAP_WIDTH/2 + MAP_RADIUS, MAP_HEIGHT / 2);

    initNcurses();

    srand(time(NULL));

    generateMap();


    int command = 0;
    while (command != 'q') {
        command = getch();
        if (command == 'r') {
            generateMap();
        }
        gameLoop(command, 0);
        printScreen();
    }

    endwin();
}
