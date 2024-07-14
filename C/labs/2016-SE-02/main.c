#include <err.h>
#include <fcntl.h>


// [(x1,y1), (x2, y2), ... , (xn, yn)]
// elems = [z1, z2, z3, ..., zk]
// result = []

// for (x,y) in intervals
//	for j = 0 do y: // goleminata na intervala
//		result.append(elems[x + j]);
int main(int argc, char* argv[]) {
	// O_CREAT S_IRUSR | S_IRWR -> S_IRWXU
	// check them in man 2  open

	// stat -> podavame mu pato i buffer od tip struct stat
	// dava informacija za pato

	//fstat -> prima fajlov deskriptor vmesto put do pato
	// man 3type stat

	struct stat s;
	if(fstat(f1, &s) == -1) {
		err(
	}

	// 2017-in-01
	// 2017-se-02 ?
}
