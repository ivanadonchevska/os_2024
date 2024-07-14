#include <err.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

const int BUFF_SIZE = 4096;

int main(void) {
	
	int bytes_read;
	char buff[BUFF_SIZE];

	const char* input_text = "Enter command: ";

	if(write(1, input_text, strlen(input_text)) != (int)strlen(input_text)) {
		err(1, "Error: writing to stdout failed");
	}

	while(1) {

		bytes_read = read(0, &buff, sizeof(buff));
		if(bytes_read == -1) {
			err(2, "Error: reading failed");
		}

		buff[bytes_read - 1] = '\0';

		const pid_t pid = fork();
		if(pid == -1) {
			err(3, "Error, fork failed");
		}

		// exit it exit is written
		if(strcmp(buff, "exit") == 0) {
			exit(0);
		}
		
		if(pid == 0) {
			char command[BUFF_SIZE];
			strcpy(command, "/bin/"); // copy /bin/ to command
			strcat(command, buff); // concatenate /bin/ to buff to get full path
			if(execl(command, buff, (char*) NULL) == -1) {
				err(4, "Error: execl failed");
			}
		}

		int status;
		if(wait(&status) == -1) {
			err(5, "Error: could not wait for child process to finish");
		}

		if(!WIFEXITED(status)) {
			errx(6, "Error: child process did not terminate normally");
		}

		if(WEXITSTATUS(status) != 0) {
			errx(7, "Error: child process finished with exit code different from 0"); 
		}
	}
}
