#!/bin/bash

if [[ $# -ne 2 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

csv_file="${1}"
star_type="${2}"

if [[ ! -f "${csv_file}" ]]; then
	echo "Invalid file"
	exit 1
fi

if [[ ! "${star_type}" =~ ^[a-zA-Z]+ ]]; then
	echo "Second argument is expected to be a string"
	exit 1
fi
 
# naj jarka -> najmalka stojnost

valid=$(mktemp)

# namirame vsichki sudzvezdija koito imat s_ttype == star_type 
while read line; do
	s_type=$(echo "${line}" | awk -F',' '{print $5}')
	if [[ "${s_type}" == "${star_type}" ]]; then
		echo "${line}" >> "${valid}"
	fi
done < <(cat "${csv_file}" | grep "${star_type}")

# tursim sydzvezdieto koeto ima naj-mnogo dzvezdi

consellation=$(cat "${valid}" | awk -F',' '{print $4}' | sort | uniq -c | sort -rn | head -n 1 | awk '{print $2}')

# tuk ima vsichki redove, koito imat sudzvezdie ravno na iskanoto i star_type
final=$(mktemp)

while read line; do
	current_consellation=$(echo "${line}" | awk -F',' '{print $4}')
	if [[ "${current_consellation}" == "${consellation}" ]]; then
		echo "${line}" >> "${final}"
	fi
done < <(cat "${valid}")

wanted_star_name=$(cat "${final}" | awk -F',' '{print $1 "," $7 }' | sort -k2 -t',' -n | head -n 1 | awk -F',' '{print $1}')

echo "${wanted_star_name}"

rm "${valid}"
rm "${final}"

