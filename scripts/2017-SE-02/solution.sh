#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

src="${1}"
dst="${2}"
str="${3}"

if [ ! -d "${src}" ]; then
	echo "Invalid directory"
	exit 1
fi

if [ $( find "${dst}" -type f | wc -l) -ne 0 ]; then
	echo "Second directory should not contain files"
	exit 1
fi

if [[ ! "${str}" =~ ^[a-zA-Z]+$ ]]; then
	echo "Third argument is not a string"
	exit 1
fi

#if [ $UID -ne 0 ]; then
#	echo "Script must be executed by root"
#	exit 1
#fi

# important ask
# the same problem as this with cp


while read file; do
	real_name=$( echo "${file}" | cut -d'/' -f2-)
	mkdir -p "${dst}/$(dirname "${real_name}")"
	mv "${file}" "${dst}/${real_name}"
done < <( find "${src}" -type f -name "*${str}*")

#find "${src}" -type f -name "*${str}*" | cut -d'/' -f2- | xargs -I{} mv {} "${dst}"
