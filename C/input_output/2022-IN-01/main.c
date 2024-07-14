#include <err.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stddef.h>
#include <unistd.h>

struct header {
	uint16_t magic;
	uint16_t filetype;
	uint32_t count;
}__attribute__((packed));

//proverki na sekadee!!!!
int main(int argc, char* argv[]) {
	
	if(argc != 4) {
		errx(1, "Error: invalid number of arguments");
	}

	int fd_list = open(argv[1], O_RDONLY);
	int fd_data = open(argv[2], O_RDONLY);
	int fd_out = open(argv[3], O_RDWR | O_CREAT | O_TRUNC, 0666);

	struct header h_list;
	struct header h_data;
	struct header h_out;

	read(fd_list, &h_list, sizeof(h_list));
	read(fd_data, &h_data, sizeof(h_data));

	const uint16_t magic_check = 0x5A4D;
	if(h_list.magic != magic_check || h_data.magic != magic_check) {
		err(2, "Error: magic constraint failed");
	}

	if(h_list.filetype != 1 || h_data.filetype != 2) {
		err(2, "Error: filetype constraint failed");
	}

	struct stat st_list;
	struct stat st_data;
	
	fstat(fd_list, &st_list);
	fstat(fd_data, &st_data);

	if((st_list.st_size - sizeof(h_list)) / sizeof(uint16_t) != h_list.count || (st_data.st_size - sizeof(h_data)) / sizeof(uint32_t) != h_data.count) {
		err(2, "Error: files length does not match elements count");
	}

	if(h_list.count > h_data.count) {
		err(2, "Error: no position in data matching position in list");
	}

	h_out.magic = magic_check;
	h_out.filetype = 3;
	h_out.count = h_list.count;
	write(fd_out, &h_out, sizeof(h_out));

	size_t position = 0;
	uint16_t offset;
	uint32_t data;
	uint64_t out_buff;

	// not sure if the logic is working correct, but it should be somehow like this
	while(position < h_list.count) {
		lseek(fd_list, sizeof(h_list) + position * sizeof(uint16_t), SEEK_SET);
		read(fd_list, &offset, sizeof(offset));

		lseek(fd_data, sizeof(h_data) + offset * sizeof(uint32_t), SEEK_SET);
		read(fd_data, &data, sizeof(data));

		out_buff = data;
		lseek(fd_out,  sizeof(h_out) + offset * sizeof(uint64_t), SEEK_SET);
		write(fd_out, &out_buff, sizeof(out_buff));

		position++;
	}
	
	close(fd_list);
	close(fd_data);
	close(fd_out);

	return 0;
}
