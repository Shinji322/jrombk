#pragma once
#include "jrombk.h"

#define PLAYER_SPRITE '@'

typedef struct Player {
    char sprite;
    float health;
    float speed;
    int color;
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

void createPlayer(Player *player, float health, int color, int x, int y);
