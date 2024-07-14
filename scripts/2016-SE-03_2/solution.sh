#!/bin/bash


if [[ $# -ne 2 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

num_1="${1}"
num_2="${2}"

if [[ ! "${num_1}" =~ ^[0-9]+$ ]] || [[ ! "${num_2}" =~ ^[0-9]+$ ]]; then
	echo "Invalid arguments. Numbers expected"
	exit 1
fi

mkdir "a"
mkdir "b"
mkdir "c"

while read file; do
	lines=$(cat "${file}" | wc -l)
	if [[ "${lines}" -lt "${num_1}" ]]; then
		mv "${file}" "a"
	elif [[ "${lines}" -gt "${num_1}" ]] && [[ "${lines}" -lt "${num_2}" ]]; then
		mv "${file}" "b"
	else 
		mv "${file}" "c"
	fi
done < <( find . -type f | grep '.txt')
