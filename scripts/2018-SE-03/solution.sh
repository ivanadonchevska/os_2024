#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

input_file="${1}"
output_file="${2}"

if [ ! -f "${input_file}" ]; then
	echo "Expected file as first argument"
	exit 1
fi

touch "${output_file}"

while read line; do
	id=$( echo "${line}" | cut -d',' -f1)
	rest=$( echo "${line}" | cut -d',' -f2-)

	if [ $( cat "${output_file}" | fgrep ","${rest}"" | wc -l ) -gt 0 ]; then
		# it's already in file
		continue
	fi
	echo "${line}" >> "${output_file}"

done < <( cat "${input_file}" | sort -n -t',' -k1)
