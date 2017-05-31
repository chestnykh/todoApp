/* Main header file for the todoApp
 * A very basic terminal calendar/todo program
 * License: GPLv2
 *
 * Initial version by Wesley Van Pelt in May 2017
 */

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

#define ncPrint_centered(w, ww, l, s) mvwprintw(w, l, (ww-strlen(s))/2, s)

typedef struct {
  time_t day;
  int    eventCount;
  int    dueCount;
  char*  events[MAX_EVENTS];
  char*  dues[MAX_EVENTS];
} Day;

int  readData(WINDOW*** days, Day** dayData, int* dayCount);
void drawDays(WINDOW** days, Day* dayData, int dayCount, int displayStart, int* dayDisplayCount);
void printEvent(WINDOW* day, char* eventData, int* currentLine);
