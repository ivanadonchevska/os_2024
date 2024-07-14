#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

file="${1}"

if [ ! -f "${file}" ]; then
	echo "Expected file as argument"
	exit 1
fi

# head -n +3 -> but there are only 2 and the script does not work correct
top_sites=$(mktemp)
cat "${file}" | cut -d' ' -f2 | sort | uniq -c | sort -rn -k1 | awk '{ print $2 }' > "${top_sites}"

requests_output=$(mktemp)
while read site; do
	HTTP=$( cat "${file}" | awk -v var="${site}" '$2==var {print $8}' | grep 'HTTP/2.0' | wc -l)
	NON_HTTP=$( cat "${file}" | awk -v var="${site}" '$2==var {print $8}' | grep -v 'HTTP/2.0' | wc -l)
	
	echo ""${site}" HTTP/2.0: "${HTTP}" non-HTTP/2.0: "${NON_HTTP}"" >> "${requests_output}"

done < <( cat "${top_sites}" )

cat "${requests_output}"

# here also should have head -n +5, to get the first 5, but there is only one
cat "${file}" | awk '{ print $1 " " $9 }' | awk ' $2==302 {print $1}' | sort | uniq -c | sort -rn -k1 

rm "${top_sites}"
rm "${requests_output}"
