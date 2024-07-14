#!/bin/bash

args="${@}"

# in case when file is with more lines, make it on one, so the algorithm for the task will be right for both types of files
# cat "${file}" | cut -d';' -f1 | xargs | awk -F'(' '{print $1 $2}' | cut -d')' -f 1

serial_number=$( cat "${file}" | egrep -o '\.[a-z]\. [0-9]+ [0-9]+' | awk '{print $2}')

date_file=$( echo "${serial_number}" | cut -c 1,8)
tt=$( echo "${serial_number}" | cut -c 9,10)

current_date=$( date +'%Y%m%d')

if [ "${current_date}" == "${date_file}" ]; then
	new_tt=$(( $tt + 1 ))
	sed -i "s/("${date_file}")"${tt}"/\1"${new_tt}"/"
fi

# if date_file > current -> instead new_tt change it with 00

# this logic for all files
# put them in while loop, iterate through each one and finish the logic
