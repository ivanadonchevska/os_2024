#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>


int main(int argc, char* argv[])
{
	if (argc != 2) {
		errx(1, "Invalid number of arguments");
	}
	
	const char* file = argv[1];
	int fd1;
	if ((fd1 = open(file, O_RDONLY)) == -1) {
		err(2, "Can open file %s", file);
	}
	
	char c;
	int bytes_read;
	int line_cnt = 0;

	while ((bytes_read = read(fd1, &c, sizeof(c))) == sizeof(c)) {
		if(write(1, &c, 1) != 1) {
			close(fd1);
			err(3, "Error writing to stdout");
		}
		if( c == '\n') {
			line_cnt++;
			if(line_cnt == 10) {
				break;
			}
		}
	}

	if (bytes_read == -1) {
		close(fd1);
		err(4, "Error reading from file %s", file);
	}

	close(fd1);
}
