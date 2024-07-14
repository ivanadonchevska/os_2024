#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

// find . -type f -printf "%f %T@\n" | sort -rn -t' ' -k2 | cut -d' ' -f1 | head -n1

void check_child_process(void);

void check_child_process(void) 
{
	int status;
	if(wait(&status) == -1) {
		err(6, "Error: could not wait for child process to finish");
	}

	if(!WIFEXITED(status)) {
		errx(7, "Error: child process did not terminate normally");
	}

	if(WEXITSTATUS(status) != 0) {
		errx(8, "Error: child process status is not 0");
	}
}

int main(int argc, char* argv[]) {
	
	if (argc != 2) {
		errx(1, "Error: invalid number of arguments");
	}

	const char* dir = argv[1];

	int pf[2];

	if(pipe(pf) == -1) {
		err(2, "Error: pipe failed");
	}

	const pid_t pid = fork();
	if(pid == -1) {
		err(3, "Error: fork failed");
	}

	if( pid == 0 ) {
		close(pf[0]);
		if(dup2(pf[1], 1) == -1) {
			err(4, "Error: dup2 failed");
		}
		
		if(( execlp("find", "find", dir, "-type", "f", "-printf", "%f %T@\n", (char*) NULL)) == -1) {
			err(5, "Error: execlp failed");
		}
	}

	// ASK??
	// is it good to check here if the first child has completed the work and if the status is ok and so on?
	check_child_process();
	close(pf[1]);
	
	int pf2[2];
	if(pipe(pf2) == -1) {
		err(2, "Error: pipe failed");
	}

	const pid_t pid2 = fork();
	if(pid2 == -1) {
		err(3, "Error: fork failed");
	}

	if(pid2 == 0) {
		close(pf2[0]);
		if(dup2(pf[0], 0) == -1) {
			err(4, "Error: dup2 failed");
		}

		if(dup2(pf2[1], 1) == -1) {
			err(4, "Error: dup2 failed");
		}

		if(execlp("sort", "sort", "-rn", "-k2", (char*) NULL) == -1) {
			err(5, "Error: execlp failed");
		}
	}

	check_child_process();
	close(pf[0]);
	close(pf2[1]);

	int pf3[2];
	if (pipe(pf3) == -1) {
		err(2, "Error: pipe failed");
	}

	const pid_t pid3 = fork();
	if(pid3 == -1) {
		err(3, "Error: fork failed");
	}

	if(pid3 == 0) {
		close(pf3[0]);

		if(dup2(pf2[0], 0) == -1) {
			err(4, "Error: dup2 failed");
		}

		if(dup2(pf3[1], 1) == -1) {
			err(4, "Error: dup2 failed");
		}

		if(execlp("cut", "cut", "-d", " ", "-f1", (char*) NULL) == -1) {
			err(5, "Error: execlp failed");
		}
	}

	check_child_process();
	close(pf3[1]);
	close(pf2[0]);

	if(dup2(pf3[0], 0) == -1) {
		err(4, "Error: dup2 failed");
	}

	if(execlp("head", "head", "-n", "1", (char*)NULL) == -1) {
		err(5, "Error: execlp failed");
	}

	exit(0);

}
