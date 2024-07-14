#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

file_1="${1}"
file_2="${2}"
black_hole="${3}"

# proverki

distance_1=$( cat "${file_1}" | grep "${black_hole}" | awk '{ print $3 }' )
distance_2=$( cat "${file_2}" | grep "${black_hole}" | awk '{ print $3 }' )

if [ "${distance_1}" -lt "${distance_2}" ]; then
	echo "${file_1}"
else
	echo "${file_2}"
fi


