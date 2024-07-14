#!/bin/bash

if [ $# -ne 1 ]; then
	echo "There should be only one parameter"
	exit 1
fi

if [ "$(id -u)" -ne 0 ]; then
	echo "Only root can run this script"
	exit 2
fi

LIMIT=$1
declare -A user_rss

while read user rss; do
	user_rss[$user]=$((${user_rss[$user]} + rss))
done < <(ps -e -o user=,rss=)

for user in "${!user_rss[@]}"; do
	total_rss=${user_rss[$user]}
	echo "$user uses $total_rss kB of memory."

	if [ "$total_rss" -gt "$LIMIT" ]; then
		echo "Memory limit exceeted for user $user"
		top_pid=$(ps -e -o user=,pid=,rss= --sort=-rss | grep -E "^$user" | head -n 1 | awk ' { print $2 } ')

		echo "Killing process $top_pid for user $user"

		#kill -9 $top_pid
	fi
done
