#include <err.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	
	if(argc != 2) {
		errx(1, "Invalid number of arguments");
	}

	const char* file = argv[1];

	int pf[2];

	if(pipe(pf) == -1) {
		err(2, "Error, pipe failed");
	}

	const pid_t pid = fork();
	if(pid == -1) {
		err(3, "Error, fork failed");
	}

	if (pid == 0) {
		// in child process
		// want to read from file instead from stdin, that's why we close 0->stdin
		close(pf[0]); // close unused read end
		// redirect stout to pipe's write end
		if(dup2(pf[1], 1) == -1) {
			err(4, "Error, dup2 failed");
		}

		if(execlp("cat", "cat", file, (char*) NULL) == -1) {
			err(5, "Error, execlp failed");
		}
	}

	close (pf[1]); // close unused write end
	// ask
	// do we need to check for wait status and ifexited etc.???

	// redirect stdin to pipe's read end
	if(dup2(pf[0], 0) == -1) {
		err(4, "Error, dup2 failed");
	}

	if(execlp("sort", "sort", (char*) NULL) == -1) {
		err(5, "Error, execlp failed");
	}

	close (pf[0]);

}
