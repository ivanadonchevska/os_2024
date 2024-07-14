#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

struct patch_structure {
	uint16_t offset;
	uint8_t original;
	uint8_t new;
}__attribute__((packed));

int main(int argc, char* argv[]) {

	if(argc != 4) {
		errx(1, "Error: invalid number of arguments");
	}
	
	int patch = open(argv[1], O_RDONLY);
	if(patch == -1) {
		err(2, "Error: opening file failed");
	}

	int fd1 = open(argv[2], O_RDONLY);
	if(fd1 == -1) {
		err(2, "Error: opening file failed");
	}

	struct stat st;
	struct patch_structure p_s;

	if(fstat(patch, &st) == -1) {
		err(3, "Error: fstat failed");
	}
	int patch_size = st.st_size;
	if(patch_size == 0 || patch_size % sizeof(p_s) != 0) {
		err(4, "Error: invalid file size");
	}

	if(fstat(fd1, &st) == -1) {
		err(3, "Error: fstat failed");
	}
	int fd1_size = st.st_size;
	if(fd1_size == 0 || fd1_size % sizeof(uint8_t) != 0) {
		err(4, "Error: invalid file size");
	}

	int fd2 = open(argv[3], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(fd2 == -1) {
		err(2, "Error: creating copy file failed");
	}

	
	int read_size = 0;
	uint8_t buff;

	// copy f1 to f2
	while((read_size=read(fd1, &buff, sizeof(buff))) == sizeof(buff)) {
		if(write(fd2, &buff, sizeof(buff)) != sizeof(buff)) {
			err(5, "Error: writing to output file failed");
		}
	}

	if(read_size == -1) {
		err(6, "Error: reading from file failed");
	}

	//done with f1 file, can close it
	close(fd1);
	
	while(read(patch, &p_s, sizeof(p_s)) == sizeof(p_s)) {
		if(lseek(fd2, p_s.offset, SEEK_SET) == -1) {
			err(7, "Error: lseek failed");
		}

		if( read(fd2, &buff, sizeof(buff)) != sizeof(buff)) {
			err(6, "Error: reading failed");
		}
		if(buff == p_s.original) {
			if( lseek(fd2, -1, SEEK_CUR) == -1) {
				err(7, "Error: lseek failed");
			}
			if( write(fd2, &p_s.new, sizeof(p_s.new)) != sizeof(p_s.new)) {
				err(5, "Error: writing failed");
			}
		}
		else {
			err(8, "Error: read unexpected byte");
		}
	}

	if(read_size == -1) {
		err(6, "Error: reading from file failed");
	}

	close(fd2);
	close(patch);

	return 0;
}
