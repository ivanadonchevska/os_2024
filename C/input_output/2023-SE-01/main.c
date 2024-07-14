#include <err.h>

int main(int argc, char* argv[]) {
	
	if(argc != 3) {
		errx(1, "Error: invalid number of arguments");
	}

	int fd_in = open(argv[1], O_RDONLY);
	if(fd_in == -1) {
		err(2, "Error: opening failed");
	}

	int fd_out = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(fd_out == -1) {
		err(2, "Error: ioening failed");
	}

	int file_size = lseek(fd_in, 0, SEEK_END);

	for(int i = 0; i < file_size; i++) {
		//define one counter = 0, to know how many bytes have been read, if written message
		//read byte byte and for each check the condition
		// first check if i == 0x55, if it's start of the message cnt++
		// then check if the next one(i+1) is valid uint8_t number cnt++
		// checksum = start ^ N;
		// if yes iterate N-3 times and put each element in array
		//calculate checksum of each one, like checkcum ^= data
		// here counter will be cnt += N
		// and at the end read one more byte that should be for checksum and if checksum == calculatec checksum then it's valid message
		

		//write(fd2, start, ..)
		// N, iterate through each element from array and write it and also write the checksum
		//cnt++;

		// and if valid message is written then set i to be i=cnt, to not iterate through each byte, that is already written
		// it will fail somehow, because won't pass the condition for starting byte, but it's matter of preferences
		// on each if case, when the statement is not true, should have continue, to go to the next byte and start checking from beginning


	}

	close(fd_in);
	close(fd_out);

	return 0;
}
