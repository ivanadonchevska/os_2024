#include <err.h>
#include <sys/stat.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

struct pair {
	uint32_t start_position;
	uint32_t length;
};

int main(int argc, char* argv[]) {

	if(argc != 4) {
		errx(1, "Error: invalid number of arguments");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1) {
		err(2, "Error: opening first file failed");
	}

	int fd2 = open(argv[2], O_RDONLY);
	if(fd2 == -1) {
		err(2, "Error: opening second file failed");
	}

	struct pair p;

	struct stat st;
	if(fstat(fd1, &st) == -1) {
		err(3, "Error: fstat failed");
	}
	if(st.st_size % sizeof(p) != 0) {
		err(4, "Error: invalid file");
	}

	if(fstat(fd2, &st) == -1) {
		err(3, "Error: fstat failed");
	}
	if(st.st_size % sizeof(uint32_t) != 0) {
		err(4, "Error: invalid file");
	}

	int fd3 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	if(fd3 == -1) {
		err(2, "Error: opening output file failed");
	}

	int bytes_read = 0;
	while(( bytes_read = read(fd1, &p, sizeof(p))) > 0 ) {
		if(lseek(fd2, p.start_position*sizeof(uint32_t), SEEK_SET) == -1) {
			err(5, "Error: lseek failed");
		}

		uint32_t buff;
		for(uint32_t i = 0; i < p.length; i++) {
			if(read(fd2, &buff, sizeof(buff)) == -1) {
				err(6, "Error: reading from file failed");
			}

			if(write(fd3, &buff, sizeof(buff)) == -1) {
				err(7, "Error: writing to file failed");
			}
		}
	}

	if(bytes_read == -1) {
		err(6, "Error: reading from pair file failed");
	}

	close(fd1);
	close(fd2);
	close(fd3);

	return 0;
}
