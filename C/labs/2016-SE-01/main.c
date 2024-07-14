#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>


// uint8_t array[256]
// for byte in file
//	array[byte]++

int main(int argc, char* argv[]) {
	
	if(argc != 2) {
		errx(2, "Argument count");
	}

	int fd;
	fd = open(argv[1], O_RDWR);
	if(fd == -1) {
		err(1, "Could not open file %s", argv[1]);
	}
	
	// err -> sistemno izvikvane a errx kogato nyama sistemno izvikvane ismkame da priklucim programta
	// open ako se e provalil shte zapishe v errno i zatova zapishvame err, dokato errx ne se interesuva ot errno

	uint8_t array[256];
	
	for ( int i = 0; i < 256; i++) {
		array[i] = 0; // cargabe values
	}

	uint8_t byte;
	int result; // zashtoto iskame da vidim dali stojnosta e otricatelno chislo

	while((result = read(fd, &byte, sizeof(byte))) == sizeof(byte)) {
		array[byte]++;
	}

	if(result == -1) {
		err(1, "error read");
	}

	if(lseek(fd, 0, SEEK_SET) == -1){
		err(2, "lseek");
	}

	for(int i = 0; i < 256; i++) {
		for(int j = 0; j < array[i]; j++) {
			if(write(fd, &i, 1) == -1){
				err(1, "write");
			}
		}
	}
	
	close(fd);
}
