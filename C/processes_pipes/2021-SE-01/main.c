//#include <unistd.h>
//#include <fcntl.h>
#include <err.h>
#include <time.h>
//#include <stdlib.h>
//#include <sys/types.h>
//#include <pwd.h>
#include <sys/time.h>
//#include <string.h>
//#include <stdint.h>
//#include <stdio.h>


int main(int argc, char* argv[]) {

	struct timeval tv;
	if(gettimeofday(&tv, NULL) == -1) {
		err(1, "Error: running gettimeofday failed");
	}

}

// not done, too many 
