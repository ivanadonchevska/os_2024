#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Arguments must be exactly 3: <file> <key> <value>"
	exit 1
fi

if [ ! -f "$1" ]; then
	echo "First argument must be a file"
	exit 2
fi

WORD="[a-zA-Z0-9_]+"

if ! echo "$2" | grep -q -E "^${WORD}$"; then
	echo "Second argument must be a valid word"
	exit 3
fi


if ! echo "$3" | grep -q -E "^${WORD}$"; then
	echo "Third argument must be a valid word"
	exit 4
fi

LINE="^ *${WORD} *= *${WORD} *$"
FILE="$1"
KEY="$2"
VALUE="$3"

if cat "$FILE" | cut -d '#' -f 1 | grep -E "[^ ]+" | grep -E -q -v "$LINE"; then
	echo "Conf file is not expected format"
	exit 5
fi
 
USER=$(whoami)
DATE=$(date)

foundLine=$(cat "$FILE" | grep -E "^ *${KEY} *= *${WORD} *.*$")

if [ -z "$foundLine" ]; then
	echo "${KEY} = ${VALUE} # added at $DATE by $USER" >> "$FILE"
else
	sed -E -i "s/($foundLine)/# \1 # edited at $DATE by ${USER}\n${KEY} = ${VALUE} # added at $DATE by $USER/g" "$FILE"
fi

exit 0
