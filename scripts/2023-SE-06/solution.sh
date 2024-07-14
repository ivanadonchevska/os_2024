#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

camera="${1}"
library="${2}"

# kak kako putishta? ask2

if [ ! -d "${camera}" ]; then
	echo "Invalid directory"
	exit 1
fi

if [ -d "${library}" ]; then 
	echo "Library directory should not exist"
	exit 1
fi

#mkdir "${library}"

files_and_modification=$(mktemp)

find "${camera}" -type f | egrep '.*\.jpg' | xargs -I{} stat -c "%n %y" {} | cut -d'.' -f1,2  > "${files_and_modification}"
cat "${files_and_modification}"

start_date=$( cat "${files_and_modification}" | awk '{ print $2 }' | sort | head -n 1 )
previous_date="${start_date}"
end_date="${start_date}"

lines_cnt=$( cat "${files_and_modification}" | wc -l)
cnt=1
while read date; do
	if [ "${cnt}" -eq "${lines_cnt}" ]; then
		# check if 04-05 or 05-05 and act based on this
		# make two more scenarios
	fi
	
	echo "${date}"
	#date -d ""${previous_date}" + 1 day" +"%Y-%m-%d"
	if [ "${previous_date}" == "${date}" ]; then
		# just more photos from the same date, do nothing
		continue

	fi
	if [ $( date -d ""${previous_date}" + 1 day" +"%Y-%m-%d") == "${date}" ]; then
		previous_date="${date}"
		end_date="${date}"
	else
		echo ""${start_date}" "${end_date}""
		#mkdir
		start_date="${date}"
		previous_date="${date}"
		end_date="${date}"
	fi

done < <( cat "${files_and_modification}" | awk '{print $2}' | sort | tail -n +2)
