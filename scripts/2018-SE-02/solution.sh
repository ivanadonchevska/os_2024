#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

file="${1}"
dir="${2}"

if [ ! -f "${file}" ]; then
	echo "Expected file as first argument"
	exit 1
fi

# check if dir is empty
if [ ! -d "${dir}" ]; then
	echo "Expected directory as second argument"
	exit 1
fi

# get uniq names, inicialize counter starting from 1, and increment it on each record from file
# write the wanted data in wanted files
# format the input file, to have needed data 
# not done
