#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>


bool check_byte(unsigned char c);
bool check_byte(unsigned char c) {
	return c == 0x00 || c == 0xFF || c == 0x55 || c == 0x7D;
}


int main(int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "Error: invalid number of arguments");
	}

	const char* input_file = argv[1];
	const char* output_file = argv[2];

	int pf[2];
	if(pipe(pf) == -1) {
		err(2, "Error: pipe failed");
	}

	const pid_t pid = fork();
	if( pid == -1) {
		err(3, "Error: fork failed");
	}

	if (pid == 0) {
		close(pf[0]);

		if(dup2(pf[1], 1) == -1) {
			err(4, "Error: dup2 failed");
		}

		if(execlp("cat", "cat", input_file, (char*) NULL) == -1) {
			err(5, "Error: execlp failed");
		}
	}

	close(pf[1]);

	int fd = open(output_file, O_CREAT | O_TRUNC | O_RDWR, S_IRUSR, S_IWUSR);
	if(fd == -1) {
		err(6, "Error: opening output_file failed");
	}

	int bytes_read;
	unsigned char escape_char = 0x7D;
	unsigned char key = 0x20;
	unsigned char current_byte;

	while((bytes_read = read(pf[0], &current_byte, sizeof(current_byte))) > 0) {
		if(current_byte == escape_char) {
			// like if escape_char read one more byte
			if ((bytes_read = read(pf[0], &current_byte, sizeof(current_byte))) > 0) {
				unsigned char original_byte = (current_byte ^ key);
				if(!check_byte(original_byte)) {
					errx(7, "Error: invalid file content");
				}

				if(write(fd, &original_byte, sizeof(original_byte)) != sizeof(original_byte)) {
					err(8, "Error: writing to file failed");
				}
			}
		}
	}

	if(bytes_read == -1) {
		err(9, "Error: reading from file failed");
	}

	close(fd);

	return 0;
}
