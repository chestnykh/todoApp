#include <ncurses.h>
#include <time.h>
#include "day.h"


WINDOW **days = NULL;
Day *days_data = NULL;
int day_display_count;
int day_count = 0;
int display_start = 0;
int current_first_day = 0;
char *data_file = NULL;
char *bkgd_color = NULL;
char *txt_color = NULL;
int text_color_number = COLOR_WHITE;
int bkgd_color_number = COLOR_BLUE;
