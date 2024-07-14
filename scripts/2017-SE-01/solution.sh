#!/bin/bash

if [ $# -lt 1 ]; then
	echo "Expected at least 1 argument"
	exit 1
fi

dir="${1}"

if [ ! -d "${dir}" ]; then
	echo "First argument should be directory"
	exit 1
fi

num=0

if [ $# -eq 2 ]; then
	num="${2}"
fi

if [[ ! "${num}" =~ ^[0-9]+$ ]]; then
	echo "Second argument should be number"
	exit 1
fi

if [ $# -eq 2 ]; then
	while read file; do
		hardlinks=$( stat -c '%h' "${file}")
		num_of_hardlinks=$(( "${hardlinks}" -1 ))
		
		if [ "${num_of_hardlinks}" -gt "${num}" ]; then
			echo "${file}"
		fi
		
	done < <( find "${dir}" -type f 2>/dev/null)

else
	find -L "${dir}" -type l 2>/dev/null
	
fi

