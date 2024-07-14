#include <err.h>

int main(int argc, char* argv[]) {

	if(argc != 3) {
		errx(1, "Error: invalid number of arguments");
	}

	// open files, fstat, check for it's size
	// check each file's header values

	// iterate through comparator file, reading struct comparator
	// and check if reserved != 0 if type != 0 && != 1

	struct comparator_data c_d;

	while(read(fd_comparator, &s_d, sizeof(cd)) == sizeof(cd)) {
		//proverkite

		uint64_t num1;
		uint64_t num2;

		lseek(fd_data, s_d.offset1, SEEK_SET);
		read(fd_data, &num1, sizeof(num1));

		lseek(fd_data, s_d.offset2, SEEK_SET);
		read(fd_data, &num2, sizeof(num2));

		if((num1 > num2 && s_d.type == 1) || (num1 < num2 && s_d.type == 0) || num1 == num2) {
			// all good just continue
			continue;
		} else {
			// should make inplace swap
			// just handle where lseek should be, based on element and it's offset
			// write the right number there
			// lseek again to the other element's offset
			// and write right number
		}

	}
}
