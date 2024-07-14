#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

num="${1}"
num=$(( $num * 60 ))
if [[ ! "${num}" =~ ^[0-9]+$ ]] || [ "${num}" -lt 2 ]; then
	echo "Expected number as argument"
	exit 1
fi

user=$( id -un )
if [ "${user}" != "oracle" && "${user}" != "grid" ]; then
	echo "Script must be executed only by oracle or grid user"
	exit 1
fi

if [ -z "${ORACLE_HOME}" ]; then
	echo "Environment variable is empty"
	exit 1
fi

adrci="${ORACLE_HOME}/bin/adrci"
if [ ! -x "${adrci}" ]; then
	echo "File adrci does not exist or user has no execute permission"
	exit 1
fi

diag_dest="/u01/app/${user}"
inital_command="SET BASE ${diag_dest}"

adr_homes=$( $adrci exec="$inital_command SHOW HOMES" )

if [ "${adr_homes}" == "No ADR homes are set" ]; then
	echo "No success"
	exit 1
fi

regex="^([^\/]+\/\b(crs|tnslsnr|kfod|asm|rdbms)\b).*"
adr_homes=$( echo "${ard_homes}" | egrep "${regex}" | sed -E "s/$regex/\1/g")

while read home; do
	$adrci exec=" $inital_command SET HOMEPATH $home; PURGE -AGE "${num}"
done < <( echo "${adr_homes}" )
