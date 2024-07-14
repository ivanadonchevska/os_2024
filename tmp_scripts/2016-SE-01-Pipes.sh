#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Insert only the name of the file"
	exit 1
fi

if [ ! -f "$1" ]; then
	echo "$1 is not a file or does not exist"
	exit 2
fi

lines=$(cat $1 | grep -E '[02468]' | grep -Ev '[a-w]' | wc -l)

echo "Number of lines which fulfill the requirements are: $lines"

