#!/bin/bash

if [ $# -ne 2 ]; then
	echo "There must be exactly two arguments"
	exit 1
fi

if [ ! -f "$1" ]; then
	echo "First argument is not a valid file"
	exit 2
fi

if [ ! -f "$2" ]; then
	echo "Second argument is not a valid file"
	exit 3
fi

artist1=$(basename "$1" | cut -d '.' -f 1)
artist2=$(basename "$2" | cut -d '.' -f 1)

linesFirstFile=$(cat "$1" | grep "$artist1" | wc -l)
linesSecondFile=$(cat "$2" | grep "$artist2" | wc -l)

winnerFile=""
if [ "$linesFirstFile" -gt "$linesSecondFile" ]; then
	winnerFile="$1"
else
	winnerFile="$2"
fi

cat "$winnerFile" | cut -d '-' -f 2- | sort > "$winnerFile.songs"

