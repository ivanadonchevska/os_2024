#include <err.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

	//koeficient na zapulvane = fill_factor

	// algorithm
	//shouldopen file, check for its size and format
	// create structures for header, preamble and objects
	// check it's parameters
	// read the file properly, do the calculations and write the result to stdout

// this to continue for each object
	while(read(fd, &obj, sizeof(obj)) > 0) {
		fill_factor = obj.ssize / obj.size; // xi in formula
		weighted_fill_factor = fill_factor * size; // xi * wi in fromula
		total_weighted_fill_factor += weighted_fill_factor;
		total_size += size;
	}

	if(total_size == 0) {
		err(3, "Avoid division by zero");
	}

	weighted_average = total_weighted_fill_factor / total_size;
}
