#include "jrombk.h"



int main() {

    initNcurses();

    srand(time(NULL));

    generateMap();


    char command = 0;
    while (command != 'q') {
        command = getch();
        if (command == 'r') {
            generateMap();
        }
        printMap();
    }

    endwin();
}
