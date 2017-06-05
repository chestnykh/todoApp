#ifndef DAY_H
#define DAY_H

#define MAX_EVENTS      128

typedef struct {
  time_t day;
  int    event_count;
  int    due_count;
  char*  events[MAX_EVENTS];
  char*  dues[MAX_EVENTS];
} Day;


#endif /*DAY_H*/
