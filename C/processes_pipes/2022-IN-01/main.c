#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

void wait_child_process(void);
void wait_child_process(void) {
	int status;
	if(wait(&status) == -1) {
		err(6, "Error: could not wait for child process to finish");
	}

	if(!WIFEXITED(status)) {
		errx(7, "Error: child did not terminate normally");
	}

	if(WEXITSTATUS(status) != 0) {
		errx(8, "Error: child terminated with status code different from 0");
	}
}

int main(int argc, char* argv[]) {
	
	if (argc != 3) {
		errx(1, "Error: invalid number of arguments");
	}

	if( strlen(argv[1]) != 1 || strlen(argv[2]) != 1) {
		errx(2, "Error: invalid arguments length");
	}

	int N = argv[1][0] - '0';
	int D = argv[2][0] - '0';

	const char* ding = "DING ";
	const char* dong = "DONG\n";
	
	int pf_parent[2];
	int pf_child[2];

	if (pipe(pf_parent) == -1) {
		err(3, "Error: pipe failed");
	}

	if (pipe(pf_child) == -1) {
		err(3, "Error: pipe failed");
	}

	const pid_t pid = fork();
	if(pid == -1) {
		err(4, "Error: fork failed");
	}
	
	int signal_write = 1;
	int signal_read;


	if (pid == 0) {
		// in child process wait to read the signal from parent pipe
		// write dong to stdout
		// send signal to parent that can start sleeping for D seconds
	
		close(pf_parent[1]); // close unused parent end for writing
		close(pf_child[0]); // close unused child end for reading
	
		for(int i = 0; i < N; i++){
			if(read(pf_parent[0], &signal_read, sizeof(signal_read)) != sizeof(signal_read)) {
				err(6, "Error: reading from parent's pipe failed");
			}

			if (write(1, dong, (long int)strlen(dong)) != (long int)strlen(dong)) {
				err(5, "Error: writing dong to stdout failed");
			}	

			if( write(pf_child[1], &signal_write, sizeof(signal_write)) != sizeof(signal_write)) {
				err(5, "Error: writing to child's pipe failed");
			}
		}
	
		close(pf_parent[0]);
		close(pf_child[1]);

	} else {
		//in parent process
		// all this instructions to happen N times
		//write ding to stdout
		// send signal to child process, that can start executing
		// wait for child process to finish
		// sleep for D

		close(pf_parent[0]); // close unused parent end for reading
		close(pf_child[1]); // close unused child end for writing

		for(int i = 0; i < N; i++) {
			if ( write(1, ding, (long int)strlen(ding)) != (long int)strlen(ding)) {
				err(5, "Error: writing ding to stdout failed");
			}

			if(write(pf_parent[1], &signal_write, sizeof(signal_write)) != sizeof(signal_write)) {
				err(5, "Error: writing to parent's pipe failed");
			}
			
			if(read(pf_child[0], &signal_read, sizeof(signal_read)) != sizeof(signal_read)) {
				err(6, "Error: reading from child's pipe failed");
			}

			sleep(D);
		}

		close(pf_parent[1]);
		close(pf_child[0]);
	}
	
	// no need for waiting child implementation, because the scheduling is done by sending and reading signals in both parent and child pipes
	//wait_child_process();
	//dprintf(1, "Waited for child");
	
	return 0;
}
