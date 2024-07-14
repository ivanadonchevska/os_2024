#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Wrong argument count" >&2
	exit 1
fi

if ! cat /etc/passwd | cut -d':' -f1 | grep -q "^$1$"; then
	echo "User $1 not found" >&2
	exit 2
fi

if ! [ "$(whoami)" = "root" ]; then
    exit
fi

FOO=$1
FOOCOUNT=$(ps -u $FOO -o comm= | wc -l)

for USERNAME in $(cat /etc/passwd | cut -d: -f1); do
    COUNT=$(ps -u $USERNAME -o comm= | wc -l)

    if [ $COUNT -gt $FOOCOUNT ]; then
        echo $USERNAME
    fi
done

#SUM=0
#COUNT=0

while read HOURS MINUTES SECONDS; do

    TSECONDS=$(echo "3600 * $HOURS + 60 * $MINUTES + $SECONDS" | bc)
    COUNT=$(($COUNT + 1))
    SUM=$(($SUM + $TSECONDS))

done < <(ps -e -o time= | tr ':' ' ')

AVG_TIME=$( date -u --date=@$(echo "scale=2; $SUM / $COUNT" | bc) +%H:%M:%S)

echo Average time: ${AVG_TIME}

AVG_TIME=$( echo "$AVG_TIME*2" | bc)

while read PID SECONDS; do
	if [ $SECONDS -gt $AVG_TIME ]; then
		kill -9 $PID
	fi
done < <(ps -u %FOO -o pid=,times=)


