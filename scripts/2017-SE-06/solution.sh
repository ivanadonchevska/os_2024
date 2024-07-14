#!/bin/bash

# root script execution

#ask
# zadadena nesushtestvuvashta home dir ?? how to check this
# is it enough to check only -d ? -> YES

root_rss=$( ps -e -u root -o rss= | xargs | tr ' ' '+' | bc)


while read user home; do
	
	if [[ "${user}" != "root" && ( ! -d "${home}" || $(stat -c "%U" "${home}") != "${user}" || $(stat -c "%A" "${home}" | cut -c 3) != 'w' ) ]]; then
		current_user_rss=$( ps -e -u "${user}" -o rss= | xargs | tr ' ' '+' | bc)
		if [ "${current_user_rss}" -gt "${root_rss}" ]; then
		#	killall -u "${user}" KILL
			echo "${user}"
			echo "Kil all processes"
		fi
	fi
done < <( cat /etc/passwd | cut -d':' -f1,6 | tr ':' ' ' )
