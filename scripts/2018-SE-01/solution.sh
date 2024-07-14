#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

dir="${1}"

if [ ! -d "${dir}" ]; then
	echo "Invalid directory"
	exit 1
fi

friends=$(mktemp)

#find "${dir}" -mindepth 3 -maxdepth 3 -type d | xargs -I{} basename {} | sort | uniq > "${friends}" 

while read friend; do 
	
	total_friend_cnt=0
	echo "friend: "${friend}""
	while read file; do
		echo "file "${file}""
		lines=$( cat "${file}" | wc -l)
		total_friend_cnt=$(( $total_friend_cnt + $lines ))
		#check this out and make it works with awk $3 == "${friend}"
	done < <( find "${dir}" -mindepth 4 -maxdepth 4 | egrep "^.*/${friend}/[0-9]{4}-[0-0]{2}-[0-9]{2}-[0-9]{2}-[0-9]{2}-[0-9]{2}\.txt")
	
	echo ""${friend}" "${total_friend_cnt}"" >> "${friends}"
done < <( find "${dir}" -mindepth 1 -maxdepth 3 -type d | xargs -I{} basename {} | sort | uniq)

while read friend lines; do
	echo ""${friend}" "${lines}""
done < <(cat "${friends}" | sort -rn -k2 | head )

rm "${friends}"

