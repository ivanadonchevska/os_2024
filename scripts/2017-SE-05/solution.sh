#!/bin/bash


if [ $# -ne 2 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

dir="${1}"
str="${2}"

if [ ! -d "${dir}" ]; then
	echo "First argument is expected to be directory"
	exit 1
fi

if [[ ! "${str}" =~ ^[a-zA-Z0-9]+$ ]]; then
	echo "Second argument is expected to be string"
	exit 1
fi

files=$(mktemp)
find "${dir}" -type f | egrep "^${dir}vmlinuz-[0-9]+\.[0-9]+\.[0-9]+-[a-zA-Z0-9]+$" | egrep ".*-$str$" > "${files}"

max_global_version=$( cat "${files}" | cut -d'-' -f2 | sort -n -t'.' -k1,1  -k2,2 -k3,3 | tail -n 1)

cat "${files}" | grep "${max_global_version}"

rm "${files}"
