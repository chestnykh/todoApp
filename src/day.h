#ifndef DAY_H
#define DAY_H

#define MAX_EVENTS      128

/*for time_t*/
#include <time.h>

typedef struct{
	time_t day;
	int    event_count;
	int    due_count;
	/*FIXME excess memory usage*/
	int    prios[MAX_EVENTS];
	char   *events[MAX_EVENTS];
	char   *dues[MAX_EVENTS];
} Day;


#endif /*DAY_H*/
