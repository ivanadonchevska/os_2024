#include <err.h>
#include <unistd.h>
//#include <


int main(int argc, char* argv[]) {
	
	if( argc != 2) {
		errx(1, "Error: invalid number of arguments");
	}

	const char* dir_name = argv[1];

	int pf[2];
	if(pipe(pf) == -1) {
		err(2, "Error: pipe failed");
	}

	const pid_t pid = fork();
	if(pid == -1) {
		err(3, "Error: fork failed");
	}

	if(pid == 0) {
		close(pf[0]);
		if(dup2(pf[1], 1) == -1) {
			err(4, "Error: dup2 in child process failed");
		}

		if(execlp("find", "find", dir_name, "-type", "f", "!", "-iname", "*.hash", (char*)NULL) == -1) {
			err(5, "Error: execlp in child process failed");
		}

		close(pf[1]);
	}

	//parent process
	close(pf[1]);
	if(dup2(pf[0], 0) == -1) {
		err(4, "Error: dup2 in parent process failed");
	}

	if(execlp("xargs", "xargs", "-I", "{}", "sh", "-c", "md5sum \"{}\">\"{}.hash\"", (char*)NULL) == -1) {
		err(5, "Error: execlp in parent failed");
	}

	close(pf[0]);

	return 0;
}
