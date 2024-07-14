#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

dir="${1}"

if [ ! -d "${dir}" ]; then
	echo "Expected directory as argument"
	exit 1
fi

#last_execution=$( stat -c "%X" "${0}" )
#echo "${last_execution}"

all_archives=$( find "${dir}" -mindepth 1 -maxdepth 1 -type f | egrep "^[^_]+_report-[0-9]+\.tgz$" | xargs -I{} stat -c "%n %Y" {} | xargs -I{} basename {})
echo "${all_archives}"

last_execution=$( stat -c "%X" "${0}")
echo "${last_execution}"


archives_to_be_proceed=$(mktemp)

while read line; do
	last_modification=$( echo "${line}" | cut -d' ' -f2)
	if [ "${last_modification}" -gt "${last_execution}" ]; then
		name=$( echo "${line}" | cut -d' ' -f1)
		echo "${name}" >> "${archives_to_be_proceed}"
	fi
done < <(echo "${all_archives}" )

cat "${archives_to_be_proceed}"

while read archive; do
	
	while read line; do
		
	done < <( tar -xf "${archive}")
done < <( cat "${archives_to_be_proceed}")
# not done
# do it later!
