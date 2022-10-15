#include "jrombk.h"
#include "network.h"
#include <curses.h>

#define MS_PER_FRAME (CLOCKS_PER_SEC / 60)

clock_t frameStart;

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Invalid Arguments\n");
        return 1;
    }

    bool isServer = true;
    ServerConnection server;
    ClientConnection connection1;

    if (strcmp(argv[1], "-S") == 0) {
        initServer(&server);
        isServer = true;
    } else if (strcmp(argv[1], "-C") == 0) {
        initClient(&connection1, argv[2]);
        isServer = false;
    } else {
        return 1;
    }

    // Init network and wait for connect

    createPlayer(&player1, 100, COLOR_RED, MAP_WIDTH/2 - MAP_RADIUS, MAP_HEIGHT / 2);
    createPlayer(&player2, 100, COLOR_BLUE, MAP_WIDTH/2 + MAP_RADIUS, MAP_HEIGHT / 2);

    initNcurses();

    srand(time(NULL));

    generateMap();


    int command0 = 0;
    int command1 = 0;
    int command2 = 0;
    if (isServer) {
        while (command0 != 'q') {
            frameStart = clock();
            command0 = getch();

            command1 = networkGetch(&server);

            if (command0 == 'r') {
                generateMap();
            }

            gameLoop(command1, command2);
            printScreen();
            mvprintw(1, 0, "%i", command1);

            clock_t frameLen = 0;
            do {
                frameLen = clock() - frameStart; 
            } while (frameLen < MS_PER_FRAME);
        }
        closeServer(&server);
    } else {
        while (command1 != 'q') {
            do {
                command1 = getch();
            } while (command1 == ERR);
           
            clientPut(&connection1, command1);
        }
        closeClient(&connection1);
    }

    endwin();
}
