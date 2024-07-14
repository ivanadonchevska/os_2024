#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

file_1="${1}"
file_2="${2}"
dir="${3}"

# proverki za argumentite

valid_files=$(mktemp)

while read file; do
	error=$( cat "${file}" | egrep -nv "^{.*};$|^#")
	if [ ! -z "${error}" ]; then
		echo "Error in "${file}""
	else
		echo "${file}" >> "${valid_files}"
		continue
	fi
	echo "Line "${error}""

done < <( find "${dir}" -type f | egrep "\.cfg")

while read file; do
	cat "${file}" >> "${file_2}"

	username=$( basename "${file}" | cut -d'.' -f1)
	
	if [ $( cat "${file_1}" | grep "${username}" | wc -l) -eq 0 ]; then
		generated_pw=$( pwgen 16,1 )
		echo ""${username}" "${generated_pw}"" >> "${file_1}"
		echo ""${username}" "${generated_pw}""
	fi

done < "${valid_files}"

rm "${valid_files}"
