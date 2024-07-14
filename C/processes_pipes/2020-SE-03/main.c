#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>


typedef struct  {
	char file_name[8];
	uint32_t offset;
	uint32_t length;
}__attributes__((packed)) row;


int main(int argc, char* argv[]) {
	if(argc != 2) {
		errx(1, "Error: invalid number of arguments");
	}

	const char* input_file = argv[1];

	int fd = open(input_file, O_RDONLY);
	if(fd == -1) {
		err(2, "Error: opening file failed");
	}

	struct stat st;
	if (stat(input_file, &st) == -1) {
		err(3, "Error: stat failed");
	}

	// check if the given file size and content is valid according to struct
	if(st.st_size % sizeof(row) != 0) {
		errx(4, "Error: invalid content");
	}
	
	int  num_rows = st.st_size / sizeof(row);

	if (num_rows > 8) {
		err(5, "Error: number of rows is bigger than expected");
	}

	int pf[2];
	if(pipe(pf) == -1) {
		err(6, "Error: pipe failed");
	}
	

	// this for loop is used to create n child processes(by parent)
	for (int i = 0; i < n; i++) {
		const pid_t pid = fork();
		if(pid == -1) {
			err(7, "Error: fork failed");
		}
		if(pid == 0) {
			// each child process immediately exists the loop and performs no further iterations, preventing it from creating additional child processes
			break;
		}
	}

	if(pid == 0) {
		close(pf[0]);
		row buff;
		if(read(fd, &buff, sizeof(buff)) != sizeof(buff)) {
			err(8, "Error: reading from input_file failed");
		}
		
		int fd_c = open(buff.file_name, O_RDONLY);
		if(fd_c == -1) {
			err(8, "Error: opening child file failed");
		}

		//if (fstat(fi, $st) == -1) {
		//	err(9, "Error: fstat failed");
		//}

		uint16_t res = 0;

		if(lseek(fd_c, buff.offset * sizeof(uint16_t), SEEK_SET) == -1) {
			err(10, "Error: lseek failed");
		}

		for(uint32_t i = 0; i < buff.length; i++) {
			uint16_t curr;
			if(read(fd_c, &curr, sizeof(curr)) != sizeof(curr)) {
				err(8, "Error: reading from child file failed");
			}

			res = res ^ curr;
		}
			if(write(pf[1],  &res, sizeof(res)) != sizeof(res)) {
				err(11, "Error: writing in pipe failed");
			}

			exit(0);
	}

	close(pf[1]);
	uint16_t result = 0;
	for(int i = 0; i < num_rows; i++) {
		uint16_t child_res;
		if(read(pf[0], &child_res, sizeof(child_res)) != sizeof(child_res)) {
			err(8, "Error: reading from pipe failed");
		}

		result = result ^ child_res;
	}

	dprintf(1, "result: %dB\n", result);
	exit(0);
	
}
