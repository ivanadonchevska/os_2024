#!/bin/bash

if [ "${#}" -ne 3 ]; then
	echo "Wrong argument count!"
	exit 1
fi

if [ ! -f "${1}" ]; then

	echo "First argument must be a file!"
	exit 2
fi

WORD="[[:alnum:]_]+"

if ! echo "${2}" | grep -q -E "${WORD}"; then
	echo "Second argument must be a valid word!"
	exit 3
fi

if ! echo "${3}" | grep -q -E "${WORD}"; then
	echo "Second argument must be a valid word!"
	exit 4
fi

LINE="^ *${WORD} *= *${WORD} *$"

if cat "${1}" | cut -d'#' -f 1 | grep -E ".+" |grep -E -q -v "${LINE}" ; then
	echo "Conf file is not the exptected format!"
	exit 5
fi

while read LINEVAL; do
	echo "${LINEVAL}"
done < "${1}"

