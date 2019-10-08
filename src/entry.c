#include "entry.h"

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
			if (ret) {
				perror("stat");
				return -1;
			}

			((*entry_arr) +(entry_index))->marked = 0;
			
			entry_index++;
		}
		//free(namelist[n]);
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
	unsigned int RIGHT_PADDING = 0 + 4;
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
			if ( strlen(entry_arr[i].name) >= COLS-RIGHT_PADDING) {
				for (int j = 0; j < COLS-RIGHT_PADDING; j++) {
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
			if ( strlen(entry_arr[i+dist].name) >= COLS-RIGHT_PADDING) {
				for (int j = 0; j < COLS-RIGHT_PADDING; j++) {
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

/* KEY_VALUE** is pointer to an array */
int save_index(KEY_VALUE** p_kvp_arr, int num_stored, const char* cwd, const int current_index)
{
	char already_stored = 0;
	//int array_index = num_stored;
	//printf("\n--array_index%d--",array_index);
		
	for (int i = 0; i < num_stored; i++) 
		if (!strcmp((*p_kvp_arr+i)->key, cwd)) {
			already_stored = 1;
			break;
		}
	if (already_stored) {
	//	printf("\n--ALREADY STORED--");
		return 0;
	}
	else {
		//printf("\n--NEWLY STORED--");
		int index_to_use;
		if ( (num_stored) + 1 > 20) {
			//printf("--NUM > 20--");
			index_to_use = 0;
			// move all others up by one (19 will be overwritten)
			for (int i = 0; i < 19; i++) {
				strcpy((*p_kvp_arr + i + 1)->key, (*p_kvp_arr + i)->key);
				(*p_kvp_arr + i + 1)->value = (*p_kvp_arr + i)->value;
			}
			
		}
		else {
			//printf("--NUM < 20--");
			index_to_use = current_index;
		}

		strcpy((*p_kvp_arr + index_to_use)->key,cwd);
		(*p_kvp_arr + index_to_use)->value = current_index;
		//printf("--index-to-use=%d--",index_to_use);
		//printf("--SAVED AS cwd=%s,index=%d--",  (*p_kvp_arr + index_to_use)->key ,(*p_kvp_arr + index_to_use)->value );
	}

	//printf("--NEW NUM STORED=%d--",num_stored);
	//getchar();
	
	return 1;
}

int load_index(KEY_VALUE** p_kvp_arr, const int num_stored, const char* cwd, int* current_index)
{
	char already_stored = 0;
	int return_index = -1;
	for (int i = 0; i < num_stored; i++) 
		if (!strcmp((*p_kvp_arr+i)->key, cwd)) {
			already_stored = 1;
			return_index = i;
			break;
		}
	if (!already_stored)
		return 0;
	else {
		*current_index = return_index;
	}
	return 1;
}


int clear_entries(ENTRY* p_entry_arr, int* num_entries, int* current_index,int reset_index)
{
	//clear();
	erase();
	*num_entries = 0;
	if (reset_index)
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


int mark_file(ENTRY *p_entry, int *num_selected)
{
	p_entry->marked = 1;
	(*num_selected)++;
	return 1;
}


int unmark_file(ENTRY *p_entry, int *num_selected) 
{
	p_entry->marked = 0;
	(*num_selected)--;
	return 1;
}



int display_file_info(char* cwd, ENTRY entry, int current_index, int num_entries)
{
	int RIGHT_PADDING = 0 + 5;
	if (!num_entries)
		return 0;
	char* perm = get_permissions(cwd, entry.name);
	int path_len = strlen(cwd) + 1 + 1 + strlen(entry.name) + 1;
	//mvprintw(LINES-2, 0, "%d/%d  %c%s gid: %d\t uid: %d\t", current_index+1, num_entries, entry.type,perm, entry.marked, entry.gid, entry.uid);
	mvprintw(LINES-2, 0, "%d/%d  %c%s", current_index+1, num_entries, entry.type,perm);
	free(perm);

	// Print path
	// If full path is less than the width of the term, just print it
	wattron(stdscr,A_BOLD);
	if (path_len < COLS-RIGHT_PADDING) {
		mvprintw(LINES-1, 0, "%s",cwd);
		wattroff(stdscr,A_BOLD);
		if (strcmp(cwd,"/")) {
			mvprintw(LINES-1, strlen(cwd), "/", entry.name);
			mvprintw(LINES-1, strlen(cwd)+1, "%s", entry.name);
		}
		else {
			mvprintw(LINES-1, strlen(cwd), "%s", entry.name);
		}
	}
	// If full path is greater than width of term, 
	else {
		char path[path_len];
		strcpy(path,cwd);
		if (!strcmp(cwd,"/")) {
			strcat(path, "/");
		}
		strcat(path, entry.name);
		for (int i = 0; i < COLS-RIGHT_PADDING; i++) {
			if (i > strlen(cwd)) {
				wattroff(stdscr,A_BOLD);
			}
			mvprintw(LINES-1, i, "%c", path[i]);
		}
		mvprintw(LINES-1, COLS-RIGHT_PADDING, "...");
	}

	return 1;
}


int search_dir(const char* file_name, ENTRY* entry_arr, int* current_index, const int num_entries)
{
	for (int i = 0; i < num_entries; i++) {
		if (!strcmp(entry_arr[i].name,file_name)) {
			*current_index = entry_arr[i].index;
			return 1;
		}
	}
	return 0;
}

int binarysearch_entry(const char* file_name, ENTRY* arr, const int num_entries, int* current_index)
{
	int low = 0;
	int high = num_entries;
	int mid;

	while (low <= high) {
		mid = (low+high)/2;
		if (strcmp(file_name, arr[mid].name) == 0) {
			*current_index = arr[mid].index;
			return 1;
		}
		else if (strcmp(file_name, arr[mid].name) < 0) 
			high = mid-1;
		else if (strcmp(file_name, arr[mid].name ) > 0) 
			low = mid+1;
	}

	return -1;
}


int get_num_marked(int num_entries, ENTRY* entry_arr)
{
	int n = 0;
	for (int i = 0; i < num_entries; i++) {
			if (entry_arr[i].marked) {
				n++;
			}
	}
	return n;
}


int fill_copy_buffer(char*** copy_buff, int buff_size, int num_entries, ENTRY* entry_arr, char* cwd)
{
	int n = 0;
	*copy_buff = malloc(sizeof(char*)*buff_size);
	for (int i = 0; i < num_entries && n < buff_size; i++) {
		if (entry_arr[i].marked) {
			*((*copy_buff)+n) = malloc(sizeof(char) * (strlen(cwd)+1+strlen(entry_arr[i].name)+1));
			strcpy(*((*copy_buff)+n),cwd);
			strcat(*((*copy_buff)+n),"/");
			strcat(*((*copy_buff)+n),entry_arr[i].name);
			n++;
		}
	}
	return 1;
}

int print(char*** copy_buff, int buff_size)
{
	FILE* fp;
	fp = fopen("./output","w");
	for (int i = 0; i < buff_size; i++) {
		fputs(*(*(copy_buff)+i),fp);
		fputs("\n",fp);
		printf("---%s---\n",*(*(copy_buff)+i));
	}
	fclose(fp);
	return 1;
}

int empty_copy_buffer(char*** copy_buff, int* buff_size)
{
	for (int i = 0; i < *buff_size; i++)
		free(*((*copy_buff)+i));
	free(*copy_buff);
	*buff_size= 0;
	return 1;
}

