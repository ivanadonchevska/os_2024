#include <err.h>
#include <unistd.h>

// cut -d':' -f7 /etc/passwd | sort | uniq | sort -n -k1,1

int main(void) {
	
	int pfd_1[2];
	if(pipe(pfd_1) == -1) {
		err(1, "Error: pipe failed");
	}

	const pid_t pid_1 = fork();
	if(pid_1 == -1) {
		err(2, "Error: fork failed");
	}

	if(pid_1 == 0) {
		// in child
		close(pfd_1[0]); // close unuse pipe's end for reading
		if(dup2(pfd_1[1], 1) == -1) {
			err(3, "Error: dup2 failed");
		}

		if(execlp("cut", "cut", "-d:", "-f7", "/etc/passwd", (char*)NULL) == -1) {
			err(4, "Error: execp failed");
		}
	}

	// in parent process, close the unuse end for writing
	close(pfd_1[1]);
	
	// ask
	// should check for status and ifexited ... everytime between two child processes???

	int pfd_2[2];
	if(pipe(pfd_2) == -1) {
		err(1, "Error: pipe failed");
	}

	const pid_t pid_2 = fork();
	if(pid_2 == -1) {
		err(2, "Error: fork failed");
	}

	if(pid_2 == 0) {
		close(pfd_1[1]); // close first pfd for writing 
		close(pfd_2[0]); // close second pfd for reading

		if(dup2(pfd_1[0], 0) == -1) {
			err(3, "Error: dup2 failed");
		}

		if(dup2(pfd_2[1], 1) == -1) {
			err(3, "Error: dup2 failed");
		}

		if(execlp("sort", "sort", (char*)NULL) == -1) {
			err(4, "Error: execlp failed");
		}

	}

	// in parent process close end for writing
	// should also close pdf_1[0] ?? ask
	close(pfd_2[1]);

	
	int pfd_3[2];
	if(pipe(pfd_3) == -1) {
		err(1, "Error: pipe failed");
	}

	const pid_t pid_3 = fork();
	if(pid_3 == -1) {
		err(2, "Error: fork failed");
	}

	if(pid_3 == 0) {
		close(pfd_3[0]);
		// pfd_2[1]  already closed in parent?? is it okay?
	
		if(dup2(pfd_2[0], 0) == -1) {
			err(3, "Error: dup2 failed");
		}

		if(dup2(pfd_3[1], 1) == -1) {
			err(3, "Error: dup2 failed");
		}

		if(execlp("uniq", "uniq", "-c", (char*)NULL)  == -1) {
			err(4, "Error: execlp failed");
		}

	}
	
	// in parent process
	close(pfd_3[1]);
	// or should close pfd_1[0] here?
	
	close(pfd_2[0]);

	if(dup2(pfd_3[0], 0) == -1) {
		err(3, "Error: dup2 failed");
	}

	if(execlp("sort", "sort", "-n", "-k1,1", (char*)NULL) == -1) {
		err(4, "Error: execlp failed");
	}

	close(pfd_3[0]);
	
}
