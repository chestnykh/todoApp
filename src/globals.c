#include <ncurses.h>
#include <time.h>
#include "day.h"


WINDOW** days = NULL;
Day* daysData = NULL;
int dayDisplayCount;
int dayCount = 0;
int displayStart = 0;
int currentFirstDay = 0;
char *dataFile = NULL;
char *bkgdColor = NULL;
char *txtColor = NULL;
int textColorNumber = COLOR_WHITE;
int bkgdColorNumber = COLOR_BLUE;
