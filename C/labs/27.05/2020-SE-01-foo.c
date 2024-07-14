#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>    
#include <stdint.h>
#include <err.h>
#include <stdio.h>

int main(int argc, char** argv) {

  if(argc != 2) {
    errx(2, "arg count. Usage: ./main <file>");
  }

  printf("!\n");
  if(mkfifo("temp", S_IRWXU) == -1) {
    err(3, "mkfifo");
  }

  printf("!\n");
  int fd = open("temp", O_WRONLY);
  if(fd == -1) {
    err(5, "opening pipe");
  }

  printf("!\n");
  pid_t pid = fork();
  if(pid == -1) {
    err(8, "forking");
  }

  printf("!\n");
  if(pid == 0) {
	if(dup2(fd, 1) == -1) {
		err(6, "dup-dee dup");
	}

    if(execlp("cat", "cat", argv[1], (char*) NULL) == -1) {
      err(4, "exec");
    }
  }

  printf("!\n");
  //if(unlink("temp") == -1) {
  //  err(9, "unlinking");
  //}

  return 0;
}
