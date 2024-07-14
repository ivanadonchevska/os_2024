#!/bin/bash

function process_group() {
	
	CASE=$(echo "${1}" | awk '{print $3}' | sort -n | uniq)
	
	echo "THESE ARE YOUR FILES----------"

	echo "${1}"
	
	echo "NOW EXECUTING RM----------"

	LINES=$(echo "${CASE}" | wc -l )

	if [[ "${LINES}" -eq 1 ]] && [[ "${CASE}" -eq 1 ]]; then
			#we only have file multiples
			echo "${1}" | tail -n +2 |  awk '{print "rm" $2}'
		else
		
		MARK=''

		while read file inode; do
			
			if [[ "${MARK}" != "${inode}" ]]; then
			
			echo "rm ${file}"
			MARK=${inode}

			fi

		done	< <(echo "${1}" | awk '{print $2}' | xargs stat --printf='%n %i\n' | sort)

		
			
	fi

}

if [[ "${#}" -ne 1 ]]; then
	exit 1
fi

if [[ ! -d "${1}" ]]; then
	exit 2
fi

TEMP=$(mktemp)
MORETEMP=$(mktemp)

while read file; do
	
	(echo $(md5sum "${file}")) >> "${TEMP}"	

done < <(find "${1}" -type f)

(while read input; do
	
	echo -n "${input} "
	echo $(echo "${input}" | awk '{print $2}' | xargs stat --printf="%h\n" )

done < "${TEMP}") > "${MORETEMP}"

MARKER="$(cat ${MORETEMP} | sort | head -n 1 | cut -d ' ' -f 1)"

EVAL=''

while read input; do
	
	if [[ "${MARKER}" == "$(echo ${input} | cut -d ' ' -f 1)" ]]; then
		EVAL="${EVAL}\n${input}"
	else

		MARKER="$(echo ${input} | cut -d ' ' -f 1)"
		
		process_group "$(echo -e ${EVAL})"	
	
		EVAL="${input}"
		echo "--------------------------"
	fi
	

done < <(cat "${MORETEMP}" | sort)



rm "${TEMP}"
rm "${MORETEMP}"
