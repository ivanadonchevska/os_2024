#!/bin/bash

if [ $# -ne 1 ]; then
	echo "The number of arguments must be exactly 1"
	exit 1
fi

if [ ! -d "$1" ]; then
	echo "Parameter is not a directory"
	exit 2
fi

# Both oprions are correct

#Here find finds all symlinks and with the help of test it tests if the link points to a correct file
#find "$1" -type l -exec sh -c 'test ! -e {} && echo "{}"' \;

# Here "-xtype l" searches for symlinks that are broken
find "$1" -xtype l
