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
#include "day.h"

#define REQUIRED_ARG 1

#define DAY_WIDTH       18
#define MAX_LINE_LEN    512
#define EVENT_START_POS 15

#define GET_DAY_INDEX(b, d)             (d-b)/86400  /*86400 is the amount of seconds in a day*/
#define GET_LAST_EVENT(arr, ind, t, tc) arr[ind].t[arr[ind].tc]
#define ADD_EVENT(en, ecn)              GET_LAST_EVENT((*days_data), temp, en, ecn) = malloc(strlen(line+EVENT_START_POS)); \
                                        for (j = EVENT_START_POS, k = 0; \
                                             line[j] != '\0'; \
                                             GET_LAST_EVENT((*days_data), temp, en, ecn)[k++] = line[j++]); \
                                        GET_LAST_EVENT((*days_data), temp, en, ecn)[k] = '\0'; \
                                        (*days_data)[temp].ecn++

#define nc_print_centered(w, ww, l, s) mvwprintw(w, l, (ww-strlen(s))/2, s, COLOR_PAIR(1))


extern WINDOW** days;
extern Day* days_data;
extern int day_display_count;
extern int day_count;
extern int display_start;
extern int current_first_day;
extern char *data_file;
extern char *bkgd_color;
extern char *txt_color;
extern int text_color_number;
extern int bkgd_color_number;


int  set_sigwinch_handler();
int  parse_color(const char *color_name);
int  read_data(WINDOW*** days, Day** day_data, int* day_count, const char *data_file);
void draw_interface(WINDOW** days, Day* day_data, int day_count, int display_start, int* day_display_count);
void print_event(WINDOW* day, char* event_data, int* current_line);

void release_memory();


#endif /*TODO_H*/
