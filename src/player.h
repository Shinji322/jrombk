#pragma once
#include "jrombk.h"

#define PLAYER_SPRITE '@'

typedef struct Player {
    char sprite;
    int lives;
    int isDead;
    int color;
    Bomb bombs[3];
    int y;
    int x;
} Player;


typedef enum MoveDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT
} MoveDirection;

extern Player player1;
extern Player player2;

void createPlayer(Player *player, int health, int color, int x, int y);
