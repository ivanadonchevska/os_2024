#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

dir1="${1}"
dir2=$( realpath "${2}" )

#proverki

while read filename; do
	#file=$( echo "${filename}" | cut -d'/' -f2-)
	cp -rp "${filename}" "${dir2}"
done < <( find "${dir1}" -type f | egrep -v "^\.[.*]+\.swp$" )


# bar.swp also should be in dir2
# ask
# is it all??


#it's not working for /b/c ...
