#!/bin/bash

if [ $# -ne 1 ]; then
	echo "This script should have only one parameter"
	exit 1
fi

if [ ! -f "$1" ]; then
	echo "The parameter should be a valid file"
	exit 2
fi

newOrder=$(cat "$1" | awk -F '-' ' { $1 = "" } { print NR". @"$0 } ' | sort -t '@' -k 2 | sed 's/ @ //g')

echo "$newOrder"
