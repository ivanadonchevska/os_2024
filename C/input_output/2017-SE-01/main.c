#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

struct patch_structure {
	uint16_t offset;
	uint8_t original;
	uint8_t new;
}__attribute__((packed));

int main(int argc, char* argv[]) {
	
	if(argc != 4) {
		errx(1, "Error: invalid number of arguments");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1) {
		err(2, "Error: opening first file failed");
	}

	int fd2 = open(argv[2], O_RDONLY);
	if(fd2 == -1) {
		err(2, "Error: opening second file failed");
	}

	struct stat st;

	if(fstat(fd1, &st) == -1) {
		err(3, "Error: fstat first file failed");
	}

	int f1_size = st.st_size;
	if(f1_size % sizeof(uint8_t) != 0 || f1_size == 0) {
		err(4, "Error: invalid file");
	}

	if(fstat(fd2, &st) == -1) {
		err(3, "Error: fstat for second file failed");
	}
	
	int f2_size = st.st_size;
	if(f2_size % sizeof(uint8_t) != 0 || f2_size == 0) {
		err(4, "Error: invalid file");
	}

	if( f1_size != f2_size || f1_size > UINT16_MAX || f2_size > UINT16_MAX) {
		err(4, "Error: both files should have same size");
	}

	int patch = open(argv[3], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(patch == -1) {
		err(2, "Error: creating patch file failed");
	}

	int rs_1 = 0;
	int rs_2 = 0;
	uint8_t a;
	uint8_t b;

	struct patch_structure p_s;

	while(((rs_1 = read(fd1, &a, sizeof(a))) == sizeof(a)) && ((rs_2 = read(fd2, &b, sizeof(b))) == sizeof(b))) {
		if(a != b){
			p_s.offset = lseek(fd1, -1*sizeof(a), SEEK_CUR); //go one step backward
			if(lseek(fd1, 1*sizeof(a), SEEK_CUR) == -1) { // go one step forward, restore it to original position
				err(7, "Error: lseek failed");
			}

			p_s.original = a;
			p_s.new = b;

			if(write(patch, &p_s, sizeof(p_s)) != sizeof(patch)) {
				err(5, "Error: writing to patch file failed");
			}
		}
	}

	if(rs_1 == -1 || rs_2 == -1) {
		err(6, "Error: reading from files failed");
	}

	close(fd1);
	close(fd2);
	close(patch);
	
	return 0;
}
