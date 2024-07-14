#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>    
#include <stdint.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void sys_err(int errc, int status, const char* msg);
int seconds_cond(int seconds, long long t_begin, long long t_end);
int exit_code(int status);

void sys_err(int errc, int status, const char* msg) {
	if(errc == -1) {
		err(status, msg);
	}
}

int exit_code(int status) {
	if(WIFEXITED(status)) {
		return WEXITSTATUS(status);
	} else if(WIFSIGNALED(status)) {
		return 129; 
	} else {
		errx(6, "unreachable");
	}
}

int seconds_cond(int seconds, long long t_begin, long long t_end) {
	return seconds > (t_end - t_begin); 
}

int main(int argc, char** argv) {

	if(argc < 3) {
		errx(1, "Arg count. Usage: ./main <seconds> <program> <args> ...");
	}

	int seconds = atoi(argv[1]);
	char* Q = argv[2];
	char** args = argv + 2;

	pid_t pid;
	int condition = 0;

	int fd;
	sys_err(
		(fd = open("run.log", O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR)),
		3,
		"could not create log file"
	);

	while(1) {
		sys_err((pid = fork()), 1, "could not fork"); 
		// In child
		if(pid == 0) {
			sys_err(execvp(Q, args), 2, "Could not exec");
		} else {
			time_t t_start = time(NULL);
			sys_err(t_start, 4, "could not take time");

			int status;
			sys_err(wait(&status), 5, "could not wait");

			time_t t_end = time(NULL);
			sys_err(t_end, 4, "could not take time");

			int code = exit_code(status);
			dprintf(fd, "%ld %ld %d\n", t_start, t_end, code);

			if(code != 0 && seconds_cond(seconds, t_start, t_end)) {
				if(condition) {
					break;
				} else {
					condition = 1;
				}
			} else {
				condition = 0;
			}

		}
	}

	close(fd);

	return 0;
}
