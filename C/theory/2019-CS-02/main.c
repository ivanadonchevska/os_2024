#include <unistd.h>
#include <stdio.h>

int main(void) {
    int p1, p2, p3;
    p1 = fork();
    if (p1 == 0) {
        p2 = fork();
        if (p2 > 0) p3 = fork();
    }
    printf("Hello world!\n");
    return 0;
}

/*
	with p1 = fork(), first child is created and enters the inner block
	executes p2 = fork(), so they become 3 processes
	and only the parent of this last fork, creates another child, but the child pard of it does not create any additional processes

	Thus, we end up with four processes in total:
	P1: The original process.	
	P2: Child of P1.
	P3: Child of P2 (created in the inner block).
	P4: Child of P2 from the second fork, but not creating any further processes.
	
	Tree structure:
			P
		   /
		  C1
		 / \
		C2 C3
   */
