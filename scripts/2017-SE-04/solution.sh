#!/bin/bash

if [ $# -lt 1 ]; then
	echo "Expected at least one argument"
	exit 1
fi

dir="${1}"

if [ ! -d "${dir}" ]; then
	echo "First argument should be a directory"
	exit 1
fi

file_name=""
if [ $# -eq 2 ]; then
	file_name="${2}"
fi

if [ ! -f "${file}" ]; then
	echo "Second argument should be a file"
	exit 1
fi

broken_cnt=0

# readlink -f link -> gives name destinattion
# file filename -> destination: broken symlink (if broken)
# but how to know if destination exist?
# ask
# destination existence ==? broken symlink  -> YES
while read link; do
	file="$(readlink -f "${link}")"
	if [[ $( file "${file}" | grep 'broken' | wc -l ) ]]; then
		broken_cnt=$(( $broken_cnt + 1 ))
	fi
	# -e option on test checks if this file exist, which means check if destination exist
	if [ -e "${file}" ]; then
		if [ $# -eq 2 ]; then
			echo ""${link}" -> "${file}"" >> "${file_name}"
		else
			echo ""${link}" -> "${file}"" 
		fi
	fi
done < <( find "${dir}" -type l 2>/dev/null)

echo "Broken symlinks: "${broken_cnt}"
