#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
    pid_t p1, p2, p3;
    write(1, "A", 1);
    p1 = fork();
    if (p1 == 0) {
        write(2, "BB", 2);
        p2 = fork();
        if (p2 > 0) {
            write(1, "C", 1);
            p3 = fork();
            write(2, "DD", 2);
            if (p3 > 0) exit(0);
        }
    }
    write(1, "A", 1);
    exit(0);
}

/*
	- The initial process writes "A" to standard output.
	- The first fork() creates a child process (let's call it P1).
	- In the child process P1:
		- It writes "BB" to standard error.
		- It then forks again, creating another child process (let's call it P2).
		- In the parent process of this fork (still P1):
			- It writes "C" to standard output.
			- It forks again, creating another child process (let's call it P3).
			- It writes "DD" to standard error.
			- If p3 > 0, it exits.
Both the original process and the child processes write "A" to standard output before exiting.

//TODO
   */
