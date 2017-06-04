/* Main file for the todoApp
 * A very basic terminal calendar/todo program
 * License: GPLv2
 *
 * Initial version by Wesley Van Pelt in May 2017
 */

#include "./todo.h"
#include "color.h"
#include "resize_term.h"
#include <getopt.h>



//for getopt_long()
#define REQUIRED_ARG 1


int main(int argc, char** argv) {
	int i, j, in, ret = 0;
	
	int result, optIndex;	

	const struct option longOpts [] = {
		{"background_color", REQUIRED_ARG, NULL, 'b'},
		{"text_color", REQUIRED_ARG, NULL, 't'},
		{"data_file",  REQUIRED_ARG, NULL, 'd'}
	};
	while((result = getopt_long(argc, argv, "b:t:d:", longOpts, &optIndex)) != -1){
		switch(result){
			case 'b':{
				if(optarg){
					size_t len = strlen(optarg);
					bkgdColor = malloc(len);
					if(!bkgdColor){
						perror("malloc");
						return -1;
					}
					memcpy(bkgdColor, optarg, len);
					bkgdColorNumber = getBkgdColorNum(bkgdColor);
				}
				break;
			}
			case 't':{
				if(optarg){
					size_t len = strlen(optarg);
					txtColor = malloc(len);
					if(!txtColor){
						perror("malloc");
						return -1;
					}
					memcpy(txtColor, optarg, len);
					textColorNumber = getTextColorNum(txtColor);
				}
				break;
			}
			case 'd':{
				if(optarg){
					size_t len = strlen(optarg);
					dataFile = malloc(len);
					if(!dataFile){
						perror("malloc");
						return -1;
					}
					memcpy(dataFile, optarg, len);
				}	 
				break; 
			}
			default:
				 return -1;
		}
	}

	//set the SIGWINCH signal handler
	if(setSigwinchHandler() == -1)
		fprintf(stderr, "WARNING: Failed to set SIGEINCH handler!\n");



	if ((ret = readData(&days, &daysData, &dayCount, dataFile)) != 0) {
		printf("Error reading file data: %d\n", ret);
		return ret;
	}
	


	// Start all the ncurses stuff
	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);
	refresh();


	//ncurses color init routines
	ncursesColorInit();

	// Generate windows for all of the rendered days
	drawDays(days, daysData, dayCount, 0, &dayDisplayCount);
	mvprintw(LINES-2, 0, "^X Exit           ^R Reload Data    ^S Back 1 Day     ^D Forward 1 Day  ", COLOR_PAIR(1));
	mvchgat(LINES-1,  0, 2, A_STANDOUT, 0, NULL);
	mvchgat(LINES-1, 18, 2, A_STANDOUT, 0, NULL);
	mvchgat(LINES-1, 36, 2, A_STANDOUT, 0, NULL);
	mvchgat(LINES-1, 54, 2, A_STANDOUT, 0, NULL);
	mvprintw(LINES-1, COLS-1, "", COLOR_PAIR(1));  // Just here to put the cursor in the lower right

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
					if ((ret = readData(&days, &daysData, &dayCount, dataFile)) != 0) {
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
			drawDays(days, daysData, dayCount, currentFirstDay, &dayDisplayCount);
			mvprintw(LINES-1, COLS-1, "", COLOR_PAIR(1));  // Just here to put the cursor in the lower right
		}
		if(toExit == 1)
			break;
	}
	releaseMemory();

	endwin();
	return ret;
}

int readData(WINDOW*** days, Day** daysData, int* dayCount, const char *dataFile) {
	char type, line[MAX_LINE_LEN];
	int i, j, k, temp;
	time_t maxDay, minDay, tempTime;
	FILE* data;
	struct tm timeStruct;


	//default data file
	if(!dataFile){
		data = fopen("data.txt", "r");
		if(!data)
			return -1;
	}
	else{
		data = fopen(dataFile, "r");
		if(!data)
			return -1;
	}
	

	minDay = 31536000000;  // 1000 years after 01-01-1970
	maxDay = 0;
	timeStruct.tm_sec = 0;
	timeStruct.tm_min = 0;
	timeStruct.tm_hour = 0;
	timeStruct.tm_mday = 1;
	timeStruct.tm_year = 1900;
	timeStruct.tm_wday = 0;
	timeStruct.tm_yday = 0;
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

void drawDays(WINDOW** days, Day* daysData, int dayCount, int displayStart, int* dayDisplayCount) {
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
		ncPrint_centered_with_color(days[i], DAY_WIDTH, currentLine, temp);
		mvwchgat(days[i], currentLine++, 1, DAY_WIDTH-2, A_BOLD, 0, NULL);
		strftime(temp, DAY_WIDTH-2, "%d %b %Y", timeStruct);
		ncPrint_centered_with_color(days[i], DAY_WIDTH, currentLine++, temp);
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
			printEvent(days[i], daysData[i+displayStart].events[j], &currentLine);

		// Things due for the day
		currentLine = (LINES-7)/2+4;
		temp = "Due:";
		mvwprintw(days[i], currentLine, 1, temp, COLOR_PAIR(1));
		mvwchgat(days[i], currentLine++, 1, DAY_WIDTH-2, A_UNDERLINE, 0, NULL);
		for (j = 0; j < daysData[i+displayStart].dueCount; j++)
			printEvent(days[i], daysData[i+displayStart].dues[j], &currentLine);

		wrefresh(days[i]);
		refresh();
	}
}

void printEvent(WINDOW* day, char* eventData, int* currentLine) {
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


