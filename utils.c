#include "utils.h"

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
