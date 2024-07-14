#!/bin/bash

last_file=0
last_user=0
last_modify=0

while read user home; do
	current_file=$( find "${home}" -type f -printf "%T@ %f\n" | sort -n -t' ' -k1 | tail -n 1)
	current_modify=$( echo "${current_file}" | awk '{print $1 }' )
	if [[ "${current_modify}"  -gt "${last_modify}" ]]; then
		last_modify="${current_modify}"
		last_user="${user}"
		last_file="${current_file}"
	fi
done < <(cat /etc/passwd | cut -d':' -f1,6 | tr ':' ' ')

echo "User "${last_user}" with last modified file "${last_file}""
