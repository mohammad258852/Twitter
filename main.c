#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include "mymenu.h"

void onexit(){
    //logout();
    endwin();
}

int main(){
    //init ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    keypad(stdscr,TRUE);
    start_color();
    on_exit(onexit,NULL);
    //end of init
    //colors
    init_pair(1,COLOR_RED,COLOR_BLACK);//error
    init_pair(2,COLOR_BLUE,COLOR_CYAN);
    //end of colors
    start_menu();

    endwin();
}
