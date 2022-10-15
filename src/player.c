#include "player.h"

Player player1;
Player player2;


void createPlayer(Player *player, float health, int color, int x, int y) {
    player->health = health;
    player->sprite = PLAYER_SPRITE;
    player->color = color;
    player->x = x;
    player->y = y;
    map[y][x].hasPlayer = 1;
    //return (Player){ PLAYER_SPRITE, health, speed, color, y, x };
}
