#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <aio.h>
#include <errno.h>
#include <stdint.h>

struct header {
		uint32_t magic;
		uint8_t header_version;
		uint8_t data_version;
		uint16_t count;
		uint32_t reserved1;
		uint32_t reserved2;
	}__attribute__((packed));

	struct dataVersion0 {
		uint16_t offset;
		uint8_t original_byte;
		uint8_t new_byte;
	}__attribute__((packed));

	struct dataVersion1 {
		uint32_t offset;
		uint16_t original_word;
		uint16_t new_word;
	}__attribute__((packed));

int main (int argc, char* argv[])
{
	if (argc != 4){
		errx(1, "Invalid number of arguments");
	}

	struct header hd;
	struct dataVersion0 v0;
	struct dataVersion1 v1;

	struct stat st;

	if (stat(argv[1], &st) == -1){
		err(2, "Failed to use stat with file: %s", argv[1]);
	}

	if (st.st_size == 0){
		errx(3, "Empty file: %s", argv[1]);
	}

	int patch = open(argv[1], O_RDONLY);
	if (patch == -1){
		err(4, "Failed to open file: %s", argv[1]);
	}

	int read_size;

	if ((read_size = read(patch, &hd, sizeof(hd))) != sizeof(hd)){
		err(5, "Failed to read file: %s", argv[1]);
	}

	if (hd.magic != 0xEFBEADDE || hd.header_version != 0x01 || (hd.data_version != 0x00 && hd.data_version != 0x01)) {
		err(6, "HEADER: Wrong Format.");
	}

	int fd1 = open(argv[2], O_RDONLY);
	if (fd1 == -1){
		err(7, "Failed to open file: %s", argv[2]);
	}

	int fd2 = open(argv[3], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd2 == -1){
		err(8, "Failed to open file: %s", argv[3]);
	}

	if (hd.data_version == 0x00){
		uint8_t byte;
		struct stat st2;

		if (stat(argv[2], &st2) == -1){
			err(9, "Failed to use stat with file: %s", argv[2]);
		}

		if (st2.st_size == 0){
			errx(9, "Empty file: %s", argv[2]);
		}

		if (st2.st_size % sizeof(uint8_t) != 0){
			errx(10, "Corrupted file: %s", argv[2]);
		}

		while ((read_size = read(fd1, &byte, sizeof(byte))) == (ssize_t)(sizeof(byte))){
			if (write(fd2, &byte, sizeof(byte)) != (ssize_t)(sizeof(byte))){
				err(11, "Failed to write in file: %s", argv[3]);
			}
		}

		if (read_size == -1){
			err(12, "Failed to read from file: %s", argv[2]);
		}

		while ((read_size = read(patch, &v0, sizeof(v0))) == (ssize_t)(sizeof(v0))){
			if(lseek(fd2, v0.offset, SEEK_SET) == -1){
				err(13, "ERROR: lseek failed.");
			}

			if (read(fd2, &byte, sizeof(byte)) != (ssize_t)(sizeof(byte))){
				err(14, "Failed to read from file: %s", argv[3]);
			}

			if (byte != v0.original_byte){
				err(15, "Byte is not written or doesn't exist!");
			} else {
				if(lseek(fd2, -1, SEEK_CUR) == -1){
					err(16, "ERROR: lseek failed.");
				}

				if(write(fd2, &v0.new_byte, sizeof(v0.new_byte)) != (ssize_t)(sizeof(v0.new_byte))){
					err(17, "Failed to write to file: %s", argv[3]);
				}
			}
		}
	} else if (hd.data_version == 0x01) {
		uint16_t byte;
		struct stat st2;

		if (stat(argv[2], &st2) == -1){
			err(18, "Failed to use stat with file: %s", argv[2]);
		}

		if (st2.st_size == 0){
			errx(19, "Empty file: %s", argv[2]);
		}

		if (st2.st_size % sizeof(uint16_t) != 0){
			errx(20, "Corrupted file: %s", argv[2]);
		}

		while ((read_size = read(fd1, &byte, sizeof(byte))) == (ssize_t)(sizeof(byte))){
			if(write(fd2, &byte, sizeof(byte)) != (ssize_t)(sizeof(byte))){
				err(21, "Failed to write in file: %s", argv[3]);
			}
		}

		if (read_size == -1){
			err(22, "Failed to read from file: %s", argv[2]);
		}

		while ((read_size = read(patch, &v1, sizeof(v1))) == (ssize_t)(sizeof(v1))){
			if(lseek(fd2, v1.offset, SEEK_SET) == -1){
				err(23, "ERROR: lseek failed.");
			}

			if(read(fd2, &byte, sizeof(byte)) != (ssize_t)(sizeof(byte))){
				err(24, "Failed to read from file: %s", argv[3]);
			}

			if (byte != v1.original_word){
				err(25, "Byte is not written or doesn't exist!");
			} else {
				if(lseek(fd2, -1, SEEK_CUR) == -1){
					err(26, "ERROR: lseek failed.");
				}
				if(write(fd2, &v1.new_word, sizeof(v1.new_word)) != (ssize_t)(sizeof(v1.new_word))){
					err(27, "Failed to write to file: %s", argv[3]);
				}
			}
		}
	}
	close(patch);
	close(fd1);
	close(fd2);

	exit(0);
}
