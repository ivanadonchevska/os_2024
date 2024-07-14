#!/bin/bash

if [ $# -lt 2 ]; then
	echo "Expected at least 2 arguments"
	exit 1
fi

seconds="${1}"
arguments="${@}"
command_with_args=$( echo "${arguments}" | cut -d' ' -f2-)

duration=0
start_time=$( date +%s.%N)

times_executed=0

while [[ $( echo "${duration}" | cut -d'.' -f1 ) -lt "${seconds}" ]]; do
	# execute the command
	${command_with_args}

	end_time=$( date +%s.%N)
	duration=$( echo ""${end_time}" - "${start_time}"" | bc )
	times_executed=$(( $times_executed + 1 ))
done

average_runtime=$( echo " scale=2; "${duration}" /  "${times_executed}"" | bc )

# important
# sed
echo "Ran the command "${command_with_args}" "${times_executed}" times for  $( echo "${duration}" | sed -E 's/(.*)\.(..).*$/\1.\2/' ) seconds"
echo "Average runtime: "${average_runtime}" seconds"


