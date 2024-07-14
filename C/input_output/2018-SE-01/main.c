#include <err.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

	if(argc == 1) {
		errx(1, "Error: invalid number of arguments");
	}

	if(strcmp(argv[1], "-d") == 0 ) {
		const char* set1 = argv[2];
		char input_str[256];
		int read_size = 0;
		read_size = read(0, &input_str, sizeof(input_str));

		for(int i = 0; i < read_size; i++) {
			// if not permission for using strchr, make own
			if(strchr(set1, input_str[i]) != NULL) {
				continue;
			}
			write(1, &input_str[i], sizeof(input_str[i]));
		}
	} else if(strcmp(argv[1], "-s") == 0) {
		// can check if only 2 arguments passed and other tests
		const char* set1 = argv[2];
		char input_str[256];
		int read_size = 0;
		read_size = read(0, &input_str, sizeof(input_str));

		int already_written = 0;
		for(int i = 0; i < read_size; i++) {
			// TODO 
			// this comparation is not working, check how to compare character by character
			if(set1 == &input_str[i]) {
				if(already_written == 1) {
					continue;
				} else {
					write(1, &input_str[i], sizeof(input_str[i]));
					already_written = 1;

					continue;
				}
			} else {
				already_written = 0;
			}
			write(1, &input_str[i], sizeof(input_str[i]));
		}

	}
	else if( (strcmp(argv[1], "-s") != 0) && (strcmp(argv[1], "-d") != 0)) {
		//ckeck if two arguments given
		// check if same size
		// read each character from input and check if strchr(ch, set1)
		// if true, get the index in set1 and write to output set2[index]
		// otherwise just write the character to the output
	}

	return 0;
}
