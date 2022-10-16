#pragma once
#include "jrombk.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


char* compress(char* string, int length);
void decompress(char* string, int inLen, char* outString, int outLen);
