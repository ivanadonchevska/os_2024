#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>    
#include <stdint.h>
#include <err.h>

/*
  cat /etc/passwd | cut -d':' -f7 | sort | uniq -c | sort -n -k1,1 
 */

void syserr(const int, const uint8_t, const char*);
void exec_proc(char*, char* const*, const int, const int);

void syserr(const int status, const uint8_t code, const char* msg) {
  if(status == -1) {
    err(code, msg);
  }
}

void exec_proc(char* pname, char* const args[], const int rend, const int wend) {
  syserr(dup2(rend, 0), 6, "could not dup read end");
  syserr(dup2(wend, 1), 7, "could not dup write end");
  syserr(execvp(pname, args), 5, "could not exec");
}

int main(void) {

  int cat_pfd[2];

  char** cmd[];
  cmd[0] = { "cat", "cat", "/etc/passwd", NULL };
  cmd[1] = { "cut", "cut", "-d:", "-f7", NULL };
  .
  .
  .
  .

	// First iteration
  for(int i = 1; i < 4; i++) {
	
	exec_proc(cmd[i][0], cmd[i], 
  }

  // Final iteration
  	

  syserr(pipe(cat_pfd), 2, "could not pipe for cat");

  pid_t cat_pid;
  syserr((cat_pid = fork()), 3, "could not fork for cat");
  
  if(cat_pid == 0) {
    syserr(close(cat_pfd[0]), 4, "could not close read end for cat pipe");
    char pname[] = "cat";
    char arg1[] = "/etc/passwd";
    char* const args[] = { pname, arg1, (char*) NULL };
    exec_proc(pname, args, 0, cat_pfd[1]);
  } else {
    syserr(close(cat_pfd[1]), 5, "could not close write end for cat pipe");
  }

  int cut_pfd[2];
  syserr(pipe(cut_pfd), 2, "could not pipe for cut");

  pid_t cut_pid;
  syserr((cut_pid = fork()), 3, "could not fork for cut");
  
  if(cut_pid == 0) {
    syserr(close(cut_pfd[0]), 6, "could not close read end for cut pipe");
    char pname[] = "cut";
    char arg1[] = "-d:";
    char arg2[] = "-f7";
    char* const args[] = { pname, arg1, arg2, (char*) NULL };
    //exec_proc(pname, args, cat_pfd[0], cut_pfd[1]);
    exec_proc(pname, args, cat_pfd[0], cut_pfd[1]);
  } else {
    syserr(close(cut_pfd[1]), 6, "could not close read end for cut pipe");
  }

  int sort_pipe[2];
  syserr(pipe(sort_pipe), 8, "could not pipe sort");

  pid_t sort_pid;
  syserr((sort_pid = fork()), 4, "Couldnnot fork sort");

  if(sort_pid == 0) {
    char pname[] = "sort";
    char* const args[] = { pname, (char*) NULL };
    exec_proc(pname, args, cut_pfd[0], sort_pipe[1]);
  } else {
    syserr(close(sort_pipe[1]), 6, "could not close write end for sort pipe");
  }

  int uniq_pipe[2];
  syserr(pipe(uniq_pipe), 7, "could not pipe uniq");

  pid_t uniq_pid;
  syserr((uniq_pid = fork()), 4, "Couldnnot fork sort");

  if(uniq_pid == 0) {
    char pname[] = "uniq";
    char arg1[] = "-c";
    char* const args[] = { pname, arg1, (char*) NULL };
    //exec_proc(pname, args, cut_pfd[0], uniq_pipe[1]);
    exec_proc(pname, args, sort_pipe[0], uniq_pipe[1]);
  } else {
    syserr(close(uniq_pipe[1]), 6, "could not close write end for sort pipe");
  }

  char pname[] = "sort";
  char arg1[] = "-n";
  char arg2[] = "-k1,1";
  char* const args[] = { pname, arg1, arg2, (char *) NULL };
  exec_proc(pname, args, uniq_pipe[0], 1);

  return 1;
}

