#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "todo.h"

/*
 This function founds the first entry of the symbol 'symb' in the str
 */
static int find_symb_pos(const char *str, char symb){
	size_t i;
	for(i=0; i<strlen(str); i++){
		if(str[i] == symb)
			return (int)i;
	}
	return -1;
}

int get_prio(const char *event){
	int ret, prio_position, first_colon;
	if(!event)
		return -1;

	/*
	E<.....>: PRIO ........
	 */
	prio_position = find_symb_pos(event, '>') + 1;
	if(event[prio_position] == ':'
		|| !isdigit((int)(event[prio_position])))
			return -1;

	first_colon = find_symb_pos(event, ':');

	/*this means that between '>' and ':' exists more than one symbol*/
	if(first_colon - prio_position > 1)
		return -1;

	/*it is the manipulation with ascii codes of digits
	  in order to set the return value to correct priority
	 */
	ret = (int)(event[prio_position]) - 48;
	return ret < 4 ? ret : -1;
}
