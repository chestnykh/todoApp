#include "todo.h"
#include <assert.h>


void sigwinch_handler(int signum) {
	/*FIXME: This whole method down to drawInterface() shouldn't be here*/
	

	release_memory();
	endwin();

	if (read_data(&days, &days_data, &day_count, data_file) != 0)
		fprintf(stderr, "Error reading file data!\n");

	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);
	refresh();

	/*ncurses color init routines*/
	start_color();
	assume_default_colors(text_color_number, bkgd_color_number);


	/*A problem is how to choose color which will be emphasize important events if we dont know
	 what is the background color. 
	 */
	init_pair(1, text_color_number, bkgd_color_number);  /*1 is the pair of colors used for text and background*/
	init_pair(2, COLOR_CYAN, bkgd_color_number);
	init_pair(3, COLOR_YELLOW, bkgd_color_number);
	init_pair(4, COLOR_MAGENTA, bkgd_color_number);
	init_pair(5, COLOR_RED, bkgd_color_number);
	attron(COLOR_PAIR(1));
	bkgd(COLOR_PAIR(1));
	refresh();

	draw_interface(days, days_data, day_count, 0, &day_display_count);
}


int set_sigwinch_handler() {
	return signal(SIGWINCH, sigwinch_handler) == SIG_ERR ? -1 : 0;
}

int parse_color(const char *color_name) {
	if(!strncmp(color_name, "black", 5))
		return COLOR_BLACK;
	if(!strncmp(color_name, "red", 3))
		return COLOR_RED;
	if(!strncmp(color_name, "green", 5))
		return COLOR_GREEN;
	if(!strncmp(color_name, "yellow", 6))
		return COLOR_YELLOW;
	if(!strncmp(color_name, "blue", 4))
		return COLOR_BLUE;
	if(!strncmp(color_name, "magenta", 7))
		return COLOR_MAGENTA;
	if(!strncmp(color_name, "cyan", 4))
		return COLOR_CYAN;
	if(!strncmp(color_name, "white", 5))
		return COLOR_WHITE;

	fprintf(stderr, "Wrong color name \"%s\" ", color_name);
	exit(-1);
}

void draw_interface(WINDOW **days, Day *days_data, int day_count, int display_start, int *day_display_count) {
	int i, j, current_line;
	char *temp;
	struct tm *time_struct;
	int cp;

	*day_display_count = COLS/DAY_WIDTH;
	if (*day_display_count > 28)
		*day_display_count = 28;

	for (i = 0; i < *day_display_count; i++) {
		days[i] = newwin(LINES-1, DAY_WIDTH, 0, i*DAY_WIDTH);
		/*wbkgd(days[i], COLOR_PAIR(1));*/
		box(days[i], 0, 0);
		/*wattron(days[i], COLOR_PAIR(1));*/

		/*The part which shows day of the week and date*/
		temp = malloc(DAY_WIDTH-2);
		current_line = 1;
		time_struct = localtime(&(days_data[i+display_start].day));
		strftime(temp, DAY_WIDTH-2, "%A", time_struct);
		nc_print_centered(days[i], DAY_WIDTH, current_line, temp);
		mvwchgat(days[i], current_line++, 1, DAY_WIDTH-2, A_BOLD, 0, NULL);
		strftime(temp, DAY_WIDTH-2, "%d %b %Y", time_struct);
		nc_print_centered(days[i], DAY_WIDTH, current_line++, temp);
		mvwhline(days[i], current_line, 0, ACS_LTEE, 1);
		mvwhline(days[i], current_line, 1, ACS_HLINE, DAY_WIDTH-2);
		mvwhline(days[i], current_line, DAY_WIDTH-1, ACS_RTEE, 1);
		current_line++;
		free(temp);

		/*Events for the day*/
		temp = "Events:";
		mvwprintw(days[i], current_line, 1, temp, COLOR_PAIR(1));
		mvwchgat(days[i], current_line++, 1, DAY_WIDTH-2, A_UNDERLINE, 0, NULL);
		for (j = 0; j < days_data[i+display_start].event_count; j++){
			if(days_data[i+display_start].prios[j] < 4){
				print_event(&days[i], days_data[i+display_start].events[j], &current_line, days_data[i+display_start].prios[j] + 2);
				
			}
			else{
				print_event(&days[i], days_data[i+display_start].events[j], &current_line, 1);
			}
		}

		/*Things due for the day*/
		current_line = (LINES-7)/2+4;
		temp = "Due:";
		mvwprintw(days[i], current_line, 1, temp, COLOR_PAIR(1));
		mvwchgat(days[i], current_line++, 1, DAY_WIDTH-2, A_UNDERLINE, 0, NULL);

		/*I think that dues highlighting isn't need*/
		for (j = 0; j < days_data[i+display_start].due_count; j++){
				print_event(&days[i], days_data[i+display_start].dues[j], &current_line,1);
		}

		wrefresh(days[i]);
		refresh();

		/*Controls stuff*/
		/*TODO: put this in a window and make it dynamic*/
		mvprintw(LINES-1, 0, "^X Exit           ^R Reload Data    ^S Back 1 Day     ^D Forward 1 Day  ", COLOR_PAIR(1));
		mvchgat(LINES-1,  0, 2, A_STANDOUT, 0, NULL);
		mvchgat(LINES-1, 18, 2, A_STANDOUT, 0, NULL);
		mvchgat(LINES-1, 36, 2, A_STANDOUT, 0, NULL);
		mvchgat(LINES-1, 54, 2, A_STANDOUT, 0, NULL);
		mvprintw(LINES-1, COLS-1, "", COLOR_PAIR(1));  /*Just here to put the cursor in the lower right*/
	}
}

void print_event(WINDOW **day, char *event_data, int *current_line, int prio) {
	int i, j, k;
	char temp[16][256];

	assert(prio < 6 && prio > 0);
	for (i = 0; i < 16; temp[i++][0] = '\0');
	for (i = 0, j = 0, k = 0; event_data[i] != '\0'; i++) {
		if (event_data[i] == ';') {
			temp[j++][k] = '\0';
			k = 0;
		} else {
			temp[j][k++] = event_data[i];
		}
	}
	temp[j][k] = '\0';
	wattron(*day, COLOR_PAIR(prio));
	for (i = 0; temp[i][0] != '\0'; i++)
		mvwprintw(*day, (*current_line)++, 1, temp[i], COLOR_PAIR(prio));
}
