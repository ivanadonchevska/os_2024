#!/bin/bash

if [[ $# -ne 1 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

file="${1}"

if [[ ! -f "${file}" ]]; then
	echo "Expected file as argument"
	exit 1
fi

output=$(mktemp)

i=1
while read line; do
	echo ""${i}". "${line}"" >> "${output}"
	i=$(( ${i} + 1 ))
done < <(cat "${file}")

cat "${output}" | cut -d' ' -f1,5- | sort -k2 -t' '

rm "${output}"
