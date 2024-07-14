#include <err.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
	
	if(argc != 2) {
		errx(1, "Error: invalid number of arguments");
	}

	const char* file_name = argv[1];
	const char* pipe_name = "pipe";

	if(mkfifo(pipe_name, 0666) == -1) {
		err(2, "Error: mkfifo failed");
	}

	const pid_t pid = fork();
	if (pid == -1) {
		err(3, "Error: fork failed");
	}

	if (pid == 0) {
		int fd = open(pipe_name, O_WRONLY);
		if(fd == -1) {
			err(4, "Error: opening mkfifo failed");
		}

		if(dup2(fd, 1) == -1) {
			err(5, "Error: dup2 failed");
		}

		if(execlp("cat", "cat", file_name, (char*)NULL) == -1) {
			err(6, "Error: execlp failed");
		}
	}

	//check for status

	int status;
	if(wait(&status) == -1) {
		err(7, "Error: could not wait for child process to finish");
	}

	if(WIFEXITED(status) != 0) {
		errx(8, "Error: child process did not terminate normally");
	}

	if(WEXITSTATUS(status) == -1) {
		errx(9, "Error: child process finished with status code different from 0");
	}

	return 0;
}
