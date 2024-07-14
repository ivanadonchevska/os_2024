#!/bin/bash

TOTAL_ITERATIONS=0
SEEN_COMMANDS=$(mktemp)
BREAK=1

while true; do
    sleep 1

	while read COMM; do
		SUM=$(ps -e -o rss=,comm= | grep -E "$COMM$" | cut -d' ' -f1 | xargs | tr ' ' '+' | bc)
		if [ "$SUM" -gt 65536 ]; then
			$COMM >> ${SEEN_COMMANDS}			
		fi
	done < <(ps -e -o comm= | sort | uniq)

    TOTAL_ITERATIONS=$(( $TOTAL_ITERATIONS + 1))
    if [ $BREAK -eq 1 ]; then
        break
    fi
    BREAK=1
done

cat $SEEN_COMMANDS | sort | uniq -c | awk -v count=$TOTAL_ITERATIONS '$1 > (count/2)'

