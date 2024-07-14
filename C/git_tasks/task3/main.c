#include <stdio.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

	if(argc != 2){
		errx(1, "Invalid number of arguments");
	}
	
	const char* file = argv[1];

	int fd;
	if (( fd = open(file, O_RDONLY)) == -1) {
		err(2, "Error opening file %s", file);
	}

	char c;
	int lines = 0;
	int words = 0;
	int chars = 0;
	int bytes_read = 0;

	while((bytes_read = read(fd, &c, sizeof(c))) == sizeof(c)) {
		if(c == '\n'){
			lines++;
			words++;
		}

		if(c == ' ') {
			words++;
		}

		chars++;
	}

	if(bytes_read == -1) {
		close(fd);
		err(3, "Error reading from file %s", file);
	}

	close(fd);

	dprintf(1, "File %s has:\nLines: %d\n Words: %d\n Chars: %d\n", file, lines, words, chars);

}
