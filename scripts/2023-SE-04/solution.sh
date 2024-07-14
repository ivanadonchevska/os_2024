#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

dir="${1}"

if [ ! -d "${dir}" ]; then
	echo "Expected directory as argument"
	exit 1
fi

# failove s ednakvo sudurjanie -> ednakvi shasums

# free dir space before creating symlinks
free_dir_space_before=$( df -k --output=avail "${dir}" | tail -n 1)
sha_sums=$(mktemp)
echo "${free_dir_space_before}"

while read file; do
	sha256sum "${file}" >> "${sha_sums}"
done < <( find "${dir}" -type f)

#cat "${sha_sums}"

previous_sum=""
previous_file=""
symlink_file=""

while read sha_sum file; do
	if [ "${sha_sum}" == "${previous_sum}" ]; then
		rm "${file}"
		ln -s "${symlink_file}" "${file}"
	else
		previous_file="${file}"
		previous_sum="${sha_sum}"
		symlink_file="${file}"
	fi
done < <( cat "${sha_sums}" | sort -k1 )

deduplicated_cnt=$( cat "${sha_sums}" | awk '{ print $1 }' | sort | uniq -d | wc -l )
echo "Number of deduplicated files is: "${deduplicated_cnt}""

free_dir_space_after=$( df -k --output=avail "${dir}" | tail -n 1)
echo "${free_disc_space_after}"
difference=$(( $free_dir_space_after - $free_dir_space_before ))

echo "Free disk difference: "${difference}""
