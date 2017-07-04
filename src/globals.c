#include <ncurses.h>
#include <time.h>
#include "day.h"


WINDOW ***days = NULL;
Day **days_data = NULL;
int day_display_count;
int *day_count = NULL;
int display_start = 0;
char *data_file = NULL;
char *bkgd_color = NULL;
char *txt_color = NULL;
char *calendars_file = NULL;
int current_calendar = 0;
int *current_first_day = NULL;
int text_color_number = COLOR_WHITE;
int bkgd_color_number = COLOR_BLUE;

/*number of files which consists calendars
  1 by default 
 */
int nr_files = 1;
