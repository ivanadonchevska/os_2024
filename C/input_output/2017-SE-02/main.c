#include <err.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int print_line = 0;
int cnt = 1;

void custom_read(int fd, const char* from);
void custom_read(int fd, const char* from) {
	int read_size = 0;
	char buff;
	int new_line = 1;
// the logic here is not working at all, gonna fix it later!!!
// TODO
	while((read_size = read(fd, &buff, sizeof(buff))) == sizeof(buff)) {
		if(print_line) {
			if(new_line) {
				dprintf(1, "%d %s", cnt, &buff);
				new_line = 0;
				cnt++;
			} else {
				if(write(1, &buff, sizeof(buff)) != sizeof(buff)) {
					err(7, "Error: writing to stdout failed");
				}
			} 
		} else {
			if(write(1, &buff, sizeof(buff)) != sizeof(buff)) {
				err(7, "Error: writing to stdout failed");
			}
		}
	}

	if(read_size == -1) {
		err(6, "Error: reading from file %s failed", from);
	}
}

int main(int argc, char* argv[]) {
	
	if(argc == 1) {
		custom_read(0, "STDIN");
		//exit(0);
	}

	if(argc == 2 && strcmp(argv[1], "-n") == 0) {
		print_line = 1;
		custom_read(0, "STDIN");
		exit(0);
	}
	
	if(strcmp(argv[1], "-n")) {
		print_line = 1;
	}

	// handle case when script is called with arguments
	for( int i = 2; i < argc; i++) {
		if(strcmp(argv[i], "-")) {
			custom_read(0, "STDIN");
			continue;
		}

		const char* file = argv[i];

		int fd = open(file, O_RDONLY);
		if(fd == -1) {
			err(1, "Error: reading from file failed");
		}

		custom_read(fd, file);
	}

	return 0;
}
