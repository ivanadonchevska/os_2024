#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {

	dprintf(1, "argc: %d\n", argc);

	const char* f1 = argv[1];
	int len = strlen(f1);
	dprintf(1, "len: %d\n", len);

	char arr[256];

	read(0, &arr, sizeof(arr));

	for(long unsigned int i = 0; i < strlen(arr) - 1; i++) {
		if(arr[i] == f1[0]) {
			dprintf(1, "equal\n");
		}
		else {
			dprintf(1, "not equal\n");
		}
		//write(1, &arr[i], sizeof(arr[i]));
	}

}
