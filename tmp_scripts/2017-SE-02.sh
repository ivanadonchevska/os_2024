#!/bin/bash

if [ $# -ne 3 ]; then
	echo "The number of parameters must be exactly 3"
	exit 1
fi

if [ ! -d "$1" ]; then
	echo "First parameter must be a valid directory"
	exit 2
fi

if [ ! -d "$2" ]; then
	echo "Second parameter must be a valid directory"
	exit 3

	
fi

ifEmpty=$(find "$2" -mindepth 1) 
if [[ "$ifEmpty" != "" ]]; then
	echo "Destination directory must be empty in order to procceed"
	exit 4
fi

if [ "$(id -u "$(whoami)")" -ne 1257 ]; then
	echo "Script must be ran by root"
	exit 5
fi

src="$1"
dst="$2"
name="$3"

find "$src" -type f -name "*$name*" | while read file; do
	subdir=$(dirname "${file#$src}")
	mkdir -p "$dst$subdir"
	
	mv "$file" "$dst$subdir"
done

exit 0
