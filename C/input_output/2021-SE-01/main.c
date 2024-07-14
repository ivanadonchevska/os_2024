#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

uint16_t encode_byte(uint8_t in_byte);
// purpose of this function is to encode each bit of the input byte into two bits in the output byte
// 
uint16_t encode_byte(uint8_t in_byte) {
	uint16_t out_byte = 0;
	
	// This loop iterates from 7 down to 0, processing each bit of the input byte starting from the most significant bit (bit 7) to the least significant bit (bit 0)
	// starts from first bit -> 1011 0110 -> starts from 1 and place it to the last place in out, with shifting
	for(int i = 7; i >= 0; i--) {

		//shifts the bitsvto the left by 2 positions, making space for two new bits
		out_byte <<= 2;
		
		// extracts the i'th bit of byte and check if it's 1
		if((in_byte >> i) & 1) {
			// if 1 -> 10
			out_byte |= 0x2;
		} else {
			// if 0 -> 01
			out_byte |= 0x1;
		}
	}
	return out_byte;
}

int main(int argc, char* argv[]) {

	if(argc != 3) {
		errx(1, "Error: invalid number of arguments");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1) {
		err(2, "Error: opening file failed");
	}

	int fd2 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(fd2 == -1) {
		err(2, "Error: creating output file failed");
	}

	int read_size = 0;
	uint8_t byte;

	while(( read_size = read(fd1, &byte, sizeof(byte))) == sizeof(byte)) {
		uint16_t encoded_byte = encode_byte(byte);

		if(write(fd2, &encoded_byte, sizeof(encoded_byte)) != sizeof(encoded_byte)) {
			err(4, "Error: writing to output file failed");
		}
	}

	if(read_size == -1) {
		err(3, "Error: reading from input file failed");
	}

	close(fd1);
	close(fd2);

	return 0;
}
