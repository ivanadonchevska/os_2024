#include <err.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
	
	int fd = open("example.txt", O_RDWR);
    if (fd == -1) {
        err(2, "Open failed");
    }

    // Move file pointer to the 7th byte (after "Hello, ")
    if (lseek(fd, 7, SEEK_SET) == -1) {
        err(3, "lseek failed");
    }

    // Write "Universe" starting at the 7th byte
    const char *text = "Universe";
    if (write(fd, text, 8) != 8) {
        err(4, "Writing failed");
    }
	
    close(fd);

    return 0;

}
