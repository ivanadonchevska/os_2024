#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

struct pair {
    uint32_t key;
    char word[64];
};

uint32_t binary_search(const struct pair arr[], int size, const char* targetValue); 
uint32_t binary_search(const struct pair arr[], int size, const char* targetValue) {
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        int cmp = strcmp(arr[mid].word, targetValue);
        if (cmp == 0) {
            return arr[mid].key;
        }

        if (cmp < 0) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return UINT32_MAX; // Assuming UINT32_MAX is used as an indicator of not found
}

int main(int argc, char* argv[]) {
	if(argc < 4) {
		errx(1, "Error: invalid number of arguments");
	}

	int fd_dict = open(argv[2], O_RDONLY);
	if(fd_dict == -1) {
		err(2, "Error: opening file failed");
	}

	int fd_idx = open(argv[3], O_RDONLY);
	if(fd_idx == -1) {
		err(2, "Error: opening file failed");
	}

	int index_file_size = lseek(fd_idx, 0, SEEK_END);
	int num_of_records = index_file_size / sizeof(uint32_t);

	
	struct pair* arr = malloc(num_of_records * sizeof(struct pair));
    if (arr == NULL) {
        err(3, "Error: memory allocation failed");
    }

    lseek(fd_idx, 0, SEEK_SET);
    uint32_t idx_offset;
    char word[64];
	
	// logic for storing offset and word in array
    for (int i = 0; i < num_of_records; i++) {
        if(read(fd_idx, &idx_offset, sizeof(idx_offset)) != sizeof(idx_offset)) {
        	err(2, "reading failed");
		}
        if( lseek(fd_dict, idx_offset, SEEK_SET) == -1) {
        	err(3, "lseek failed");
		}

        char ch;
        word[0] = '\0';
        while (read(fd_dict, &ch, sizeof(char)) == sizeof(char) && ch != '\n') {
           	strncat(word, &ch, 1);
        }
	
        arr[i].key = idx_offset;
        strcpy(arr[i].word, word);
    }

	/*dprintf(1, "Array Contents:\n");
    for (int i = 0; i < num_of_records; i++) {
        dprintf(1,"Key: %u, Value: %s\n", arr[i].key, arr[i].word);
    }
    printf("\n"); */

    const char* target_value = argv[1];
    int str_len = strlen(target_value);
    //dprintf(1, "len %d ", str_len);
    uint32_t result = binary_search(arr, num_of_records, target_value);

    if (result != UINT32_MAX) {
    	char description[256];
		char c;

		description[0] = '\0';
		
		// handle the lseek to get the right description
		if(lseek(fd_dict, result, SEEK_SET) == -1) {
			err(2, "lseek failed");
		}
		lseek(fd_dict, str_len + 1, SEEK_CUR);

		while (read(fd_dict, &c, 1) == 1 && c != '\0') {
    		strncat(description, &c, 1); 
		}

		dprintf(1, "%s ", description);
	} else {
		dprintf(1, "Word is not in the dictionary");
	}
	
	close(fd_dict);
    close(fd_idx);
    free(arr);

	return 0;
}

