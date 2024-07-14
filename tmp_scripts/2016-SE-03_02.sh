#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Parameters must be exactly two"
	exit 1
fi

if ! [[ "$1" =~ ^[0-9]+$ ]]; then
    echo "The first parameter must be a number"
    exit 2
fi

if ! [[ "$2" =~ ^[0-9]+$ ]]; then
    echo "The second parameter must be a number"
    exit 3
fi

#Here find excludes the script itself from the result of found files

files=$(find . -maxdepth 1 -type f ! -name "$(basename $0)" -printf "%p\n")

mkdir {a,b,c}

while read fileName; do
	lines=$(wc -l < "$fileName")
	if [ "$lines" -lt $1 ]; then
		mv "$fileName" a/
	elif [ "$lines" -le $2 ]; then
		mv "$fileName" b/
	else
		mv "$fileName" c/
	fi
done <<< "$files"

exit 0
