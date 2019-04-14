/* entry.c*/

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


int get_entries(char* cwd, ENTRY** entry_arr, int* num_entries, int show_dots)
{
	struct dirent **namelist;
	struct stat *sb;
	char* perm;
	int dot_count, entry_index, ret;
	*num_entries = scandir(cwd, &namelist,NULL,alphasort);
	*entry_arr = malloc(sizeof(ENTRY) * (*num_entries - 2) );
	perm = malloc(sizeof(char) * (9+1));
	dot_count = 0;
	entry_index = 0;

	char abs_path[PATH_MAX];

	for (int n = 2; n < *num_entries; n++) {
		if (!show_dots && namelist[n]->d_name[0] == '.') {
			dot_count++;
			continue;
		}
		else {
			memset(( (*entry_arr) +(entry_index))->name,'\0', sizeof( ( (*entry_arr) +(entry_index))->name));
			strcpy( ( (*entry_arr) +(entry_index))->name, namelist[n]->d_name);
			((*entry_arr) +(entry_index))->index = entry_index;

			if (is_directory(cwd,((*entry_arr) +(entry_index))->name) )
				((*entry_arr) +(entry_index))->type = 'd';
			else
				((*entry_arr) +(entry_index))->type = '-';

			perm = get_permissions(cwd, ((*entry_arr) +(entry_index))->name );
			strcpy( ((*entry_arr) +(entry_index))->permission, perm);
			
			strcpy(abs_path, cwd);
			strcat(abs_path,"/");
			strcat(abs_path, ((*entry_arr) +(entry_index))->name);
			
			sb = malloc(sizeof(struct stat));
			ret = stat(abs_path, sb);

			((*entry_arr) +(entry_index))->marked = 0;
			
			entry_index++;
		}
		free(namelist[n]);
		free(sb);
		free(perm);
	}

	free(namelist);
	*num_entries -= 2;
	if (!show_dots)
		*num_entries -= dot_count;
	return 1;
}


int display_entries(ENTRY* entry_arr,int num_entries, int current_index,int LINES)
{
	int PRINT_STOP = LINES - 3;
	int SCROLL_STOP_BOTTOM = PRINT_STOP - 5;
	//int SCROLL_STOP_TOP = 10;

	if (!num_entries) {
		printw("<empty directory>");
		return 1;
	}

	if (num_entries <= PRINT_STOP) {
		for (int i = 0; i < num_entries; i++) {
			if (entry_arr[i].index == current_index) 
				wattron(stdscr, A_REVERSE);
			if (entry_arr[i].type == 'd')
				wattron(stdscr, A_BOLD);
			if (entry_arr[i].marked) {
				//wattron(stdscr, A_UNDERLINE);
				//wattron(stdscr, A_BLINK);
				printw("   ");
			}
			if ( strlen(entry_arr[i].name) >= COLS-6) {
				for (int j = 0; j < COLS-6; j++) {
					printw("%c",entry_arr[i].name[j]);
				}
				printw("...\n");
			}
			else
				printw("%s\n",entry_arr[i].name);
			wattrset(stdscr, A_NORMAL);
		}
		return 1;
	}
				
	else {
		int dist = current_index - SCROLL_STOP_BOTTOM;
		if (dist < 0)
			dist = 0;

		for (int i = 0; i <= PRINT_STOP; i++) {

			if (dist > 0)
				current_index = SCROLL_STOP_BOTTOM;

			if (entry_arr[i].index == current_index) 
				wattron(stdscr, A_REVERSE);
			if (entry_arr[i].type == 'd')
				wattron(stdscr, A_BOLD);
			if (entry_arr[i].marked) {
			//	wattron(stdscr, A_UNDERLINE);
				printw("   ");
			}
			if ( strlen(entry_arr[i+dist].name) >= COLS-6) {
				for (int j = 0; j < COLS-6; j++) {
					printw("%c",entry_arr[i+dist].name[j]);
				}
				printw("...\n");
			}
			else
				printw("%s\n",entry_arr[i+dist].name);
			wattrset(stdscr, A_NORMAL);
		}
		return 1;
	}

	return -1;
}



int clear_entries(ENTRY* p_entry_arr, int* num_entries, int* current_index)
{
	//clear();
	erase();
	*num_entries = 0;
	*current_index = 0;
	return 1;
}


int update_curr_index(short int direction, int* current_index, int *num_entries)
/* int update_pos(UP or DOWN, &current_index, &num_entries) */
{
	erase();
	if (*current_index < *num_entries || *current_index > 0)
		*current_index += direction;
	return 1;
}


int mark_file(ENTRY *p_entry)
{
	p_entry->marked = 1;
	return 1;
}


int unmark_file(ENTRY *p_entry) 
{
	p_entry->marked = 0;
	return 1;
}


int init_ncurses(WINDOW *win)
{
	initscr();
	noecho();
	keypad(stdscr,TRUE);
	curs_set(0); /* 0, 1, 2 */
	cbreak();
	//scrollok(stdscr,TRUE);
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
	}
	else{
	    //return strerror(errno);
	    for (int i = 0; i < 9; i++) {
	    	modeval[i] = '-';
	    }
	}
	modeval[9] = '\0';

	return modeval;
}

int display_file_info(char* cwd, ENTRY entry, int current_index, int num_entries)
{
	char* perm = get_permissions(cwd, entry.name);
	mvprintw(LINES-2, 0, "%d/%d  %c%s gid: %d\t uid: %d\t", current_index+1, num_entries, entry.type,perm, entry.marked, entry.gid, entry.uid);
	free(perm);
	wattron(stdscr,A_BOLD);
	if (strcmp(cwd, "/")) {
		mvprintw(LINES-1, 0, "%s/", cwd);
		wattroff(stdscr,A_BOLD);
		if ( (strlen(cwd)+1+strlen(entry.name) ) >= COLS-6) {
			move(LINES-1, strlen(cwd)+1);
			for (int i = 0; i < COLS-6-strlen(cwd)-1; i++) {
				printw("%c",entry.name[i]);
			}
			printw("...");
		}
		else
			mvprintw(LINES-1, strlen(cwd)+1, "%s",entry.name);
	}
	else {
		mvprintw(LINES-1, 0, "%s", cwd);
		wattroff(stdscr,A_BOLD);
		mvprintw(LINES-1, strlen(cwd), "%s",entry.name);
	}
	return 1;
}


int check_permissions(char* action, char* cwd, ENTRY entry)
{
	int access = false;
	if (!strcmp(action,"enter") && entry.type == 'd') {
		char bit = entry.permission[2];
		(bit == 'x') ? (access = true) : (access = false);
	}
/*
	char path[PATH_MAX];
	strcpy(path,cwd);
	strcat(path,"/");
	strcat(path,entry.name);
	if (!access(path,X_OK)) {
		access = true;
	}
*/
	return access;
}

char* get_abs_path(char* cwd, char* file_name)
{
	int len_cwd = strlen(cwd);
    int len_file_name = strlen(file_name);
    int len_abs_path = len_cwd + len_file_name + 1;
	char* abs_path;

	abs_path = malloc(sizeof(char)*len_abs_path);
	strcpy(abs_path,cwd);
	strcat(abs_path,"/");
	strcat(abs_path,file_name);

	return abs_path;
}




