#include <err.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char* argv[]) {
	
	if(argc != 3) {
		errx(1, "Error: invalid number of arguments");
	}

	int scl = open(argv[1], O_RDONLY);
	int sdl = open(argv[2], O_RDONLY);

	int output = open("high,sdl", O_RDWR | O_CREAT | O_TRUNC, 0666);

	uint16_t sdl_buff;
	uint8_t level
	int rs = 0;

	while((rs = read(scl, &level, sizeof(level))) == sizeof(level)) {
		read(sdl, &sdl_buff, sizeof(sdl_buff));

		if(level == 1) {
			write(output, &sdl_buff, sizeof(sdl_buff));
		}
	}

	close(scl);
	close(sdl);
	close(output);

	return 0;

	// proverki sekade
}
