#!/bin/bash
set -u

if [ $# -ne 1 ]; then
	echo "This script takes only one parameter as an argument"
	exit 1
fi

if [ ! -d "$1" ]; then
	echo "The parameter must be a valid directory"
	exit 2
fi

DIR="$1"

fileFormat="^([0-9]{4}-[0-1][0-9]-[0-3][0-9])-([0-2][0-9])-([0-5][0-9])-([0-5][0-9]).txt$"

files=$(find "$DIR" -mindepth 4 -maxdepth 4 -type f)
fileNames=$(echo "$files" | xargs -I {} basename {} | grep -E "$fileFormat")

validFiles=$(mktemp) 

while read file; do 
	validDate=$(echo "$file" | sed -E "s/$fileFormat/\1 \2:\3:\4/g")
	if date -d "$validDate" &>/dev/null; then
		echo "$file" >> "$validFiles"
	fi
done <<< "$fileNames"

logs=$(echo "$files" | grep -f "$validFiles" | sed -E "s|^${DIR}/?[^/]+/[^/]+/([^/]+/.*.txt$)|\1|g")

fullFiles=$(echo "$files" | grep -f "$validFiles")

# kpacu

rm file
echo "$fullFiles" | cut -d '/' -f 5 | sort | uniq | while read user; do
	echo "$user $(echo "$fullFiles" | grep -E "^([^/]+/){4}${user}" | xargs cat | wc -l)" >> file
done

cat file | sort -t ' '  -k 2 -nr | head -n 10
 
# end
rm $validFiles
rm file
exit 0
