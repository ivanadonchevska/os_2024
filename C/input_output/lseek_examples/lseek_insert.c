#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

int main(void) {
    int fd = open("example.txt", O_RDWR); // Open file for read/write
    if (fd == -1) {
        err(1, "Error:");
    }

    // Move the file pointer to the insertion point (7th byte)
    int insert_pos = 7;

    // Get the size of the file
    int file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) {
       err(1, "Error:");
    }

    // Calculate the size of the buffer needed
    int buffer_size = file_size - insert_pos;

    // Allocate buffer
    char *buffer = (char *)malloc(buffer_size);
    if (buffer == NULL) {
        err(1, "Error:");
    }

    // Read the part of the file after the insertion point into the buffer
    if (lseek(fd, insert_pos, SEEK_SET) == -1) {
        err(1, "Error:");
    }

    if (read(fd, buffer, buffer_size) != buffer_size) {
        err(1, "Error:");
    }

    // Move the file pointer back to the insertion point
    if (lseek(fd, insert_pos, SEEK_SET) == -1) {
       err(1, "Error:");
    }

    // Write the new text at the insertion point
    const char *text = "test ";
    int text_length = strlen(text);
    if (write(fd, text, text_length) != text_length) {
       err(1, "Error:");
    }

    // Write the buffer content back to the file
    if (write(fd, buffer, buffer_size) != buffer_size) {
		err(1, "Error:");
    }

    // Clean up
    free(buffer);
    close(fd);
    return 0;
}

