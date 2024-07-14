#!/bin/bash

if [[ $# -ne 1 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

num="${1}"

if [[ ! "${num}" =~ ^[0-9]+$ ]]; then
	echo "Invalid number given as argument"
	exit 1
fi

if [[ $UID -ne 0 ]]; then
	echo "Script should be executed by root"
	exit 1
fi

users=$(mktemp)

ps -e -o user= | sort | uniq > "${users}"

#cat "${users}"


while read user; do
	total_user_rss=0
	while read pid rss; do
		total_user_rss=$(( $total_user_rss + $rss ))
		last_pid="${pid}"
	done < <(ps -u "${user}" -o pid=,rss= | sort -n -k2 )
	# sort -n -k2 -> so the pid with max rss will be last
	echo "Total user rss "${total_user_rss}" "

	if [[ "${total_user_rss}" -gt "${num}" ]]; then
		kill -s TERM "${last_pid}"
		sleep 1
		kill -s KILL "${last_pid}"
	fi

done < <(cat "${users}")

rm -- "${users}"
