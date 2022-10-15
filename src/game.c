#include "generate.h"
#include "jrombk.h"
#include "player.h"
#include <time.h>
#include <ncurses.h>


void movePlayer(MoveDirection direction, Player* player) {
    int y = player->y;
    int x = player->x;
    switch (direction) {
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        case RIGHT:
            x++;
            break;
        case LEFT:
            x--;
            break;
    };

    if (y >= MAP_HEIGHT || y < 0) {
        return;
    }
    if (x >= MAP_WIDTH || x < 0) {
        return;
    }

    Tile tile = map[y][x];
    if (tile.type == NONE || tile.type == WALL) {
        return;
    }
    if (tile.hasPlayer) {
        return;
    }

    map[player->y][player->x].hasPlayer = 0;
    player->x = x;
    player->y = y;
    tile.hasPlayer = 1;
}


void dropBomb(Player* player) {
    int x = player->x;
    int y = player->y;
    Tile tile = map[y][x];
}


void handlePlayerInput(int cmd, Player* player) {
    switch (cmd) {
        case KEY_UP:
            movePlayer(UP, player);
            break;
        case KEY_DOWN:
            movePlayer(DOWN, player);
            break;
        case KEY_LEFT:
            movePlayer(LEFT, player);
            break;
        case KEY_RIGHT:
            movePlayer(RIGHT, player);
            break;
        case 'b':
            dropBomb(player);
            break;
        // case 'w':
        //     placeWall(player);
        //     break;
    }
}


void gameLoop(int input1, int input2) {
    handlePlayerInput(input1, &player1);
    handlePlayerInput(input2, &player2);
}
