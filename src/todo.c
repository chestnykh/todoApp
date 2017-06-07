/* Main file for the todoApp
 * A very basic terminal calendar/todo program
 * License: GPLv2
 */
#include "todo.h"

int main(int argc, char** argv) {
	int in, result, opt_index, ret = 0;
	size_t len;

	const struct option long_opts [] = {
		{"background_color", REQUIRED_ARG, NULL, 'b'},
		{"text_color", REQUIRED_ARG, NULL, 't'},
		{"data_file",  REQUIRED_ARG, NULL, 'd'}
	};

	while((result = getopt_long(argc, argv, "b:t:d:", long_opts, &opt_index)) != -1) {
		if (optarg) {
			switch (result) {
				case 'b':
					bkgd_color_number = parse_color(optarg);
					break;
				case 't':
					text_color_number = parse_color(optarg);
					break;
				case 'd':
					len = strlen(optarg);
					data_file = malloc(len);
					memcpy(data_file, optarg, len);
					break;
			}
		}
	}

	/*set the SIGWINCH signal handler*/
	if (set_sigwinch_handler() == -1)
		fprintf(stderr, "WARNING: Failed to set SIGEINCH handler!\n");

	if ((ret = read_data(&days, &days_data, &day_count, data_file)) != 0) {
		printf("Error reading file data: %d\n", ret);
		return ret;
	}

	/*Start all the ncurses stuff*/
	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);
	/*ncurses color init routines*/
	start_color();
	assume_default_colors(text_color_number, bkgd_color_number);
	init_pair(1, text_color_number, bkgd_color_number);  /*1 is the pair of colors used for text and background*/
	attron(COLOR_PAIR(1));
	bkgd(COLOR_PAIR(1));
	refresh();

	/*Generate windows for all of the rendered days*/
	draw_interface(days, days_data, day_count, 0, &day_display_count);

	int to_exit;
	while(1){
		to_exit = 0;
		in = getch();
		if (keyname(in)[0] == '^') {
			switch (keyname(in)[1]) {
				case 'Q':
				case 'C':
				case 'X':
					to_exit = 1;
					break;
				case 'R':
					if ((ret = read_data(&days, &days_data, &day_count, data_file)) != 0) {
						to_exit = 1;
						ret = -1;
						printf("Error reading file data\n");
						break;
				}
					break;
				case 'S':
					if (current_first_day > 0)
						current_first_day--;
					break;
				case 'D':
					if (current_first_day+day_display_count < day_count)
						current_first_day++;
					break;

			}
			draw_interface(days, days_data, day_count, current_first_day, &day_display_count);
			mvprintw(LINES-1, COLS-1, "", COLOR_PAIR(1));  /*Just here to put the cursor in the lower right*/
		}
		if(to_exit == 1)
			break;
	}
	release_memory();

	endwin();
	return ret;
}

int read_data(WINDOW*** days, Day** days_data, int* day_count, const char *data_file) {
	char line[MAX_LINE_LEN];
	int i, j, k, temp;
	time_t max_day, min_day, temp_time;
	FILE* data;
	struct tm time_struct;

	if (!(data = fopen(data_file ? data_file : "data.txt", "r")))
		return -1;

	min_day = 31536000000;  /*1000 years after 01-01-1970*/
	max_day = 0;
	time_struct.tm_sec   = 0;
	time_struct.tm_min   = 0;
	time_struct.tm_hour  = 0;
	time_struct.tm_mday  = 1;
	time_struct.tm_year  = 1900;
	time_struct.tm_wday  = 0;
	time_struct.tm_yday  = 0;
	time_struct.tm_isdst = 0;

	while (fgets(line, MAX_LINE_LEN, data) != NULL) {
		if (strptime(line+2, "%Y-%m-%d", &time_struct) == NULL)
			return 2;

		temp_time = mktime(&time_struct);
		if (temp_time < min_day)
			min_day = temp_time;
		if (temp_time > max_day)
			max_day = temp_time;
	}
	rewind(data);
	*day_count = GET_DAY_INDEX(min_day, max_day)+1;
	if (*day_count < 1) return 3;

	*days_data = malloc(*day_count*sizeof(Day));
	*days     = malloc(((*day_count < 28) ? *day_count : 28)*sizeof(WINDOW*));
	for (i = 0; i < *day_count; i++) {
		(*days_data)[i].day        = min_day + i*86400;  /*86400 is the amount of seconds in a day*/
		(*days_data)[i].event_count = 0;
		(*days_data)[i].due_count   = 0;
	}

	while (fgets(line, MAX_LINE_LEN, data) != NULL) {
		line[strlen(line)-1] = '\0';
		if (strptime(line+2, "%Y-%m-%d", &time_struct) == NULL)
			return 4;

		temp_time = mktime(&time_struct);
		temp = GET_DAY_INDEX(min_day, temp_time);
		if (line[0] == 'E') {
			ADD_EVENT(events, event_count);
		} else {
			ADD_EVENT(dues, due_count);
		}
	}
	fclose(data);

	return 0;
}

void release_memory() {
	int i, j;
	for (i = 0; i < day_count; i++) {
		for (j = 0; j < days_data[i].event_count; j++)
			free(days_data[i].events[j]);
				for (j = 0; j < days_data[i].due_count; j++)
					free(days_data[i].dues[j]);
	}
	free(days);
	free(days_data);
}
