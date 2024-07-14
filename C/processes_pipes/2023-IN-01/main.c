#include <err.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

#define WORDS_COUNT 3

// add new lines, just for testing purpose
const char L[3][4] = {"tic\n", "tac\n", "toe\n"};

int main(int argc, char* argv[]) {
	
	if( argc != 3) {
		errx(1, "Error: invalid number of arguments");
	}

	char* endptr;
	long NC = strtol(argv[1], &endptr, 10);
	long WC = strtol(argv[2], &endptr, 10);
	
	if ( NC < 1 || NC > 7) {
		err(2, "Error: first argument is not in valid range");
	}

	if( WC < 1 || WC > 35 ) {
		err(2, "Error: second argument is not in valid range");
	}

	// create NC + 1 number of pipes
	int pf[NC + 1][2];
	for(int i = 0; i < NC + 1; i++) {
		if(pipe(pf[i]) == -1) {
			err(3, "Error: creating pipe number %d failed", i);
		}
	}
	
	pid_t pid[NC];
	int child_count = 0;
	for(int i = 0; i < NC; i++) {
		pid[i] = fork();
		if(pid[i] == -1) {
			err(4, "Error: fork failed. Creating child process number %d failed", i);
		}

		if(pid[i] == 0) {
			// close all unused pipe ends in current child process
			// leave open only reading end from previous process
			// and writing end from current process
			for (int j = 0; j < NC + 1; j++) {
				if( i != j) {
					close(pf[j][0]);
				}
				if(i + 1 != j) {
					close(pf[j][1]);
				}
			}

			int cnt; // cnt is used as signal and to keep track which word is next
			// whhile there are signals in pipes, repeat the process
			while(1) {
				// read the signal from current pipe
				if( read(pf[i][0], &cnt, sizeof(cnt)) != sizeof(cnt)) {
					// think how to handle both err and break???
					//err(5, "Error: reading signal from child process pipe failed");
					break;
				}
				// stop if the the WC is reached
				if(cnt >= WC) {
					break;
				}

				dprintf(1, "\nChild\n");
				if( write(1, L[cnt % WORDS_COUNT], 4) != 4) {
					err(6, "Error: writing to stdout, from child process, failed");
				}
				cnt++;
	
				// write the signal to next pipe
				if( write(pf[i+1][1], &cnt, sizeof(cnt)) != sizeof(cnt)) {
					err(6, "Error: writing to pipe, from child process, failed");
				}	
			}
			// close used ends
			close(pf[i][0]);
			close(pf[i+i][1]);
			
			// ensure that child process stops here and does not continue to create another processes
			return 0;
		} else {
			child_count++;
		}
	}

	// parent process
	// parent will read only from last child process pipe and write to it's own
	for(int i = 0; i < NC + 1;  i++) {
		if(i != NC) {
			close(pf[i][0]);
		}
		if(i != 0) {
			close(pf[i][1]);
		}
	}
	
	int cnt = 0;
	while(cnt < WC) {
		dprintf(1, "\nParent\n");
		if(write(1, L[cnt % WORDS_COUNT ], 4) != 4) {
			err(6, "Error: writing to stdout, from parent process, failed");
		}
		cnt++;
	
		// send signal to first child process
		if (write(pf[0][1], &cnt, sizeof(cnt)) != sizeof(cnt)) {
			err(6, "Error: writing to pipe, from parent process, failed");
		}
		
		// read signal from last child process
		if(read(pf[NC][0], &cnt, sizeof(cnt)) != sizeof(cnt)) {
			// think how to handle both???
			//err(5, "Error: reading from pipe failed");
			break;
		}
	}
	// close used ends once when finished
	close(pf[0][1]);
	close(pf[NC][0]);

	// not sure if this code for waiting is needed, when processes are scheduled by using signals?? -> YES IT UIS!!!
	// wait for all the child processes to finish execution properly
	for(int i = 0; i < NC; i++) {
		wait(NULL);
	}

	dprintf(1, "\nTotal child processes created: %d\n", child_count);
	
	return 0;
	
}
