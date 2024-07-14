#include <err.h>
#include <fcntl.h>

// arg1 = [5] = [f|1| | | | ] -> 4 + 1 za terminirashtata nula
// arg2 = [5] = [f|2| | | | ]

// argCount = 0 -. na novio red +1
// charCurrent = 0 -> zanuljava se

// while read 0:
//	...
//	cmd(arg1, arg2)
//	...
//	argCount++;
//	if(argCount % 2 )
//		cmd(arg1)


// argCount = 0
// charCurr = 0

// while read 0 in b:
//	if b is delimeter:
//		args[argCount % 2][chrCurr] = '\0';
//		if argCount  == 2:
//			cmd(args[0], args[1]);
//			argCount = 0;
//			

//	else:
//		args[argCount % 2][delimeter] = b;
//		charCurr++
//
//if argCount == 1:
//	cmd(arg1);
//	

