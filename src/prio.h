#ifndef PRIO_H
#define PRIO_H


/*
@get_prio(const char *event)
returns: -1 means an error, or priority isn't set
another value is the priority of the passed event as an argument
We provide 4 levels of importance: from 0 to 3
 */

int get_prio(const char *event);


#endif /*PRIO_H*/
