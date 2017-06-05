#ifndef DAY_H
#define DAY_H

#define MAX_EVENTS      128

typedef struct {
  time_t day;
  int    eventCount;
  int    dueCount;
  char*  events[MAX_EVENTS];
  char*  dues[MAX_EVENTS];
} Day;


#endif /*DAY_H*/
