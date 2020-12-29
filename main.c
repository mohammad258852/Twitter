#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include "mymenu.h"


int main(){
    //init ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr,TRUE);
    //end of init

    start_menu();

    endwin();
}
