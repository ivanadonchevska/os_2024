#!/bin/bash


if [ $# -ne 1 ]; then
	# not on standard output but on stderr
	echo "Wrong arguments count" >&2
	exit 1
fi

if ! cat /etc/passwd | cut -d':' -f1 | grep -q "^$1$"; then
	echo "User $1 not found" >&2
	exit 1
fi

if ! [ "$(whoami)" = "root" ]; then
	exit 
fi


FOO=$1
FOOCOUNT=$(ps -u $FOO -o comm= | wc -l)

# time -> na ps pokazva vremeto koeto sa rabotili tezi procesi
#for USERNAME in $(cat /etc/passwd

# check it
date -u --date="@2" +%H:%M:%S


echo "Average time: $( date -u --date=@$(echo scale=2; SUM /
