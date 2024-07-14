#include <err.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int seconds_condition(int seconds, long long start_time, long long end_time);
int seconds_condition(int seconds, long long start_time, long long end_time) {
	return (end_time - start_time) < seconds;
}

int main(int argc, char* argv[]) {

	if(argc < 3) {
		errx(1, "Error: invalid number of arguments");
	}

	int seconds = atoi(argv[1]);
	char* Q = argv[2];
	dprintf(1, Q);
	char** args = argv + 2;
	for (int i = 0; args[i] != NULL; i++) {
        dprintf(1, "args[%d] = %s\n", i, args[i]);
	}

	int condition = 0;
	int fd = open("run.log", O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR, S_IWUSR);
	if(fd == -1) {
		err(2, "Error: opening file for logs failed");
	}

	while(1) {
		
		const pid_t pid = fork();
		if(pid == -1) {
			err(3, "Error: fork failed");
		}

		if(pid == 0) {
			if (execvp(Q, args) == -1) {
				err(4, "Error: execvp failed");
			}
		} else {

			time_t start_time = time(NULL);
			if( start_time == (time_t)(-1)) {
				err(5, "Error: time_t failed");
			}

			int status;
			if(wait(&status) == -1) {
				err(6, "Error: could not wait for child process to finish");
			}

			time_t end_time = time(NULL);
			if( end_time == (time_t)(-1)) {
				err(5, "Error: time_t failed");
			}

			int exit_code;
			if(WIFEXITED(status)) {
				exit_code = WEXITSTATUS(status);
			} else if (WIFSIGNALED(status)) {
				exit_code = 129;
			} else {
				errx(7, "Unreachable exit status");
			}
	
			// writes needed information in log file
			dprintf(fd, "%ld %ld %d\n", start_time, end_time, exit_code);

			// check if stop condition happened
			if (exit_code != 0 && seconds_condition(seconds, start_time, end_time)) {
				if (condition == 1) {
					// condition happens two times in a row
					break;
				}
				else {
					// if for the first time just set condition to 1
					condition = 1;
				}
			} else {
				// if the stop condition is not true, restore the condition counter 
				condition = 0;
			}

		}
	}

	close(fd);

	return 0;
}
//if always successful it wil be infinit loop
// ASK?
