#!/bin/bash

if [[ $# -ne 1 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

dir="${1}"

if [[ ! -d "${dir}" ]] || [[ ! -r "${dir}" ]]; then
	echo "Not a directory or not readable"
	exit 1
fi

find -L "${dir}" -type l 2>/dev/null 
