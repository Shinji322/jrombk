#include "jrombk.h"
#include <curses.h>



int main() {

    initNcurses();

    generateMap();

    printMap();

    while (getch() != 'q') {
        mvprintw(0,0, "hi");
    }

    endwin();
}
