#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

dir="${1}"
num="${2}"

if [ ! -d "${dir}" ]; then
	echo "First argument should be directory"
	exit 1
fi

if [ "${num}" -lt 1 ] || [ "${num}" -gt 99 ];  then
	echo "Number is not in the right interval"
	exit 1
fi


objects=$(mktemp)

# here we have uniq objects for backup
find "${dir}" -mindepth 1 -type f -printf "%f\n" | cut -d'-' -f1,2 | sort -u >> "${objects}"

while read object; do
	result=$( find "${dir}" -type f -name ""${object}"*")
	
	yearBackup=$( echo "${result}" | awk -F'/' '$2 == 0 {print $0}')
	monthBackup=$( echo "${result}" | awk -F'/' '$2 == 1 {print $0}')
	weekBackup=$( echo "${result}" | awk -F'/' '$2 == 2 {print $0}')
	dayBackup=$( echo "${result}" | awk -F'/' '$2 == 3 {print $0}')

	yearCount=$( echo "${yearBackup}" | wc -l)
	monthCount=$( echo "${monthBackup}" | wc -l)
	weekCount=$( echo "${weekBackup}" | wc -l)
	dayCount=$( echo "${dayBackup}" | wc -l)

	#the logic for this task is to check if there is more than needed backups, by categories, and if true to delete the unnecessary backups and leave only the wanted number of them

	if [ "${yearCount}" -gt 1 ]; then
		backupsToDelete=$(( $yearCount - 1 ))
		# maybe first check if df is less than number, then delete them, else do nothing

		usage=$( df "${dir}" | egrep -o '[0-9]+\%' | cut -d'%' -f1)
		if [ "${usage}" -ge "${num}" ]; then

			find "${dir}0" -mindepth 1 -type f -name ""${object}"*" | sort | head -n "${backupsToDelete}" | xargs -I{} rm {}
		fi
	fi

	# repeat the same for month, week and daily count
done < <(cat "${objects}")

# and delete all broken symlinks
find -L "${dir}" -type l | xargs -I{} rm {}
