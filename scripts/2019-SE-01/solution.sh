#!/bin/bash

#a

valid_lines=$(mktemp)
max_num=0

while read line; do
	if [ $( echo "${line}" | egrep '^[+-]?[0-9]+$' | wc -l ) -gt 0 ]; then
		echo "${line}" >> "${valid_lines}"
	fi
done

cat "${valid_lines}"

rm "${valid_lines}"

#not done
