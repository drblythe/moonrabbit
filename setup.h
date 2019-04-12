/*setup.h*/
#ifndef SETUP_H
#define SETUP_H
#include <stdlib.h>
#include <string.h>

int set_default_programs(char* config_path)
{
    FILE *fp;
	int n;
    char line[255];
    fp = fopen(config_path,"r");
    while (fgets(line,255,(FILE*)fp)) {
		if (line[0] == '#')
			continue;
		else
			n = 0;
			while 
	}
    fclose(fp);
	
	return 1;
}
