#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>    
#include <stdint.h>
#include <err.h>

int main(int argc, char** argv) {

  if(argc != 2)
    errx(1, "arg count");

  int fd = open("temp", O_RDONLY);
  if(fd == -1)
    err(1, "could not open pipe");

  if(dup2(fd, 0) == -1) {
    err(2, "could not dup");
  }

  if(execl(argv[1], argv[1], (char*) NULL) == -1) {
    err(3, "could not exec");
  }

  return 0;
}
