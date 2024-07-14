#!/bin/bash
set -u

if [ $# -ne 2 ]; then
	echo "Arguments must be exactly two"
	exit 1
fi

if [ ! -d "$1" ]; then
	echo "First parameter must be a valid directory"
	exit 2
fi

if [ -d "$2" ]; then
	echo "Second parameter must not exist as directory"
	exit 3
fi

SRC="$1"
DEST="$2"

mkdir "$DEST"

START=""
DATE=""
NEW_DIR=""

while read FILE NEW_NAME; do
	DATE=$(echo "$NEW_NAME" | cut -d '_' -f 1)

	DEST_EMPTY=$(find "$DEST" -type f)

	if [ "$DEST_EMPTY" != "" ]; then
		PREV_FILE_DATE=$(find "$NEW_DIR" -type f | cut -d '_' -f 1 | sort -n | tail -n 1 | xargs -I {} basename {})
		
		NEXT_DAY=$(date -d "$PREV_FILE_DATE + 1 day" +%Y-%m-%d)
		
		if [ "$DATE" = "$PREV_FILE_DATE" ] || [ "$DATE" = "$NEXT_DAY" ]; then
			mv "$FILE" "${NEW_DIR}/${NEW_NAME}"
		else
			mv "${NEW_DIR}" "${NEW_DIR}_${PREV_FILE_DATE}"
			START=""
		fi
	fi

	if [ "$START" = "" ]; then
		START="$DATE"
		NEW_DIR="${DEST}/${START}"
		mkdir -p "$NEW_DIR" 

		mv "$FILE" "${NEW_DIR}/${NEW_NAME}"

	fi
done < <(find "$SRC" -type f -name "*.jpg" -printf "%p %TF_%TH:%TM:%TS.jpg\n" | sed -E "s/(^[^.]*.jpg [^.]*).[0-9]*.jpg$/\1.jpg/g" | sort -n -t ' ' -k 2,2)

LAST_DIR=$(find "${DEST}" -type d | sort -n | tail -n 1)
mv "$LAST_DIR" "${LAST_DIR}_${DATE}"

exit 0
