#include "todo.h"
#include <assert.h>


void sigwinch_handler(int signum) {
	// FIXME: This whole method down to drawInterface() shouldn't be here

	release_memory();
	endwin();

	if (read_data(&days, &daysData, &dayCount, dataFile) != 0)
		fprintf(stderr, "Error reading file data!\n");

	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);
	refresh();

	// ncurses color init routines
	start_color();
	assume_default_colors(textColorNumber, bkgdColorNumber);
	init_pair(1, textColorNumber, bkgdColorNumber);  // 1 is the pair of colors used for text and background
	attron(COLOR_PAIR(1));
	bkgd(COLOR_PAIR(1));
	refresh();

	draw_interface(days, daysData, dayCount, 0, &dayDisplayCount);
}


int set_sigwinch_handler() {
	return signal(SIGWINCH, sigwinch_handler) == SIG_ERR ? -1 : 0;
}

int parse_color(const char *colorName) {
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

void draw_interface(WINDOW** days, Day* daysData, int dayCount, int displayStart, int* dayDisplayCount) {
	int i, j, currentLine;
	char *temp;
	struct tm *timeStruct;

	*dayDisplayCount = COLS/DAY_WIDTH;
	if (*dayDisplayCount > 28) *dayDisplayCount = 28;

	for (i = 0; i < *dayDisplayCount; i++) {
		days[i] = newwin(LINES-1, DAY_WIDTH, 0, i*DAY_WIDTH);
		wbkgd(days[i], COLOR_PAIR(1));
		box(days[i], 0, 0);
		wattron(days[i], COLOR_PAIR(1));

		// The part which shows day of the week and date
		temp = malloc(DAY_WIDTH-2);
		currentLine = 1;
		timeStruct = localtime(&(daysData[i+displayStart].day));
		strftime(temp, DAY_WIDTH-2, "%A", timeStruct);
		nc_print_centered(days[i], DAY_WIDTH, currentLine, temp);
		mvwchgat(days[i], currentLine++, 1, DAY_WIDTH-2, A_BOLD, 0, NULL);
		strftime(temp, DAY_WIDTH-2, "%d %b %Y", timeStruct);
		nc_print_centered(days[i], DAY_WIDTH, currentLine++, temp);
		mvwhline(days[i], currentLine, 0, ACS_LTEE, 1);
		mvwhline(days[i], currentLine, 1, ACS_HLINE, DAY_WIDTH-2);
		mvwhline(days[i], currentLine, DAY_WIDTH-1, ACS_RTEE, 1);
		currentLine++;
		free(temp);

		// Events for the day
		temp = "Events:";
		mvwprintw(days[i], currentLine, 1, temp, COLOR_PAIR(1));
		mvwchgat(days[i], currentLine++, 1, DAY_WIDTH-2, A_UNDERLINE, 0, NULL);
		for (j = 0; j < daysData[i+displayStart].eventCount; j++)
			print_event(days[i], daysData[i+displayStart].events[j], &currentLine);

		// Things due for the day
		currentLine = (LINES-7)/2+4;
		temp = "Due:";
		mvwprintw(days[i], currentLine, 1, temp, COLOR_PAIR(1));
		mvwchgat(days[i], currentLine++, 1, DAY_WIDTH-2, A_UNDERLINE, 0, NULL);
		for (j = 0; j < daysData[i+displayStart].dueCount; j++)
			print_event(days[i], daysData[i+displayStart].dues[j], &currentLine);

		wrefresh(days[i]);
		refresh();

		// Controls stuff
		// TODO: put this in a window and make it dynamic
		mvprintw(LINES-1, 0, "^X Exit           ^R Reload Data    ^S Back 1 Day     ^D Forward 1 Day  ", COLOR_PAIR(1));
		mvchgat(LINES-1,  0, 2, A_STANDOUT, 0, NULL);
		mvchgat(LINES-1, 18, 2, A_STANDOUT, 0, NULL);
		mvchgat(LINES-1, 36, 2, A_STANDOUT, 0, NULL);
		mvchgat(LINES-1, 54, 2, A_STANDOUT, 0, NULL);
		mvprintw(LINES-1, COLS-1, "", COLOR_PAIR(1));  // Just here to put the cursor in the lower right
	}
}

void print_event(WINDOW* day, char* eventData, int* currentLine) {
	int i, j, k;
	char temp[16][256];

	wattron(day, COLOR_PAIR(1));
	for (i = 0; i < 16; temp[i++][0] = '\0');
	for (i = 0, j = 0, k = 0; eventData[i] != '\0'; i++) {
		if (eventData[i] == ';') {
			temp[j++][k] = '\0';
			k = 0;
		} else {
			temp[j][k++] = eventData[i];
		}
	}
	temp[j][k] = '\0';
	for (i = 0; temp[i][0] != '\0'; i++)
		mvwprintw(day, (*currentLine)++, 1, temp[i], COLOR_PAIR(1));
}
