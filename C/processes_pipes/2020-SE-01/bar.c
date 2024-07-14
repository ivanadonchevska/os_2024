#include <err.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {

	if (argc != 2) {
		errx(1, "Error: invalid number of arguments");
	}

	const char* command = argv[1];
	const char* pipe_name = "pipe";

	const pid_t pid = fork();
	if (pid == -1) {
		err(2, "Error: fork failed");
	}

	if(pid == 0) {
		int fd = open(pipe_name, O_RDONLY);
		if(fd == -1) {
			err(3, "Error: opening mkfifo failed");
		}

		if(dup2(fd, 0) == -1) {
			err(4, "Error: dup2 failed");
		}

		if(execl(command, command, (char*) NULL) == -1) {
			err(5, "Error: execl failed");
		}
	}

	int status;
	if(wait(&status) == -1) {
		err(6, "Error: could not wait for child process to finish");
	}

	if(!WIFEXITED(status)) {
		errx(7, "Error: child proces did not terminate normally");
	}

	if(WEXITSTATUS(status) != 0) {
		errx(8, "Error: child process finished with status code different from 0");
	}

	// unlink mkfifo
	if(unlink(pipe_name) == -1) {
		err(9, "Error: unlink mkfifo failed");
	}

	return 0;
}
