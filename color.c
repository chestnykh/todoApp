#include <string.h>
#include <ncurses.h>
#include <stdlib.h>
#include "todo.h"


int doCompare(const char *colorName){
	if(!strncmp(colorName, "black", 5))
		return COLOR_BLACK;
	if(!strncmp(colorName, "red", 3))
		return COLOR_RED;
	if(!strncmp(colorName, "green", 5))
		return COLOR_GREEN;
	if(!strncmp(colorName, "yellow", 6))
		return COLOR_YELLOW;
	if(!strncmp(colorName, "blue", 4))
		return COLOR_BLUE;
	if(!strncmp(colorName, "magenta", 7))
		return COLOR_MAGENTA;
	if(!strncmp(colorName, "cyan", 4))
		return COLOR_CYAN;
	if(!strncmp(colorName, "white", 5))
		return COLOR_WHITE;

	fprintf(stderr, "Wrong color name \"%s\" ", colorName);
	exit(-1);
}

int getTextColorNum(const char *txtColor){
	return doCompare(txtColor);
}

int getBkgdColorNum(const char *bkgdColor){
	return doCompare(bkgdColor);
}


void ncursesColorInit(){
	start_color();
	assume_default_colors(textColorNumber, bkgdColorNumber);

	/*use number one for pair of colors set by default or by user as cmd option*/
	init_pair(1, textColorNumber, bkgdColorNumber);
	attron(COLOR_PAIR(1));
	bkgd(COLOR_PAIR(1));
	refresh();
}
