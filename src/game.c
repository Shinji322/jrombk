#include "jrombk.h"
#include "player.h"
#include <ncurses.h>


Player createPlayer(float health, float speed, int x, int y) {
    return (Player){ PLAYER_SPRITE, health, speed, y, x };
}


void handleMovement(MoveDirection direction, Player* player) {
    switch (direction) {
        case UP:
            player->y += 1;
        case DOWN:
            player->y -= 1;
        case RIGHT:
            player->x += 1;
        case LEFT:
            player->x -= 1;
    };
}
