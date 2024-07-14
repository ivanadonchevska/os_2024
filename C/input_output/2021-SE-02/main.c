#include <err.h>
#include <fcntl.h>
#include <unistd.h>

uint8_t encode_byte(uint16_t in_byte);
uint8_t encode_byte(uint16_t in_byte) {
	uint8_t out_byte = 0;

	for(int i = 7; i >= 0; i--) {
		out_byte <<= 1;

		//extract the two bits from current position
		uint16_t two_bits = (in_byte >> (i * 2)) & 0x3;
		if(two_bits == 0x2) {
			out_byte |= 1;
		}
		else {
			out_byte |= 0;
		}
	}
	return out_byte;
}

int main(int argc, char* argv[]) {
	
	if(argc != 3) {
		errx(1, "Error: invalid number of arguments");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if(fd == -1) {
		err(2, "Error: opening file failed");
	}

	int fd2 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(fd2 == -1) {
		err(2, "Error: creating file failed");
	}

	int read_size = 0;
	uint16_t byte;

	while((read_size = read(fd1, &byte, sizeof(byte))) == sizeof(byte)) {
		uint8_t encoded = encode_byte(byte);

		if(write(fd2, &encoded, sizeof(encoded)) != sizeof(encoded)) {
			err(3, "Error: writing to output file failed");
		}
	}

	if(read_size == -1) {
		err(4, "Error: reading from file failed");
	}
	
	close(fd1);
	close(fd2);

	return 0;
}
