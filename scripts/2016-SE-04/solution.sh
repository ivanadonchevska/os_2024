#!/bin/bash

if [[ $# -ne 2 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

file_1="${1}"
file_2="${2}"

if [[ ! -f "${file_1}" ]] || [[ ! -f "${file_2}" ]]; then
	echo "Expected files as arguments"
	exit 1
fi

file_1_cnt=$(cat "${file_1}" | grep -w "${file_1}" | wc -l)
file_2_cnt=$(cat "${file_2}" | grep -w "${file_2}" | wc -l)

if [[ "${file_1_cnt}" -gt "${file_2_cnt}" ]]; then
	winner_file="${file_1}"
else
	winner_file="${file_2}"
fi

cat "${winner_file}" | cut -d' ' -f4- | sort > ""${winner_file}".songs"
