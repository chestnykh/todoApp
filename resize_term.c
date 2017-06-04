#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include "todo.h"
#include "color.h"



void sigwinchHandler(int signum){
	releaseMemory();
	endwin();


	if (readData(&days, &daysData, &dayCount, dataFile) != 0)
		fprintf(stderr, "Error reading file data!\n");

	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);
	refresh();

	//ncurses color init routines
	ncursesColorInit();

	drawDays(days, daysData, dayCount, 0, &dayDisplayCount);
	mvprintw(LINES-2, 0, "^X Exit           ^R Reload Data    ^S Back 1 Day     ^D Forward 1 Day  ", COLOR_PAIR(1));
	mvchgat(LINES-1,  0, 2, A_STANDOUT, 0, NULL);
	mvchgat(LINES-1, 18, 2, A_STANDOUT, 0, NULL);
	mvchgat(LINES-1, 36, 2, A_STANDOUT, 0, NULL);
	mvchgat(LINES-1, 54, 2, A_STANDOUT, 0, NULL);
	mvprintw(LINES-1, COLS-1, "", COLOR_PAIR(1));  // Just here to put the cursor in the lower right

}


int setSigwinchHandler(){
	return signal(SIGWINCH, sigwinchHandler) == SIG_ERR ? -1 : 0;
}
