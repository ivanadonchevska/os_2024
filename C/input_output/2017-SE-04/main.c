#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

void custom_read(int fd, const char* from);
void custom_read(int fd, const char* from) {
	int read_size = 0;
	char buff;

	while((read_size = read(fd, &buff, sizeof(buff))) > 0) {
		if(write(1, &buff, sizeof(buff)) != sizeof(buff)) {
			err(3, "Error: writing to stdout failed");
		}
	}

	if(read_size == -1) {
		err(2, "Error: reading from %s failed", from);
	}
	close(fd);
}

int main(int argc, char* argv[]) {
	
	// no arguments, read from stdin
	if(argc == 1) {
		custom_read(0, "STDIN");
		exit(0);
	}

	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "-") == 0) {
			char buff[256];
			int rs;
			if((rs = read(0, &buff, sizeof(buff))) == -1) {
				err(2, "Error: reading failed");
			}
			buff[rs] = '\0';
			if( write(1, &buff, rs) != rs) {
				err(3, "Error: writing failed");
			}
			continue;
		}
		
		const char* file = argv[i];
		int fd = open(file, O_RDONLY);
		if(fd == -1) {
			err(1, "Error: opening file failed");
		}

		custom_read(fd, file);
	}

	return 0;
}
