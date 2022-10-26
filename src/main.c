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
    ClientConnection connection;

    if (strcmp(argv[1], "-S") == 0) {
        initServer(&server);
        isServer = true;
    } else if (strcmp(argv[1], "-C") == 0) {
        if (argc != 3) {
            printf("Invalid Arguments\n");
            return 1;
        }
        initClient(&connection, argv[2]);
        isServer = false;
    } else {
        return 1;
    }

    // Init network and wait for connect

    createPlayer(&player1, 100, COLOR_RED, MAP_WIDTH/2 - MAP_RADIUS, MAP_HEIGHT / 2);
    createPlayer(&player2, 100, COLOR_BLUE, MAP_WIDTH/2 + MAP_RADIUS, MAP_HEIGHT / 2);

    //initNcurses();

    srand(time(NULL));

    generateMap();


    //int command0 = 0;
    int command1 = 0;
    int command2 = 0;
    int lastCommand1 = 0;
    int lastCommand2 = 0;
    bool waiting1 = false;
    bool waiting2 = false;
    int test = 0;
    if (isServer) {
        while (1){//command0 != 'q') {
            frameStart = clock();
            //command0 = getch();
           
            command1 = networkGetch(&server, true);
            command2 = networkGetch(&server, false);
            
            printf("%d",command1);
            fflush(stdout);
            
            //input looks like this
            //126 represents key pressed
            //-1 comes the cycle after you press a key (not on release)
            //0 no key

            //1260-1260-10260-1260-10260-1260260-1260259-100000000000000000000000259-1010000000259-100000000260-10000000000000000000000000000000000000000
            if(command1 != 0){
               if(command1 == -1){
                    waiting1 = false;
                    //lastCommand1 = 0;
               }
               else{
                    waiting1 = true;
               }
            }
            if(command2 != 0){
               if(command2 == -1){
                    waiting2 = false;
                    lastCommand2 = 0;
               }
               else{
                    waiting2 = true;
               }
            }

            if(waiting1){
                lastCommand1 = command1;
            }
            if(waiting2){
                lastCommand2 = command2;
            }

            //if (command0 == 'r') {
            //    generateMap();
            //}

            if(test > 40){
                gameLoop(lastCommand1, lastCommand2);
                printScreen(false);
                //printScreenCharArray();

                sendServerData(&server, true);
                sendServerData(&server, false);

                //mvprintw(0, 0, "%i", command1);
                //mvprintw(1, 0, "%i", command2);
                test = 0;
            }
            else{
                test++;
            }

            clock_t frameLen = 0;
            do {
                frameLen = clock() - frameStart; 
            } while (frameLen < MS_PER_FRAME);
        }
        closeServer(&server);
    } else {
        initNcurses();
        bool moved = false;
        while (command1 != 'q') {
            frameStart = clock();
            command1 = getch();
          
            if (command1 != ERR) {
                clientPut(&connection, command1);
                moved = true;
                //mvprintw(0,0,"%i", command1);
            }
            else{
                if(moved){
                    moved = false;
                    clientPut(&connection, -1); 
                }
            }
           
            if (receiveServerData(&connection) != -1) {
                printScreenCharArray();
            }
            clock_t frameLen = 0;
            do {
                frameLen = clock() - frameStart; 
            } while (frameLen < MS_PER_FRAME);
        }
        closeClient(&connection);
    }

    endwin();
}
