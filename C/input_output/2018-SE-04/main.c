#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdint.h>

int cmp(const void* a, const void* b);
int cmp(const void* a, const void* b) {
//	return *((const uint16_t*)a) - ((const uint16_t*)b);
	return (*(const uint16_t*)a - *(const uint16_t*)b);
}

int main(int argc, char* argv[]) {
	
	if(argc != 3) {
		errx(1, "Error: invalid number of arguments");
	}

	int fd_in = open(argv[1], O_RDONLY);
	int fd_out = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666);

	if(fd_in == -1 || fd_out == -1) {
		err(2, "Error: opening files failed");
	}

	struct stat st;
	if(fstat(fd_in, &st) == -1) {
		err(3, "Error: fstat failed");
	}

	if(st.st_size % sizeof(uint16_t) != 0) {
		err(4, "Error: file does not contain only uint16_t numbers");
	}

	int num_of_elements = st.st_size / sizeof(uint16_t);
	if(num_of_elements > 65535) {
		err(4, "Error: max limit of elements reached");
	}

	uint16_t* buffer = malloc(num_of_elements * sizeof(uint16_t));

	if(read(fd_in, buffer, num_of_elements * sizeof(uint16_t)) != (ssize_t)(num_of_elements * sizeof(uint16_t))) {
		err(5, "Error: reading failed");
	}

	qsort(buffer, num_of_elements, sizeof(uint16_t), cmp);

	if(write(fd_out, buffer, num_of_elements * sizeof(uint16_t)) != (ssize_t) (num_of_elements * sizeof(uint16_t))) {
		err(6, "Error: writing failed");
	}


	close(fd_in);
	close(fd_out);
	free(buffer);

	return 0;
}
