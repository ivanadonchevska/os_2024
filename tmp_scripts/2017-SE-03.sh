#!/bin/bash

if [ $# -ne 0 ]; then
	echo "Script takes 0 arguments"
	exit 1
fi

if [ "$(id -u)" -ne 0 ]; then
	echo "Script must be run by root"
#	exit 2
fi

users=$(ps -e -o user= | sort | uniq)

while read user; do
	processes=$(ps -u $user -o pid=,rss=)
	
	numberOfProcesses=$(echo "$processes" | wc -l)
	rssSum=$(echo "$processes"| awk ' {sum += $2} END {print sum}')
	echo "User ${user} has: \"${numberOfProcesses}\" processes and \"${rssSum}\" RRS memory combined for all of them"
	
	if [ $numberOfProcesses -gt 0 ]; then
		avgRss=$((rssSum / numberOfProcesses))
		echo "Avarage RSS for user $user is: $avgRss"

		while read pid rss; do
		doubleAvg=$((2 * avgRss))
			if [ $rss -gt $doubleAvg ]; then
				echo "Killing process $pid of user $user using $rss RSS, which is more than double the average $avgRss."
				# kill $pid
			fi
		done <<< "$(echo "$processes" | awk '{print $1,$2}')"
	fi

done <<< "$users"
