#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>

int main(int argc, char* argv[]) {
	
	if (argc != 3) {
		errx(1, "Expected 3 arguments");
	}

	const char* file_1 = argv[1];
	const char* file_2 = argv[2];

	int fd1;
	if ((fd1 = open( file_1, O_RDONLY)) == -1) {
		errx(2, "Could not open file %s", file_1);
	}

	int fd2;
	if(( fd2 = open( file_2, O_WRONLY | O_CREAT)) == -1) {
		err(3, "Could not open file %s", file_2);
	}

	char c;
	int bytes_read;

	while(( bytes_read = read(fd1, &c, sizeof(c))) == sizeof(c)) {
		if ( write(fd2, &c, sizeof(c)) != sizeof(c)) {
			close(fd1);
			close(fd2);
			errx(4, "Error writing in file %s", file_2);
		}
	}

	if (bytes_read == -1) {
		errx(5, "Error reading from file %s", file_1);
	}

	close(fd1);
	close(fd2);
	exit(0);

}
