#include <err.h>
#include <string.h>
#include <stdint.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

static void exec(const char* cmd, const char* arg1, const char* arg2, int count) {
	const pid_t pid = fork();
	if (pid == -1) {
		err(4, "Error: fork failed");
	}

	if ( pid == 0) {
		if(count == 1) {
			if(execlp(cmd, cmd, arg1, (char*) NULL) == -1) {
				err(5, "Error: execlp failed");
			}
		} else {
			if(execlp(cmd, cmd, arg1, arg2, (char*) NULL) == -1) {
				err(6, "Error: execlp failed");
			}

		}
	}
	else {
		wait(NULL);
	}
}

int main(int argc, char* argv[]) {

	const char* command = "echo";

	switch(argc) {
		case 1:
			break;
		case 2: 
			if (strlen(argv[1]) > 4) {
				errx(1, "Maximum allowed command length is 4");
			}
			command = argv[1];
			break;
		default:
			errx(2, "One or zero arguments expected");
	}

	char args[2][5];
	for(int i = 0; i < 5; i++) {
		args[0][i] = '\0';
		args[1][i] = '\0';
	}

	uint8_t arg = 0;
	int current_cmd_byte = 0;

	uint8_t byte;
	int bytes_read;

	while(( bytes_read = read(0, &byte, 1)) != 0 ) {
		if( current_cmd_byte > 4) {
			errx(3, "Maxiumum allowed argument length is 4");
		}

		if( byte == '\n' || byte == ' ') {
			arg++;	
			if(arg == 2) {
				exec(command, args[0], args[1], 2);

				for(int i = 0; i < 5; i++) {
					args[0][i] = '\0';
					args[1][i] = '\0';
				}

				arg = 0;
			}

			current_cmd_byte = 0;
			continue;
		}
	
		args[arg][current_cmd_byte] = byte;
		current_cmd_byte++;
	}

	if (args[0][0] != '\0') {
		if(args[1][0] != '\0') {
			exec(command, args[0], args[1], 2);
		} else {
			exec(command, args[0], NULL, 1);
		}
	}

	return 0;
}

