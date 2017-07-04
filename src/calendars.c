#include "calendars.h"
#include "todo.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h> /*for PATH_MAX*/


ChArray *read_calendars_file(const char *path){
	char next_filename[PATH_MAX];
	size_t len;	
	int lines = 0;
	ChArray *ret_list;


	FILE *file;
	file = fopen(path, "r");
	if(!file)
		return NULL;

	/*First of all, count the number of filenames*/
	while(fgets(next_filename, PATH_MAX, file) != NULL)
		lines++;

	if(lines == 0)
		return NULL;

	if((fseek(file, 0, SEEK_SET)) != 0)
		return NULL;

	ret_list = malloc(lines*sizeof(ChArray));
	nr_files = lines;

	/*using this variable as a string iterator*/
	lines = 0;
	while(fgets(next_filename, PATH_MAX, file) != NULL){
		len = strlen(next_filename);
		next_filename[len - 1] = '\0';
		len = len - 1;
		ret_list[lines].data = malloc(len);
		memcpy(ret_list[lines].data, next_filename, len);
		ret_list[lines].len = len;
		lines++;
	}
	return ret_list;
}
