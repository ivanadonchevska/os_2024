#!/bin/bash

if [ $# -ne 2 ]; then
	echo "The script takes 2 arguments"
	exit 1
fi

if [ ! -d "$1" ]; then
	echo "First parameter must be a valid directory"
	exit 2
fi

DIR="$1"

ARCH="$2"

CONSTANT_STRING="vmlinuz"
NUMBER="[0-9]+"
CORRECT_FILE_FORMAT="^${CONSTANT_STRING}\-(${NUMBER})\.(${NUMBER})\.(${NUMBER})\-${ARCH}$"

files=$(find "$DIR" -maxdepth 1 -type f -exec basename '{}' \; | grep -E "$CORRECT_FILE_FORMAT")

versions=$(echo "$files" | cut -d '-' -f 2)
bestVersion=$(echo "$versions" | sort -n -r -t '.' -k 1,1 -k 2,2 -k 3,3 | head -n 1)

echo "$files" | grep "$bestVersion"
