#include <err.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	
	if(argc != 3) {
		errx(1, "Error: invalid number of arguments");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1) {
		err(2, "Error: opening file failed");
	}

	struct stat st;
	if(fstat(fd1, &st) == -1) {
		err(3, "Error: fstat failed");
	}
	
	int size = st.st_size / sizeof(uint16_t);

	if(st.st_size % sizeof(uint16_t) != 0 || size > 524288) {
		err(4, "Error: invalid file size");
	}

	int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if(fd2 == -1) {
		err(2, "Error: creating output file failed");
	}
	
	dprintf(fd2, "#include <stdio.h>\n");
	dprintf(fd2, "uint32_t arrN = %d;\n", size);
	dprintf(fd2, "uint16_t arr[] = {");
	int rs = 0;
	uint16_t buff;
	while((rs = read(fd1, &buff, sizeof(buff))) == sizeof(buff)) {
		dprintf(fd2, "%d,", buff);
	}	

	if(rs == -1) {
		err(5, "Error: reading from file failed");
	}

	dprintf(fd2, "};");

	close(fd1);
	close(fd2);

	return 0;
}

// should make it with write, byte by byte, in fd2, instead of using dprintf
