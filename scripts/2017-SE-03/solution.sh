#!/bin/bash


#if [ $UID -ne 0 ]; then
#	echo "Script must be executed by root"
#	exit 1
#fi

# ask
# is there a case where ps_count can be 0, like to have user without processes?
# but ps gives only those with active processes??

while read user; do
	total_user_rss=0
	max_pid=-1
	ps_count=0
	max_rss=-1

	while read pid rss; do
		ps_count=$(( $ps_count + 1 ))
		total_user_rss=$(( $total_user_rss + $rss ))
		max_pid="${pid}"
		max_rss="${rss}"
	done < <( ps -u "${user}" -o pid=,rss= | sort -n -k2)
	echo "User "${user}" has "${ps_count}" processes with total rss "${total_user_rss}""
	
	avg_rss=$(( $total_user_rss / $ps_count ))
	if [[ "${max_rss}" -gt $(( ${avg_rss} * 2 )) ]]; then
		echo "Terminating process"
	#	kill -s TERM "${max_pid}"
	#	sleep 3
	#	kill -s KILL "${max_pid}"
	fi
done < <(ps -e -o user= | sort | uniq)
