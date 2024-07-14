#!/bin/bash - 
#===============================================================================
#
#          FILE: 2023-IN-01.sh
# 
#         USAGE: ./2023-IN-01.sh 
# 
#   DESCRIPTION: 
# 
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: YOUR NAME (), 
#  ORGANIZATION: 
#       CREATED: 04/10/24 23:03:45
#      REVISION:  ---
#===============================================================================


if [ $# == 1 ] && [ $1 == "autoconfig" ]; then
		echo "yes"
fi

SLOTS="1 2"
slots="0"
if [ ! -z "${SLOTS}" ]; then
		slots="$SLOTS"
fi

output_file=$(mktemp)
for slot in $slots; do
		cat "data/2023-IN-01/${slot}.txt" | awk '{if ($1 == "Smart") { slot = $6; model=$3;} if ($1 == "Array") { array=$2 } if ($1 == "Unassigned") {array="UN"} if ($1 == "physicaldrive") {disk=$2} if ($1 == "Current" && $2 == "Temperature") {print slot, model, array, disk, $4}}' >> $output_file 
done

while IFS=' ' read -r slot model array disk temperature; do
		diskname=$(echo "$disk" | tr -d ':')
		identifier="SSA${slot}${model}${array}${diskname}"
		if [ $# == 0 ]; then
				echo "${identifier}.value ${temperature}"
		elif [ $1 == "config" ]; then
				echo "${identifier}.label SSA $model $array $disk"
				echo "${identifier}.type GAUGE"
		else 
				echo "Invalid arguments"
				exit 1
		fi
		done < "$output_file"
# exit 0 -> pri yes tuk v tazi zadacha
