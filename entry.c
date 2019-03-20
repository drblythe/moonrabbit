#include "entry.h"



int is_directory(char* cwd, char* name)
{

	struct stat sb;
	int ret;
	char abs_path[100];
	strcpy(abs_path,cwd);
	strcat(abs_path,"/");
	strcat(abs_path,name);

	(stat(abs_path, &sb) == 0 && S_ISDIR(sb.st_mode)) ? (ret=1) : (ret=0);


	return ret;
}



int get_entries(char* cwd, ENTRY** entry_arr, int* num_entries)
/* int get_entriesw(cwd, &entry_arr, &num_entries)*/
/* take in string literal, addr of entry array to add, addr of entry count so it can be modified */
{
	struct dirent **namelist;
	struct stat *sb;
	sb = malloc(sizeof(struct stat));
	*num_entries = scandir(cwd, &namelist,NULL,alphasort);
	*entry_arr = malloc(sizeof(ENTRY) * (*num_entries - 2) );

	for (int i = 2; i < *num_entries; i++) {
		stat(  ((*entry_arr) +(i-2))->name, stat_buf);
		memset(( (*entry_arr) +(i-2))->name,'\0', sizeof( ( (*entry_arr) +(i-2))->name));

		strcpy( ( (*entry_arr) +(i-2))->name, namelist[i]->d_name);
		free(namelist[i]);

		if (is_directory(cwd,((*entry_arr) +(i-2))->name) )
			((*entry_arr) +(i-2))->is_dir = 1;
		else
			((*entry_arr) +(i-2))->is_dir = 0;
		((*entry_arr) +(i-2))->index = i-2;


	}
	free(stat_buf);
	free(namelist);
	*num_entries -= 2;

	return 1;
}



int display_entries(ENTRY* entry_arr,int num_entries, int current_index,int LINES)
{
	int PRINT_STOP = LINES - 3;
	int SCROLL_STOP_BOTTOM = PRINT_STOP - 8;
	//int SCROLL_STOP_TOP = 10;

	if (!num_entries) {
		printw("<empty directory>");
		return 1;
	}

		if (num_entries <= PRINT_STOP) {
			for (int i = 0; i < num_entries; i++) {
				if (entry_arr[i].index == current_index) {
					wattron(stdscr, A_REVERSE);
					//start_color();
					printw("%s\n",entry_arr[i].name);
					wattroff(stdscr, A_REVERSE);
				}
				else
					if (entry_arr[i].is_dir){
						wattron(stdscr, A_BOLD);
						printw("%s\n",entry_arr[i].name);
						wattroff(stdscr, A_BOLD);
					}
					else
						printw("%s\n",entry_arr[i].name);
			}
			return 1;
		}
		else {
			int dist = current_index - SCROLL_STOP_BOTTOM;
			if (dist < 0)
				dist = 0;

			for (int i = 0; i < PRINT_STOP; i++) {

					if (dist > 0)
						current_index = SCROLL_STOP_BOTTOM;
					if (entry_arr[i].index == current_index) {
						wattron(stdscr, A_REVERSE);
						printw("%s\n",entry_arr[current_index+dist].name);
						wattroff(stdscr, A_REVERSE);
					}
					else {
						printw("%s\n",entry_arr[i+dist].name);
					}
			}
			return 1;
		}


	return -1;
}



int clear_entries(ENTRY* p_entry_arr, int* num_entries)
{
	//clear();
	erase();
	*num_entries = 0;
	return 1;
}




/*
SOME USEFUL NCURSES ATTRIBUTES
ALL PREFIXED WITH 'A_', |'ed together


A_STANDOUT
A_UNDERLINE
..REVERSE
BLINK
DIM
BOLD
NORMAL


*/



int update_curr_index(short int direction, int* current_index, int *num_entries)
/* int update_pos(UP or DOWN, &current_index, &num_entries) */
{
	erase();
	if (*current_index < *num_entries || *current_index > 0)
		*current_index += direction;
	return 1;
}


int mark_file()
{

	return 1;
}


int init_ncurses(WINDOW *win)
{
	initscr();
	noecho();
	//keypad(FALSE);
	curs_set(0); /* 0, 1, 2 */
	cbreak();
	scrollok(stdscr,TRUE);
	//start_color();
	move(0,0);
	refresh();
	return 1;
}

char* get_permissions(char* cwd, char* file_name)
{
	int len_cwd = strlen(cwd);
	int len_name = strlen(file_name);
	int len_file_path = len_cwd + len_name + 1;

	char file_path[len_file_path];
	strcpy(file_path, cwd);
	strcat(file_path,"/");
	strcat(file_path,file_name);

	struct stat st;
	char *modeval = malloc(sizeof(char)*9 + 1);
	if (stat(file_path, &st) == 0) {
        mode_t perm = st.st_mode;
		modeval[0] = (perm & S_IRUSR) ? 'r' : '-';
		modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
	    modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
	    modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
	    modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
	    modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
	    modeval[6] = (perm & S_IROTH) ? 'r' : '-';
	    modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
	    modeval[8] = (perm & S_IXOTH) ? 'x' : '-';
	    modeval[9] = '\0';
	}
	else{
	    //return strerror(errno);
	    for (int i = 0; i < 9; i++) {
	    	modeval[i] = '-';
	    }
	    modeval[9] = '\0';
	}
	return modeval;
}

int display_file_info(char* cwd,char* file_name,int current_index, int num_entries)
{
	char* perm = get_permissions(cwd, file_name);
	mvprintw(LINES-2, 0, "%d/%d  %s", current_index+1, num_entries, perm);
	wattron(stdscr,A_BOLD);
	if (strcmp(cwd, "/")) {
		mvprintw(LINES-1, 0, "%s/", cwd);
		wattroff(stdscr,A_BOLD);
		mvprintw(LINES-1, strlen(cwd)+1, "%s",file_name);
	}
	else {
		mvprintw(LINES-1, 0, "%s", cwd);
		wattroff(stdscr,A_BOLD);
		mvprintw(LINES-1, strlen(cwd), "%s",file_name);
	}
	free(perm);
	return 1;
}





