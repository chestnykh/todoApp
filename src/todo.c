/* Main file for the todoApp
 * A very basic terminal calendar/todo program
 * License: GPLv2
 */
#include "todo.h"

int main(int argc, char** argv) {
	int i, j, in, result, optIndex, ret = 0;
	size_t len;

	const struct option longOpts [] = {
		{"background_color", REQUIRED_ARG, NULL, 'b'},
		{"text_color", REQUIRED_ARG, NULL, 't'},
		{"data_file",  REQUIRED_ARG, NULL, 'd'}
	};

	while((result = getopt_long(argc, argv, "b:t:d:", longOpts, &optIndex)) != -1) {
		if (optarg) {
			switch (result) {
				case 'b':
					bkgdColorNumber = parse_color(optarg);
					break;
				case 't':
					textColorNumber = parse_color(optarg);
					break;
				case 'd':
					len = strlen(optarg);
					dataFile = malloc(len);
					memcpy(dataFile, optarg, len);
					break;
			}
		}
	}

	//set the SIGWINCH signal handler
	if (set_sigwinch_handler() == -1)
		fprintf(stderr, "WARNING: Failed to set SIGEINCH handler!\n");

	if ((ret = read_data(&days, &daysData, &dayCount, dataFile)) != 0) {
		printf("Error reading file data: %d\n", ret);
		return ret;
	}

	// Start all the ncurses stuff
	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);
	// ncurses color init routines
	start_color();
	assume_default_colors(textColorNumber, bkgdColorNumber);
	init_pair(1, textColorNumber, bkgdColorNumber);  // 1 is the pair of colors used for text and background
	attron(COLOR_PAIR(1));
	bkgd(COLOR_PAIR(1));
	refresh();

	// Generate windows for all of the rendered days
	draw_interface(days, daysData, dayCount, 0, &dayDisplayCount);

	int toExit;
	while(1){
		toExit = 0;
		in = getch();
		if (keyname(in)[0] == '^') {
			switch (keyname(in)[1]) {
				case 'Q':
				case 'C':
				case 'X':
					toExit = 1;
					break;
				case 'R':
					if ((ret = read_data(&days, &daysData, &dayCount, dataFile)) != 0) {
						toExit = 1;
						ret = -1;
						printf("Error reading file data\n");
						break;
				}
					break;
				case 'S':
					if (currentFirstDay > 0)
						currentFirstDay--;
					break;
				case 'D':
					if (currentFirstDay+dayDisplayCount < dayCount)
						currentFirstDay++;
					break;

			}
			draw_interface(days, daysData, dayCount, currentFirstDay, &dayDisplayCount);
			mvprintw(LINES-1, COLS-1, "", COLOR_PAIR(1));  // Just here to put the cursor in the lower right
		}
		if(toExit == 1)
			break;
	}
	release_memory();

	endwin();
	return ret;
}

int read_data(WINDOW*** days, Day** daysData, int* dayCount, const char *dataFile) {
	char type, line[MAX_LINE_LEN];
	int i, j, k, temp;
	time_t maxDay, minDay, tempTime;
	FILE* data;
	struct tm timeStruct;

	if (!(data = fopen(dataFile ? dataFile : "data.txt", "r")))
		return -1;

	minDay = 31536000000;  // 1000 years after 01-01-1970
	maxDay = 0;
	timeStruct.tm_sec   = 0;
	timeStruct.tm_min   = 0;
	timeStruct.tm_hour  = 0;
	timeStruct.tm_mday  = 1;
	timeStruct.tm_year  = 1900;
	timeStruct.tm_wday  = 0;
	timeStruct.tm_yday  = 0;
	timeStruct.tm_isdst = 0;

	while (fgets(line, MAX_LINE_LEN, data) != NULL) {
		if (strptime(line+2, "%Y-%m-%d", &timeStruct) == NULL)
			return 2;

		tempTime = mktime(&timeStruct);
		if (tempTime < minDay)
			minDay = tempTime;
		if (tempTime > maxDay)
			maxDay = tempTime;
	}
	rewind(data);
	*dayCount = GET_DAY_INDEX(minDay, maxDay)+1;
	if (*dayCount < 1) return 3;

	*daysData = malloc(*dayCount*sizeof(Day));
	*days     = malloc(((*dayCount < 28) ? *dayCount : 28)*sizeof(WINDOW*));
	for (i = 0; i < *dayCount; i++) {
		(*daysData)[i].day        = minDay + i*86400;  // 86400 is the amount of seconds in a day
		(*daysData)[i].eventCount = 0;
		(*daysData)[i].dueCount   = 0;
	}

	while (fgets(line, MAX_LINE_LEN, data) != NULL) {
		line[strlen(line)-1] = '\0';
		if (strptime(line+2, "%Y-%m-%d", &timeStruct) == NULL)
			return 4;

		tempTime = mktime(&timeStruct);
		temp = GET_DAY_INDEX(minDay, tempTime);
		if (line[0] == 'E') {
			ADD_EVENT(events, eventCount);
		} else {
			ADD_EVENT(dues, dueCount);
		}
	}
	fclose(data);

	return 0;
}
