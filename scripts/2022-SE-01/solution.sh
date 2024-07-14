#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

file="${1}"

if [ ! -f "${file}" ]; then
	echo "Expected file as first argument"
	exit 1
fi

while read device status; do
	cat "example_wakeup" | awk '{ print $1}' | tail -n +2 |  grep -q "${device}"
	if [[ ${?} -ne 0 ]]; then
		echo "Warning, device "${device}" does not exist"
		exit 1
	fi
	
	if [ "${status}" != "*enabled" ] && [ "${status}" != "*disabled" ]; then
		echo "Invalid device status"
		exit 1
	fi

	current_status=$( cat "example_wakeup" | tail -n +2 | awk '{ print $1 " " $3}' | grep "${device}" | awk '{ print $3 }' )
	
	if [ "${current_status}" == "${status}" ]; then
		echo "No need to wakeup device, status is already the same"
	else
		echo "${device}" > "example_wakeup}"
	fi
		
done < <( cat "${file}" | cut -d'#' -f1 | egrep '.+' | awk '{$1=$1}1' )
