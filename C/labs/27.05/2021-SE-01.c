#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>

static uint16_t enc_byte(uint8_t in_b) {
    uint16_t in = in_b;
    uint16_t out = 0;
    for (int i = 7; i >= 0; i--) {
        out <<= 2;
        if ((in >> i) & 1) {
            out |= 0x2;
        } else {
            out |= 0x1;
        }
    }
    return out;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "need 2 arguments");
    }

    int fd_in = open(argv[1], O_RDONLY);
    if (fd_in < 0) {
        err(1, "unable to open %s", argv[1]);
    }

    int fd_out = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd_out < 0) {
        err(1, "unable to open %s", argv[2]);
    }

	int read_result;
	uint8_t byte;
    while((read_result = read(fd_in, &byte, 1)) > 0) {
        uint16_t encoded_byte = enc_byte(byte);
        if (write(fd_out, &encoded_byte, sizeof(encoded_byte)) < 0) {
            err(1, "unable to write");
        }
    }

    if(read_result == -1) {
		err(2, "ERROR: Read");
    }

    close(fd_in);
    close(fd_out);
}

