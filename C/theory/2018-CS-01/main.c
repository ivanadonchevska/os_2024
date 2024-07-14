#include <unistd.h>
#include <string.h>

int main(void) {
    const char *buff = "Hello world!\n";
    int p;
    if (fork() == 0) 
        write(1, buff, strlen(buff));
    p = fork();
    write(1, buff, strlen(buff));
}

// total number of prints will be 5
// first one from first fork, and wirst child
// after that on line 9 where the parent process is forking again it creates nwe child, but the first child also create a child so they become 1 parent child1 child2 child3
// and 4 more times buff is printed

/*
	- Both the parent and the first child proceed to the next fork().
	- When the parent executes p = fork(), it creates another child process (second child). Now, there are three processes: the parent, the first child, and the second child.
	- When the first child executes p = fork(), it creates another child process (third child). Now, there are four processes: the parent, the first child, the second child, and the third child.
	- Each of these four processes executes write(1, buff, strlen(buff)), printing "Hello world!" once each.*/

// parent P, creates first child C1 with first fork
// on second fork both C1 and P create a child, C2 and C3
/*
		P
   	   / \
 	 C1   C2
	 /
	C3
*/
