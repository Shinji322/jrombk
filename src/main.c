#include "io.h"
#include "jrombk.h"
#include "misc.h"
#include "network.h"
#include <asm-generic/errno-base.h>
#include <curses.h>
#include <errno.h>
#include <stdio.h>

#include "inputX11.h"

#define MS_PER_FRAME (CLOCKS_PER_SEC / 60)

clock_t frameStart;

bool debug = false;

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("Invalid Arguments\n");
    return 1;
  }
  // sigaction(SIGPIPE, &(struct sigaction){SIG_IGN}, NULL);

  bool isServer = true;
  ServerConnection server;
  ClientConnection connection;

  if (strcmp(argv[1], "-S") == 0) {
    initServer(&server);
    // initServer(&server2);
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

  createPlayer(&player1, 100, COLOR_RED, MAP_WIDTH / 2 - MAP_RADIUS,
               MAP_HEIGHT / 2);
  createPlayer(&player2, 100, COLOR_BLUE, MAP_WIDTH / 2 + MAP_RADIUS,
               MAP_HEIGHT / 2);

  // initNcurses();

  srand(time(NULL));

  generateMap();

  int command0 = 0;
  int command1 = 0;
  int command2 = 0;
  int lastCommand1 = 0;
  int lastCommand2 = 0;
  bool waiting1 = false;
  bool waiting2 = false;
  int timer = 0;

  if (isServer) {
    while (1) { // command0 != 'q') {
      frameStart = clock();
      internalLog(SERVER_LOG, "Frame is starting on cycle:%ld\n", frameStart);
      // command0 = getch();
      internalLog(SERVER_LOG, "Server Command:%i\n", command0);

      internalLog(SERVER_LOG, "Recive data from player 1\n");
      command1 = networkGetch(&server, true);
      internalLog(SERVER_LOG, "Player 1 Command:%i\n", command1);
      internalLog(SERVER_LOG, "Recive data from player 2\n");
      command2 = networkGetch(&server, false);
      internalLog(SERVER_LOG, "Player 2 Command:%i\n", command2);

      // if (command0 == 'r') {
      //   internalLog(SERVER_LOG, "Re-gen map\n");
      //   generateMap();
      // }

      internalLog(SERVER_LOG, "Run gameLoop\n");

      printf("P1%d\n", command1);
      if (command1 != 0) {
        // printf("P1: %d\n", command1);
        if (command1 == -999) {
          waiting1 = false;
          lastCommand1 = 0;
        } else {
          waiting1 = true;
        }
      }
      if (command2 != 0) {
        if (command2 == -999) {
          waiting2 = false;
          lastCommand2 = 0;
        } else {
          waiting2 = true;
        }
      }

      if (waiting1 && command1 != 0) {
        lastCommand1 = command1;
      }
      if (waiting2 && command2 != 0) {
        lastCommand2 = command2;
      }

      if (timer > 0) {
        // handlePlayerInput(command1, &player1);
        gameLoop(lastCommand1, lastCommand2);
        timer = 0;
      } else {
        timer++;
      }
      internalLog(SERVER_LOG, "Write map to charArray\n");
      printScreen(false);
      internalLog(SERVER_LOG, "Print charArray\n");
      // printScreenCharArray();

      internalLog(SERVER_LOG, "Send data to player 1");
      sendServerData(&server, true);
      internalLog(SERVER_LOG, "Send data to player 1");
      sendServerData(&server, false);

      if (errno == ECONNRESET || errno == EPIPE) {
        break;
      }

      internalLog(SERVER_LOG, "Waiting for frame end \n");
      clock_t frameLen = 0;
      do {
        frameLen = clock() - frameStart;
      } while (frameLen < MS_PER_FRAME);
    }
    closeServer(&server);
  } else {
    initNcurses();
    initX11();
    while (command1 != 113) {
      frameStart = clock();
      int temp1 = getch();
      int temp = processX11Input();
      command1 = temp;
      // printf("aaa%d\n", command1);
      //  printf("comm1: %d\n", command1);
      //  }
      //   getch();
      //     printf("Key: %s\n", key);

      internalLog(CLIENT_LOG, "Frame is starting on cycle:%ld\n", frameStart);
      //  do {
      //      command1 = getch();
      //  } while (command1 == ERR);
      // command1 = getch();
      internalLog(CLIENT_LOG, "Player Command:%i\n", command1);

      if (command1 != ERR && command1 != 0) {
        // internalLog(CLIENT_LOG, "Send command to server\n");
        // printf("Sending: %d\n", command1);
        clientPut(&connection, command1);
        // printf("Sent\n");
        if (errno == ECONNRESET | errno == EPIPE) {
          break;
        }
      }

      internalLog(CLIENT_LOG, "Try and receive data from server\n");
      if (receiveServerData(&connection) != -1) {
        internalLog(CLIENT_LOG, "Print out screen\n");
        printScreenCharArray();
      }

      internalLog(CLIENT_LOG, "Waiting for frame end \n");
      clock_t frameLen = 0;
      do {
        frameLen = clock() - frameStart;
      } while (frameLen < MS_PER_FRAME);
    }
    closeClient(&connection);
  }
  endwin();
}
