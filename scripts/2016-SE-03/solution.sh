#!/bin/bash

#if [[ $UID -ne 0 ]]; then
#	echo "Script must be executed by root"
#	exit 1
#fi

while read user home; do
	perm=$(stat -c "%A" "${home}" 2>/dev/null | cut -c 3)
	if [[ "${perm}" != 'w' ]]; then
		echo "${user}"
	fi
	#echo "${perm}"
done < <( cat /etc/passwd | cut -d':' -f1,6 | tr ':' ' ')

# ask

# yes it's enought to test only with -d for nesushtestvuvashta home directory

#while read user home; do
#	if [[ -z "${home}" ]] || $(stat -c "%A" "${home}"| cut -c 3) == '-' ]]; then
#		echo "${user}"
#	fi
#done < <(cat /etc/passwd | cut -d':' -f1,6 | tr ':' ' ' )
