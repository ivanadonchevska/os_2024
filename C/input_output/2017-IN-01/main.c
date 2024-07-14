#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/stat.h>

struct idx_structure {
	uint16_t offset;
	uint8_t length;
	uint8_t reserved; //not used
}__attribute__((packed));

int main(int argc, char* argv[]) {

	if( argc != 5) {
		errx(1, "Error: invalid number of arguments");
	}

	int fd1_dat = open(argv[1], O_RDONLY);
	if(fd1_dat == -1) {
		err(2, "Error: opening dat file failed");
	}

	int fd1_idx = open(argv[2], O_RDONLY);
	if(fd1_idx == -1) {
		err(2, "Error: opening idx file");
	}
	
	struct stat st;
	// check dat file
	if(fstat(fd1_dat, &st) == -1) {
		err(3, "Error: fstat failed");
	}

	int dat_size = st.st_size;
	if(dat_size == 0) {
		err(4, "Error: dat file is empty");
	}

	if(dat_size % sizeof(uint8_t) != 0) {
		err(4, "Error: invalid dat file");
	}

	// check idx file
	if(fstat(fd1_idx, &st) == -1) {
		err(3, "Error: fstat failed");
	}

	int idx_size = st.st_size;
	if(idx_size == 0) {
		err(4, "Error: idx file is empty");
	}
	
	struct idx_structure idx_str;
	if(idx_size % sizeof(idx_str) != 0) {
		err(4, "Error: invalid file format");
	}

	// create output files
	int fd2_dat = open(argv[3], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(fd2_dat == -1) {
		err(2, "Error: creating output dat file failed");
	}

	int fd2_idx = open(argv[4], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(fd2_idx == -1) {
		err(2, "Error: creating output idx file failed");
	}

	struct idx_structure input;
	int read_size = 0;
	int read_size_2 = 0;
	uint8_t buff;
	int counter = 0;

	while(( read_size = read(fd1_idx, &idx_str, sizeof(idx_str))) == sizeof(idx_str)) {
		if(lseek(fd1_dat, idx_str.offset, SEEK_SET) == -1) {
			err(5, "Error: lseek failed");
		}
		
		if((read_size_2 = read(fd1_dat, &buff, sizeof(buff))) != sizeof(buff)) {
			err(5, "Error: reading failed");
		}

		if(buff >= 0x41 && buff <= 0x5A) {
			input.offset = lseek(fd2_dat, 0, SEEK_CUR);
			if(write(fd2_dat, &buff, sizeof(buff)) != sizeof(buff)) {
				err(6, "Error: writing to dat output failed");
			}
			counter = 1;
			while(( read_size_2 = read(fd1_dat, &buff, sizeof(buff))) == sizeof(buff)) {
				if(write(fd2_dat, &buff, sizeof(buff)) != sizeof(buff)) {
					err(6, "Error: writing to dat output failed");
				}

				counter++;

				if(counter == idx_str.length) {
					input.length = idx_str.length;
					input.reserved = idx_str.reserved;
					
					if(write(fd2_idx, &input, sizeof(input)) != sizeof(input)) {
						err(6, "Error: writing to idx output file failed");
					}
					break;
				}
			}

			if(read_size_2 == -1) {
				err(7, "Error: reading failed");
			}
		}
	}

	if(read_size == -1) {
		err(7, "Error: reading failed");
	}


	close(fd1_dat);
	close(fd1_idx);
	close(fd2_dat);
	close(fd2_idx);

	return 0;
}
