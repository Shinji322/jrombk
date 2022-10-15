#include "jrombk.h"
#define SPRITE '#'

typedef struct Player {
    char sprite;
    float health;
    float speed;
} Player;


Player createPlayer(float health, float speed) {
    return (Player){ SPRITE, health, speed };
}
