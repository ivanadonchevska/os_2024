#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdbool.h>

#define ERROR_STATUS 26
#define BUFF_SIZE 4096 //OR 1024?

bool read_line(int fd, char* buffer);
bool read_line(int fd, char* buffer) {
	for(int i = 0; i < BUFF_SIZE; i++) {
		int rs = read(fd, &buffer[i], 1);
		if(rs == -1) {
			err(9, "Error: reading failed");
		}

		if(rs == 0) {
			if(i != 0) {
				err(10, "Error: incomplete data");
			}
			return false;
		}

		if(buffer[i] == '\n') {
			buffer[i] = '\0';
			return true;
		}
	}

	errx(10, "Given larger line, than can handle");
}

void terminate_processes(pid_t *pids, int count);
void terminate_processes(pid_t *pids, int count) {
	for(int i = 0; i < count; i++) {
		kill(pids[i], SIGTERM);
	}
	for(int i = 0; i < count; i++) {
		waitpid(pids[i], NULL, 0);
	}
}

int main(int argc, char* argv[]) {
	
	if (argc < 2) {
		errx(1, "Error: invalid number of arguments");
	}

	int NC = argc - 1; // number of commands, number of children
	int pf[NC][2];
	pid_t pids[NC];

	// each child will write in it's pipe and parent will read from all child pipes
	for(int i = 0; i < NC; i++) {
		if(pipe(pf[i]) == -1) {
			err(ERROR_STATUS, "Error: creating pipe for child number %d failed", i);
		}

		// create child process
		pids[i] = fork();
		if(pids[i] == -1) {
			err(ERROR_STATUS, "Error: creating child number %d failed", i);
		}

		if(pids[i] == 0) {
			close(pf[i][0]); // close reading end
			
			if(dup2(pf[i][1], 1) == -1) {
				err(ERROR_STATUS, "Error: dup2 for child number %d failed", i);
			}

			if(execlp(argv[i+1], argv[i+1], (char*)NULL) == -1) {
				err(ERROR_STATUS, "Error: execlp from child number %d failed", i);
			}
		}
		
		// if not child close writing end
		close(pf[i][1]);
	}

	char buff[BUFF_SIZE];
	int found = 0;
	const char* msg = "found it!";
	for(int i = 0; i < NC; i++) {
		while(read_line(pf[i][0], buff)) {
		
			if (buff == msg) {
				found = 1;
				break;
			}
			// not sure if can use strstr??
			/*if(strstr(buff, "found it!") != NULL) {
				found = 1;
				break;
			}*/

		}
		close(pf[i][0]);
		if(found) {
			break;
		};
	}
	
	// on success 0
	if(found) {
		terminate_processes(pids, NC);
		exit(0);
	} else {
		// if not found 1
		for(int i = 0; i < NC; i++) {
			waitpid(pids[i], NULL, 0);
		}
		exit(1);
	}

	// other fail is handled by err, on each system call

	return 0;
}
