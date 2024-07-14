#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

number="${1}"
prefix_symbol="${2}"
unit_symbol="${3}"

# proverki

prefix=$( cat "prefix.csv" | grep ",${prefix_symbol}," | cut -d',' -f3)
unit=$( cat "base.csv" | grep ",${unit_symbol}," | awk -F',' '{ print $3 ", " $1 }')

number=$( echo ""${number}" * "${prefix}"" | bc )§

echo ""${number}" "${unit_symbol}" ("${unit}")"
