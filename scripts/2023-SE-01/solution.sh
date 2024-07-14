#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

file="${1}"
dir="${2}"

#proverki za dir i file

while read filename; do
	
	while read word; do
		censored_word=$( echo "${word}" | sed 's/./\*/g' )
		sed -i "s/\b${word}\b/${censored_word}/gi" "${filename}"
	done < <( cat "${file}")
done < <( find "${dir}" -type f | egrep '.*\.txt' )

# \< \> -> the same as \b \b 
