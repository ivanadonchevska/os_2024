#include <err.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {

	if(argc != 2) {
		errx(1, "Error: invalid number of arguments");
	}

	const char* file_name = argv[1];

	int fd = open(file_name, O_RDWR);
	if(fd == -1) {
		err(2, "Error: opening file for reading and writing failed");
	}

	uint8_t bytes[256] = { 0 };

	int bytes_read = 0;
	uint8_t byte;

	while(( bytes_read = read(fd, &byte, sizeof(byte))) == sizeof(byte)) {
		bytes[byte]++;
	}

	if(bytes_read == -1 ) {
		err(3, "Error: reading from faile failed");
	}

	if( lseek(fd, 0, SEEK_SET) == -1) {
		err(4, "Error: lseek failed");
	}

	for(int i = 0; i < 256; i++) {
		uint8_t c = i;
		for(int j = 0; j < bytes[i]; j++) {
			if(write(fd, &i, sizeof(i)) == -1) {
				err(5, "Error: writing to file failed");
			}
		}
	}

	close(fd);
	return 0;
}
