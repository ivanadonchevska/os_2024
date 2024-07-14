#!/bin/bash

if [[ $# -ne 3 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

file="${1}"
str_1="${2}"
str_2="${3}"

if [[ ! -f "${file}" ]]; then
	echo "Expected file as first argument"
	exit 1
fi

if [[ ! "${str_1}" =~ ^[a-zA-Z_]+$ ]] || [[ ! "${str_2}" =~ ^[a-zA-Z_]+$ ]]; then
	echo "Expected strings as second and third argument"
	exit 1
fi

terms_1=$(cat "${file}" | grep "^${str_1}=" | awk -F'=' '{print $2}' | tr ' ' '\n')
terms_2=$(cat "${file}" | grep "^${str_2}=" | awk -F'=' '{print $2}' | tr ' ' '\n')
result=""
while read term; do
	if [[ $(echo "${terms_1}" | grep "${term}" | wc -l) -eq 0 ]]; then
		result=$( echo "${result}" "${term}")
	fi
done < <(echo "${terms_2}" )

sed -i -E "s/(${str_2}=).*/\1${result}/g" "${file}"

# important
