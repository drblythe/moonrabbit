#include <file_type.h>

int get_file_list(char* cwd, file_t** file_arr, int* num_entries, int show_dots)
{
	struct dirent **namelist;
	char* perm;
	int dot_count, file_index;
	*num_entries = scandir(cwd, &namelist,NULL,alphasort);
	if (*file_arr != NULL) {
		free(*file_arr);
	}
	*file_arr = malloc(sizeof(file_t) * (*num_entries - 2) );
	dot_count = 0;
	file_index = 0;

	char abs_path[PATH_MAX];

	for (int n = 2; n < *num_entries; n++) {
		if (!show_dots && namelist[n]->d_name[0] == '.') {
			dot_count++;
		}
		else {
			memset(( (*file_arr) +(file_index))->name,'\0', sizeof( ( (*file_arr) +(file_index))->name));
			strcpy( ( (*file_arr) +(file_index))->name, namelist[n]->d_name);
			((*file_arr) +(file_index))->index = file_index;

			if (is_directory(cwd,((*file_arr) +(file_index))->name) )
				((*file_arr) +(file_index))->type = 'd';
			else
				((*file_arr) +(file_index))->type = '-';

			perm = get_permissions(cwd, ((*file_arr) +(file_index))->name );
			strcpy( ((*file_arr) +(file_index))->permission, perm);
			
			strcpy(abs_path, cwd);
			strcat(abs_path,"/");
			strcat(abs_path, ((*file_arr) +(file_index))->name);
			
			((*file_arr) +(file_index))->marked = 0;
			
			file_index++;
			free(perm);
		}
		free(namelist[n]);
	}
	free(namelist[0]);
	free(namelist[1]);
	free(namelist);
	*num_entries -= 2;
	if (!show_dots)
		*num_entries -= dot_count;
	return 1;
}


int index_table_init(index_table* table)
{
	// Initial array size is 20
	table->capacity = 20;
	table->size = 0;
	table->stored_index_array = (dir_index_pair*) malloc(sizeof(dir_index_pair) * table->capacity);
	return 0;
}

int index_table_grow(index_table* table)
{
	dir_index_pair* new_array = (dir_index_pair*) malloc(sizeof(dir_index_pair) * (2*(table->capacity)));
	for (int i = 0; i < table->size; i++) {
		new_array[i].dir_name = malloc(sizeof(char) * (strlen(table->stored_index_array[i].dir_name) + 1) );
		strcpy(new_array[i].dir_name, table->stored_index_array[i].dir_name);
		free(table->stored_index_array[i].dir_name);
	}
	table->capacity *= 2;
	free(table->stored_index_array);
	table->stored_index_array = new_array;
	return 0;
}

int index_table_search(index_table* table, const char* cwd)
{
	for (int i = 0; i < table->size; i++) {
		if (!strcmp(table->stored_index_array[i].dir_name, cwd)) {
			return table->stored_index_array[i].index;
		}
	}
	return -1;
}

int store_index(index_table* table, const char* cwd, int current_index)
{
	for (int i = 0; i < table->size; i++) {
		if (!strcmp(table->stored_index_array[i].dir_name, cwd)) {
			table->stored_index_array[i].index = current_index;
			return 1;
		}
	}

	if (table->size == table->capacity) {
		index_table_grow(table);
	}
	table->stored_index_array[table->size].dir_name = (char*) malloc(sizeof(char) * (strlen(cwd)+1) );
	strcpy(table->stored_index_array[table->size].dir_name, cwd);
	table->stored_index_array[table->size].index = current_index;
	table->size++;
	return 0;
}

int load_index(index_table* table, const char* cwd, int* current_index)
{
	for (int i = 0; i < table->size; i++) {
		if (!strcmp(table->stored_index_array[i].dir_name,cwd)) {
			*current_index = table->stored_index_array[i].index;
			return 0;
		}
	}
	*current_index = 0;
	return -1;
}

int index_table_free(index_table*table)
{
	for (int i = 0; i < table->size; i++) {
		free(table->stored_index_array[i].dir_name);
	}
	free(table->stored_index_array);
	return 0;
}

int clear_file_list(file_t* p_file_arr, int* num_entries, int* current_index,int reset_index)
{
	//clear();
	erase();
	*num_entries = 0;
	if (reset_index)
		*current_index = 0;
	return 1;
}

int update_curr_index(short int direction, int* current_index, int *num_entries)
{
	erase();
	if (*current_index < *num_entries || *current_index > 0)
		*current_index += direction;
	return 1;
}

int mark_file(file_t *p_file, int *num_selected)
{
	p_file->marked = 1;
	(*num_selected)++;
	return 1;
}

int unmark_file(file_t *p_file, int *num_selected) 
{
	p_file->marked = 0;
	(*num_selected)--;
	return 1;
}

int search_dir(const char* file_name, file_t* file_arr, int* current_index, const int num_entries)
{
	for (int i = 0; i < num_entries; i++) {
		if (!strcmp(file_arr[i].name,file_name)) {
			*current_index = file_arr[i].index;
			return 1;
		}
	}
	return 0;
}

int binarysearch_dir(const char* file_name, file_t* arr, const int num_entries, int* current_index)
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
	return 0;
}

int get_num_marked(int num_entries, file_t* file_arr)
{
	int n = 0;
	for (int i = 0; i < num_entries; i++) {
			if (file_arr[i].marked) {
				n++;
			}
	}
	return n;
}

int fill_copy_buffer(char*** copy_buff, int buff_size, int num_entries, file_t* file_arr, char* cwd)
{
	int n = 0;
	*copy_buff = malloc(sizeof(char*)*buff_size);
	for (int i = 0; i < num_entries && n < buff_size; i++) {
		if (file_arr[i].marked) {
			*((*copy_buff)+n) = malloc(sizeof(char) * (strlen(cwd)+1+strlen(file_arr[i].name)+1));
			strcpy(*((*copy_buff)+n),cwd);
			strcat(*((*copy_buff)+n),"/");
			strcat(*((*copy_buff)+n),file_arr[i].name);
			n++;
		}
	}
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
