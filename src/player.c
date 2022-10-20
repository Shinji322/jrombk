#include "player.h"

Player player1;
Player player2;


void createPlayer(Player *player, int lives, int color, int x, int y) {
    player->lives = lives;
    player->sprite = PLAYER_SPRITE;
    player->color = color;
    player->x = x;
    player->y = y;
    map[y][x].hasPlayer = 1;

    player->bombs[0] = (Bomb) {0,0,0,0,0,0,0,0};
    player->bombs[1] = (Bomb) {0,0,0,0,0,0,0,0};
    player->bombs[2] = (Bomb) {0,0,0,0,0,0,0,0};
}
