#include <unistd.h>
#include <stdio.h>

int main(void) {
    int p1, p2;
    p1 = fork();
    p2 = fork();
    printf("Hello world!\n");
}

/*
   Hello World will be printed 4 times. Here's why
   
   There is the initial process - Parent
   On first fork() it creates a new child process C1, so they become 2 processes
   On second fork(), parent creates one more child C3, but C1 also creates a child procss, so they become 4 processes
   each of them will execute the print command once


   Process tree will be something like this:

   			P
   		   / \
   		  C1 C2
   		  /
   		 C3

   	The fork() system call is used to create a new process in Unix-like operating systems. It creates a child process that is an exact copy of the parent process. The parent and child processes continue executing from the point where fork() was called. fork() returns a different value in each process: 0 in the child process and the child's Process ID (PID) in the parent process. If the call fails, it returns -1 in the parent process.

   */
