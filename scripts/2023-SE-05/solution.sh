#!/bin/bash

total_iterations=0
seen_commands=$(mktemp)
break=1

while true; do
	sleep 1

	while read comm; do
		sum_rss=$( ps -e -o rss=,comm= | egrep "${comm}" | cut -d' ' -f1 | xargs | tr ' ' '+' | bc)

		if [ "${sum_rss}" -gt 65536 ]; then
			echo "${comm}" >> "${seen_commands}"
		fi
	done < <( ps -e -o comm= | sort | uniq)
	
	total_iterations=$(( $total_iterations + 1 ))
	if [ "${break}" -eq 1 ]; then
		break
	fi

	break=1
done

cat "${seen_commands}" | sort | uniq -c | awk -v count="${total_iterations}" '$1 < (count/2)'

# dokato spryat da sushtestvuvat komandi, chiyato zaeta pamet e 65536? -> kakvo tochno se ima predvid

# ask2
# grep -F -> fixed strings
# fixed string ne go vuspriema kato regularen izraz a kako string
