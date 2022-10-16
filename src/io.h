#pragma once
#include "jrombk.h"
//#include "generate.h"
//#include "player.h"

void initNcurses();

void mvprintWrapper(bool toScreen, int y, int x, const char* format, ...);

void printScreen(bool toScreen);
void printScreenCharArray();

char* getScreenArray();

void printTitle();
