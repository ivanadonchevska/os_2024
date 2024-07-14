#include <err.h>

int main(int argc, char* argv[]) {
	// create struct for data representation
	// open file, check it's size using stat

	// read buff from file, where buff is the struc
	// and for each read data, calculate total_time += buff.end - buff.start
	// create users and times arrays and on each iteration in users[index] = buff.uid and times[index] = buff.end - buff.start
	// increase index on each loop

	// when have all the data in arrays
	
	// calculate average value
	// lseek(fd, 0, SEEK_SET), so can read the data again and calculate the total dispersion
	//lseek again and iterate through each record
	// calculate time spent for each record and check if it's greater than total dispersion
	// if greater write to stdout uid of the user with greater time spent thhan dispersion and the time of it's longest time session

	//TODO
}
