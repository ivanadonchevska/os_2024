#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <stdio.h>

#define MAX_FILENAME_LEN 1024

bool read_filename(int fd, char* filename);
bool read_filename(int fd, char* filename) {
	for(int i = 0; i < MAX_FILENAME_LEN; i++) {
		int rs = read(fd, &filename[i], 1);
		if(rs == -1) {
			err(6, "Error: reading data from file %s failed", filename);
		}

		if(rs == 0) {
			if(i != 0) {
				err(8, "incomplete data from find");
			}
			return false;
		}

		// if new line is found, change it with '\0' and return true
		if(filename[i] == '\n') {
			filename[i] = '\0';
			return true;
		}
	}

	errx(7, "Given longer filename than can handle");
}

int main(int argc, char* argv[]) {
	
	// from parent make pipe in which the found files will be stored
	// fork()
	// dup2(fd[1], 1);
	// execlp("find", "find", dirname, "-type", "f", "!", "-iname", "*.hash", (char*)NULL)

	// for each file, create new child process and execlp md5sum ..
	// set buffer size and write byte by byte in thus buffer size, and on each read byte check if it's != '\n' if not != then add '\0' and that's the filename
	// open new file filename.hash, dup2(fd[1], 1);
	// and execlp will write the result in created file
	
	// in parent process should wait for all children to finish, something like
	// for(int i = 0; i < number_of_files; i++) {
	// wait(NULL)

	if( argc != 2 ) {
		errx(1, "Error: invalid number of arguments");
	}
	
	const char* dirname = argv[1];
	int pf1[2];
	if(pipe(pf1) == -1) {
		err(2, "Error: pipe failed");
	}

	const pid_t pid1 = fork(); // the child process that finds all files to work with
	if(pid1 == -1) {
		err(3, "Error: pipe failed");
	}

	if(pid1 == 0) {
		close(pf1[0]);
		if(dup2(pf1[1], 1) == -1) {
			err(4, "Error: dup2 failed");
		}

		if(execlp("find", "find", dirname, "-type", "f", "!", "-iname", "*.hash", (char*)NULL) == -1) {
			err(5, "Error: execlp failed");
		}
	}

	close(pf1[1]); // done with writing, can close the write end

	char filename[MAX_FILENAME_LEN];

	int cnt = 1;
	while(read_filename(pf1[0], filename)) {
	//	dprintf(1, "%s\n", filename);		
		const pid_t pid2 = fork(); // not good to be called pid2, but let's make it work first
		if(pid2 == -1) {
			err(3, "Error: fork failed");
		}
		
		cnt++;
		if(pid2 == 0) {
			close(pf1[1]); 

			int len = strlen(filename);
//			dprintf(1, "len: %d\n", len); 

    		const char suffix[] = ".hash";
			int suffix_len = strlen(suffix);

			char output_file[len + suffix_len + 1];

			strcpy(output_file, filename);
    		strcat(output_file, suffix);

			//dprintf(1, "%s \n", output_file);

			int fd_out = open(output_file, O_RDWR | O_CREAT | O_TRUNC, 0666);
			if(fd_out == -1) {
				err(9, "Error: creating output file failed");
			}
	
			if(dup2(fd_out, 1) == -1) {
				err(4, "Error: dup2 failed");
			}

			if(execlp("md5sum", "md5sum", filename, (char*)NULL) == -1) {
				err(5, "Error: execlp failed");
			}

		}

	}

	//dprintf(1, "cnt: %d\n", cnt);
	for(int i = 0; i < cnt; i++) {
		wait(NULL);
	}

	close(pf1[1]);
	return 0;
}
