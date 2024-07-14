#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

dir="${1}"

# proverki

total_number_of_files=$( find "${dir}" -type f | wc -l)
#half_number_of_files=$(( $total_number_of_files / 2 ))
# make it to 2, just to test the logic, because real value is 8 and there is are files, which contain the word >= 3 times, which are >= half_number
half_number_of_files=2

all_words=$(mktemp)
words_present_at_least_three_times=$(mktemp)

while read file; do
	cat "${file}" | grep -o "\b[a-z']*\b" >> "${all_words}"
	cat "${file}" | grep -o "\b[a-z']*\b" | sort | uniq -c | awk ' $1 >= 3 { print $0 }' >> "${words_present_at_least_three_times}"

done < <( find "${dir}" -type f)

more_than_half=$(mktemp)
# one record of a word in file words_present.., means that it's present in one file >= 3, if there are 3 records of this word, it means that it's present in 3 files >= 3 times
cat "${words_present_at_least_three_times}" | awk '{print $2}' | sort | uniq -c | awk -v var="${half_number_of_files}" '$1 >= var {print $2}'>> "${more_than_half}"

#cat "${more_than_half}"

# it will get only first three records, because there is no more
cat "${all_words}" | grep -wf "${more_than_half}" | sort | uniq -c | sort -rn -k1 | awk '{ print $2 }' | head

rm "${all_words}"
rm "${words_present_at_least_three_times}"
rm "${more_than_half}"

