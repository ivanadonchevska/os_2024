#!/bin/bash

args="${@}"
N=10

if [ $( echo "${args}" | grep '\-n' | wc -l) -gt 0 ]; then
	N=$(echo "${args}" | cut -d' ' -f2)
fi

#can make the args depending on this if -n is provided or not!

output=$(mktemp)

while read file; do
	IDF=$( echo "${file}" | cut -d'.' -f1)

	while read line; do
		timestamp=$( echo "${line}" | cut -d' ' -f1,2)
		interval=' '
		data=$( echo "${line}" | cut -d' ' -f3-)

		echo ""${timestamp}" "${interval}" "${IDF}" "${interval}" "${data}"" >> "${output}"
	done < <( cat "${file}" | sort -rn | head -n "${N}")
	
done < <( echo "${args}" | tr ' ' '\n')

cat "${temp}" | sort -n -k1,1 -k2,2

rm "${output}"

