/*display.h*/
#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int preview_text_file(char *cwd, char *file_name)
{
    FILE *fp;
    char line[255];
	
    fp = fopen("/home/haru/Documents/moonrabbit/config","r");
    while (fgets(line,255,(FILE*)fp))
        printf("%s\n", line);
    fclose(fp);

	return 1;
}
#endif
