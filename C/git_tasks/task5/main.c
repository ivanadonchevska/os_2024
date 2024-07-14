#include <err.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	if(argc != 3) {
		errx(1, "Errorr: invalid number of arguments");
	}

	const char* file_1 = argv[1];
	const char* file_2 = argv[2];

	int fd1;
	if (( fd1 = open(file_1, O_RDONLY)) == -1) {
		err(2, "Error: can not open file %s", file_1);
	};

	int fd2;
	// some problem with the permissions
	if ((fd2 = open(file_2, O_RDWR | O_CREAT | O_TRUNC,  S_IRUSR | S_IWUSR)) == -1) {
		err(2, "Error: can not open file %s", file_2);
	}

	int bytes_read;
	char c;

	while(( bytes_read = read(fd1, &c, sizeof(c))) == sizeof(c)) {
		if(( write(fd2, &c, sizeof(c))) != sizeof(c)) {
			close(fd1);
			close(fd2);
			err(3, "Error: can not write in file %s", file_2);
		}
	}

	if (bytes_read == -1) {
		close(fd1);
		close(fd2);
		err(4, "Error can not read from file %s", file_1);
	}

	close(fd1);
	close(fd2);

}
