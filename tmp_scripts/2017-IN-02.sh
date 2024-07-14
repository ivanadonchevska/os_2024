#!/bin/bash

if [ $# -ne 1 ]; then
	echo "There must be only one parameter"
	exit 1
fi

if [ "$(id -u)" -ne 0 ]; then
	echo "Script must be executing from root only"
#	exit 2
fi

if ! cat /etc/passwd | cut -d ':' -f 1 | grep -q "$1"; then
	echo "Enter a valid user"
	exit 3
fi

FOO=$1
FOOCOUNT=$(ps -u $FOO | tail -n +2 | wc -l)

for USERNAME in $(cat /etc/passwd | cut -d ':' -f 1); do
	COUNT=$(ps -u $USERNAME -o comm= | wc -l)

	if [ $COUNT -gt $FOOCOUNT ]; then
		echo $USERNAME
	fi
done

SUM=0
COUNT=0

while read HOURS MINUTES SECONDS; do
	TSECONDS=$(echo "3600 * $HOURS + 60 * $MINUTES + $SECONDS" | bc)
	COUNT=$(($COUNT + 1))
	SUM=$(($SUM + $TSECONDS))

done < <(ps -e -o time= | tr ':' ' ')

AVG_TIME=$(date -u --date=@$(echo "scale=2; $SUM / $COUNT" | bc) +%H:%M:$S)


echo Average time: ${AVG_TIME}

AVG_TIME=((${AVG_TIME} * 2))

while read PID SECONDS; do

	if [ $SECONDS -gt $AVG_TIME ]; then
		#kill -9 $PID
	fi
done < <(ps -u $FOO -o pid=,times=)
