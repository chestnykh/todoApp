#ifndef TODO_H
#define TODO_H

/* Main header file for the todoApp
 * A very basic terminal calendar/todo program
 * License: GPLv2
 */
#include <getopt.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#define REQUIRED_ARG 1

#define DAY_WIDTH       18
#define MAX_EVENTS      128
#define MAX_LINE_LEN    512
#define EVENT_START_POS 15

#define GET_DAY_INDEX(b, d)             (d-b)/86400  // 86400 is the amount of seconds in a day
#define GET_LAST_EVENT(arr, ind, t, tc) arr[ind].t[arr[ind].tc]
#define ADD_EVENT(en, ecn)              GET_LAST_EVENT((*daysData), temp, en, ecn) = malloc(strlen(line+EVENT_START_POS)); \
                                        for (j = EVENT_START_POS, k = 0; \
                                             line[j] != '\0'; \
                                             GET_LAST_EVENT((*daysData), temp, en, ecn)[k++] = line[j++]); \
                                        GET_LAST_EVENT((*daysData), temp, en, ecn)[k] = '\0'; \
                                        (*daysData)[temp].ecn++

#define ncPrint_centered(w, ww, l, s) mvwprintw(w, l, (ww-strlen(s))/2, s, COLOR_PAIR(1))

typedef struct {
  time_t day;
  int    eventCount;
  int    dueCount;
  char*  events[MAX_EVENTS];
  char*  dues[MAX_EVENTS];
} Day;


static WINDOW** days;
static Day* daysData;
static int dayDisplayCount;
static int dayCount = 0;
static int currentFirstDay = 0;
static char *dataFile = NULL;
static char *bkgdColor = NULL;
static char *txtColor = NULL;
static int textColorNumber = COLOR_WHITE;
static int bkgdColorNumber = COLOR_BLUE;


int  setSigwinchHandler();
int  parseColor(const char *colorName);
int  readData(WINDOW*** days, Day** dayData, int* dayCount, const char *dataFile);
void drawInterface(WINDOW** days, Day* dayData, int dayCount, int displayStart, int* dayDisplayCount);
void printEvent(WINDOW* day, char* eventData, int* currentLine);


static inline void releaseMemory() {
	int i, j;
	for (i = 0; i < dayCount; i++) {
		for (j = 0; j < daysData[i].eventCount; j++)
			free(daysData[i].events[j]);
				for (j = 0; j < daysData[i].dueCount; j++)
					free(daysData[i].dues[j]);
	}
	free(days);
	free(daysData);
}

#endif /*TODO_H*/
