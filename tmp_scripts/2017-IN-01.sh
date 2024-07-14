#!/bin/bash

if [ $# -ne 3 ]; then
	echo "The script accepts 3 parameters only"
	exit 1
fi

if [ ! -f "$1" ]; then
	echo "First parameter must be a valid file"
	exit 2
fi

file="$1"

key1="$2"
key2="$3"

firstLine=$(grep -E "^$key1=" < "$file")
secondLine=$(grep -E "^$key2=" < "$file")

if [[ "$secondLine" == "" ]]; then
	#echo "Line with second parameter could not be found."
	exit 0
fi

firstValue=$(echo "$firstLine" | cut -d '=' -f 2- | xargs)
secondValue=$(echo "$secondLine" | cut -d '=' -f 2- | xargs)

for term in $firstValue; do
	secondValue=$(echo "$secondValue" | sed "s/\b$term\b\s*//g")
done

newSecondLine="$key2=$secondValue"
sed -i "/^$key2=/c\\$newSecondLine" "$file"

exit 0
