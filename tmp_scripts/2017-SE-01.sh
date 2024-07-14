#!/bin/bash

if [ $# -gt 2 ]; then
	echo "Script accepts two parameters at most. First should be mandatory and the second is optional"
	exit 1
fi

if [ ! -d "$1" ]; then
	echo "First argument is not a valid directory"
	exit 2
fi

dir="$1"
numberOfLinks="$2"

if [ $# -eq 1 ]; then
	find "$dir" -xtype l -print
fi

if [ $# -eq 2 ]; then
	if ! [[ $numberOfLinks =~ ^[0-9]+$ ]]; then
        echo "Second parameter must be an integer."
        exit 3
    fi

	find "$dir" -type f -link +$(($numberOfLinks - 1)) -print
fi

exit 0
