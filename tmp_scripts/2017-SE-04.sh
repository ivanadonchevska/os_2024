#!/bin/bash

if [ $# -lt 1 ] || [ $# -gt 2 ]; then
	echo "There should be at most 2 parameters"
	exit 1
fi

if [ ! -d "$1" ]; then
	echo "First parameter should be a valid directory"
	exit 2
fi

dirname="$1"
outfile="$2"
brokenSyms=0

allSymlinks=$(find "$dirname" -type l) 

while read -r link; do
	if [ ! -e "$link" ]; then
		((brokenSyms++))
	else
		target=$(readlink "$link")
		
		if [ -z "$outfile" ]; then
			echo "$link -> $target"
		else
			echo "$link -> $target" >> "$outfile"
		fi
	fi
done <<< "$allSymlinks"

if [ -z "$outfile" ]; then
	echo "Broken symlinks: $brokenSyms"
else
	echo "Broken symlinks: $brokenSyms" >> "$outfile"
fi

exit 0
