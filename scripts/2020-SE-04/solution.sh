#!/bin/bash

if [ $# -ne 2 ]; then 
	echo "Invalid number of arguments"
	exit 2
fi

src="${1}"
dst="${2}"

if [ ! -d "${src}" ]; then
	echo "Expected directory as first argument"
	exit 1
fi

if [ -d "${dst}" ]; then
	echo "Second directory should not exist"
	exit 1
fi

mkdir -p "${dst}"
mkdir -p "${dst}/images"

while read file; do
	sha_sum=$( sha256sum "${file}" | cut -c 1-16)
	touch "${dst}/images/${sha_sum}.jpg"
done < <( find"${dir}" -type f | egrep "\.jpg$" )

# not done
# second part of the solution not clear at all
# ask
