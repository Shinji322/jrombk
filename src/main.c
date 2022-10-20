#include "jrombk.h"
#include "misc.h"
#include "network.h"
#include <curses.h>

#define MS_PER_FRAME (CLOCKS_PER_SEC / 60)


clock_t frameStart;

bool debug = false;

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Invalid Arguments\n");
        return 1;
    }

    bool isServer = true;
    ServerConnection server;
    ClientConnection connection;

    if (strcmp(argv[1], "-S") == 0) {
        initServer(&server);
        //initServer(&server2);
        isServer = true;
    } else if (strcmp(argv[1], "-C") == 0) {
        initClient(&connection, argv[2]);
        isServer = false;
    } else {
        return 1;
    }

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-D") == 0) {
            debug = true;
        }
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
            internalLog(SERVER_LOG, "Frame is starting on cycle:%ld\n", frameStart);
            command0 = getch();
            internalLog(SERVER_LOG,"Server Command:%i\n", command0);
            

            internalLog(SERVER_LOG,"Recive data from player 1\n");
            command1 = networkGetch(&server, true);
            internalLog(SERVER_LOG,"Player 1 Command:%i\n", command1);
            internalLog(SERVER_LOG,"Recive data from player 2\n");
            command2 = networkGetch(&server, false);
            internalLog(SERVER_LOG,"Player 2 Command:%i\n", command2);

            if (command0 == 'r') {
                internalLog(SERVER_LOG,"Re-gen map\n");
                generateMap();
            }

            internalLog(SERVER_LOG,"Run gameLoop\n");
            gameLoop(command1, command2);
            internalLog(SERVER_LOG,"Write map to charArray\n");
            printScreen(false);
            internalLog(SERVER_LOG,"Print charArray\n");
            printScreenCharArray();

            internalLog(SERVER_LOG,"Send data to player 1");
            sendServerData(&server, true);
            internalLog(SERVER_LOG,"Send data to player 1");
            sendServerData(&server, false);

            //mvprintw(0, 0, "%i", command1);
            //mvprintw(1, 0, "%i", command0);

            internalLog(SERVER_LOG,"Waiting for frame end \n");
            clock_t frameLen = 0;
            do {
                frameLen = clock() - frameStart; 
            } while (frameLen < MS_PER_FRAME);
        }
        closeServer(&server);
    } else {
        while (command1 != 'q') {
            frameStart = clock();
            internalLog(CLIENT_LOG,"Frame is starting on cycle:%ld\n", frameStart);
            //do {
            //    command1 = getch();
            //} while (command1 == ERR);
            command1 = getch();
            internalLog(CLIENT_LOG,"Player Command:%i\n", command1);
          
            if (command1 != ERR) {
                internalLog(CLIENT_LOG,"Send command to server\n");
                clientPut(&connection, command1);
            }
           
            internalLog(CLIENT_LOG,"Try and receive data from server\n");
            if (receiveServerData(&connection) != -1) {
                internalLog(CLIENT_LOG,"Print out screen\n");
                printScreenCharArray();
            }

            internalLog(CLIENT_LOG,"Waiting for frame end \n");
            clock_t frameLen = 0;
            do {
                frameLen = clock() - frameStart; 
            } while (frameLen < MS_PER_FRAME);
        }
        closeClient(&connection);
    }

    endwin();
}
