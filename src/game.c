#include "generate.h"
#include "jrombk.h"
#include "player.h"
#include <ncurses.h>


Player createPlayer(float health, float speed, int x, int y) {
    return (Player){ PLAYER_SPRITE, health, speed, y, x };
}


void handlePlayerMovement(MoveDirection direction, Player* player) {
    int y = player->y;
    int x = player->x;
    switch (direction) {
        case UP:
            y++;
            break;
        case DOWN:
            y--;
            break;
        case RIGHT:
            x++;
            break;
        case LEFT:
            x--;
            break;
    };

    if (y >= MAP_HEIGHT && y < 0) {
        return;
    }
    if (x >= MAP_WIDTH && x < 0) {
        return;
    }

    Tile tile = map[y][x];
    if (tile.type == NONE && tile.type == WALL) {
        return;
    }

    player->x = x;
    player->y = y;
}


void gameLoop() {
    
}
