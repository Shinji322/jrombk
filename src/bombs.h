#pragma once
#include <time.h>

typedef struct Bomb {
    clock_t startCycles;
    clock_t cyclesToBlow;
    int isNone;
    int isPlaced;
    int y;
    int x;
} Bomb;

