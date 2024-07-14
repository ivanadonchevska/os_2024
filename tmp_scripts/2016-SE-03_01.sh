#!/bin/bash

#Checks if the one who starts the script is root iwth userid 0
if [ "$(id -u)" -ne 0 ]; then
	echo "Script must be run by root"
	exit 1
fi

#IFS is userd as a separator, -r is used to escape the use of "\" as special symbols and the "_" is used
#as fields that have to be ignored

while IFS=: read -r username _ _ _ _ homedir _; do
	if [ ! -d "$homedir" ] || [ ! -w "$homedir" ]; then
		echo "$username: $homedir"
	fi
done < /etc/passwd
