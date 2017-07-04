#ifndef CALENDARS_H
#define CALENDARS_H 1

#include <stddef.h>

/*Character Array*/
typedef struct{
	char *data;
	size_t len;
} ChArray;

/*This function reads names of files which contain different calendars  
 */
ChArray *read_calendars_file(const char *path);

#endif /*CALENDARS_H*/
