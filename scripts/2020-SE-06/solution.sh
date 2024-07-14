#!/bin/bash

# cat foo.conf | cut -d'#' -f1 | egrep ".+" | tr -d ' '
# tr -d ' ' -> delete intervals
# to remove all unnecessary data from file

if [ ${#} -ne 3 ]; then
	echo "Wrrong arrgument count"
	exit 1
fi

file="${1}"
key="${2}"
value="${3}"

if [ ! -f "${file}" ]; then
	echo "First argument must be a file"
	exit 1
fi

WORD="[[:alnum:]_]+"

if ! echo "${key}" | egrep -q "${WORD}"; then
	echo "Second argument must be a valid word!"
	exit 1
fi

if ! echo "${value}" | egrep -q "${WORD}"; then 
	echo "Third argument must be a valid word!"
	exit 1
fi
#write permissions!
#LINE="^ *${WORD} *= *${WORD} *$"

#if cat "${file}" | cut -d'#' -f1 | tr -d ' ' | egrep ".+"

#if cat "${file}" | cut -d'#' -f1 | egrep ".+" | egrep -q -v "${LINE}" ; then
#	echo "Conf file is not in expected format"
#	exit 1
#fi

user=$(whoami)
# if key is not present in file
if ! cat "${file}" | cut -d'#' -f1 | egrep '.+' | tr -s ' ' | cut -d' ' -f1 | grep -q "${key}"; then
	echo ""${key}" = "${value}" # added at $(date) by "${user}"" >> "${file}"
else
	while read line; do
		current_key=$(echo "${line}" | cut -d' ' -f1)
		current_value=$(echo "${line}" | cut -d' ' -f3)
		if [ "${current_key}" = "${key}" ]; then
			#echo "present"
			if [ "${current_value}" != "${value}" ]; then
				sed -i "s/^${key}.*/#&/" "${file}"
				# how to modify only this line?? ask
			fi
			continue
		fi
	done < <( cat "${file}" | cut -d'#' -f1 | egrep ".+" | tr -s ' ' )
fi
