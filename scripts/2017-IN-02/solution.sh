#!/bin/bash

if [[ $# -ne 1 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

#if [[ $UID -ne 0 ]]; then
#	echo "Script must be executed by root"
#	exit 1
#fi

FOO="${1}"
processes=$(mktemp)

ps -e -o user=,pid=,time= |sort -t' ' -k1 > "${processes}"
foo_ps_count=$(cat "${processes}" | awk '{print $1}' | grep "${FOO}" | wc -l)

#a
while read count user; do
	current_user_ps_count=$( echo "${count}" )
	if [[ "${current_user_ps_count}" -gt "${foo_ps_count}" ]]; then
		echo "${user}"
	fi
done < <(cat "${processes}" | awk '{print $1}' | uniq -c)

#b
#check it out!!
#ask
#seconds=$( cat "${processes}" | awk '{print $3}' | awk -F':' '{ print $1 "*3600 " $2 "*60 " $3 }'| xargs |tr ' ' '+' | bc  -l)
#count=$(cat "${processes}" | wc -l)
#echo "${count}"
#result=$(( ${seconds} / ${count} ))
#echo "${result}"

avg_time=$( cat "${processes}" | awk ' BEGIN {total = 0} { total += $3 } END {print total }')
#echo "${avg_time}"
#c



