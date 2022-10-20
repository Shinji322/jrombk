#include "misc.h"

void internalLog(const char* fileName, const char* format, ...) {
    if (debug) {
        va_list args;
        va_start(args, format);

        //vprintf(format, args);

        FILE *fptr;

        fptr = fopen(fileName, "a");

        vfprintf(fptr, format, args);

        fclose(fptr);


        va_end(args);
    }
}
