#!/bin/bash

if [ $# -gt 0 ]; then
	echo "No arguments expected"
	exit 1
fi

if [ "$( id -un )" != "oracle" && "$( id -un )" != "grid" ]; then
	echo "Script must be executed only by oracle or grrid user"
	exit 1
fi

if [ -z "${ORACLE_HOME}" ]; then
	echo "Environment variable is empty"
	exit 1
fi

if [ ! -x "${ORACLE_HOME}/bin/adrci" ]; then
	echo "File adrci does not exist or user has no execute access"
	exit 1
fi

output=$( "${ORACLE_HOME}/bin/adrci" exec="show homes" )

if [ "${output}" == "No ADR homes are set" ]; then
	echo "No success"
	exit 0
fi

while read line; do
	size=$( du -sm "${line}" | awk '{print $1}' )
	# /u01/app/potrebitel instead of ORACLE_HOME
	# isto prvite primeri sa narochno so interval, handle it
	path="/u01/app/$user/${line}"

	echo ""${size}" "${path}""
done < <( echo "${output}" | tail -n +2)

# ask
# not sure how should this task work
# done
# discussed 
