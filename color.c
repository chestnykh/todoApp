#include <string.h>
#include <ncurses.h>
#include <stdlib.h>

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
