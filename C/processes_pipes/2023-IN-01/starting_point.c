#include <err.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

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

	int pf[NC + 1][2];
	for(int i = 0; i < NC + 1; i++) {
		if(pipe(pf[i]) == -1) {
			err(3, "Error: creating pipe number %d failed", i);
		}
	}
		
}
