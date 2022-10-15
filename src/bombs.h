#pragma once
#include <time.h>

typedef struct Bomb {
    clock_t startCycles;
    clock_t cyclesToBlow;
    int isNone;
    int isPlaced;
    int isBlowing;
    int radius;
    int y;
    int x;
} Bomb;

extern Bomb noneBomb;
