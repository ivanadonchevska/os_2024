#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/wait.h>


static void exec(const char* cmd, const char* arg1, const char* arg2, int count) {
	int pid = fork();
	if(pid == -1) {
		err(10, "Could not fork");
	}

	if(pid == 0) {
		if(count == 1) {
			if(execlp(cmd, cmd, arg1, NULL) == -1) {
				err(11, "Could not exec");
			}
		} else {
			if(execlp(cmd, cmd, arg1, arg2, NULL) == -1) {
				err(11, "Could not exec");
			}
		}
	} else {
		wait(NULL);
	}
}

int main(int argc, char** argv) {
	
	const char* cmd = "echo";

	switch(argc) {
		case 1: break;
		case 2:
			if(strlen(argv[1]) > 4) {
				errx(1, "Maximum allowed command length: 4");
			}
			cmd = argv[1];
			break;
		default: errx(2, "One/zero arguments expected");
	}

	char args[2][5];
	for(int i = 0; i < 5; i++) {
		args[0][i] = '\0';
		args[1][i] = '\0';
	}

	uint8_t arg = 0;
	int curr = 0;

	uint8_t byte;
	int result;
	while((result = read(0, &byte, 1)) != 0) {
		if(curr > 4) {
			errx(3, "Maximum length of arg: 4");
		}

		if(byte == '\n' || byte == ' ') {
			arg++;
			if(arg == 2) {
				exec(cmd, args[0], args[1], 2);		
				
				for(int i = 0; i < 5; i++) {
					args[0][i] = '\0';
					args[1][i] = '\0';
				}

				arg = 0;
			}
			
			curr = 0;
			continue;
		} 

		args[arg][curr] = byte;
		curr++;
	}

	if(args[0][0] != '\0') {
		if(args[1][0] != '\0') {
			exec(cmd, args[0], args[1], 2);
		} else {
			exec(cmd, args[0], NULL, 1);
		}
	}

	return 0;	
}

