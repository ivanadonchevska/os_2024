#!/bin/bash

max_failures=$(cat space.txt | tail -n +2 | grep 'Failure' | cut -d'|' -f2 | sort | uniq -c | sort -rn | head -n 1 | awk '{print $2}')

earliest_flight=$(cat space.txt | tail -n +2 | grep "${max_failures}" | sort -k1rn -t'|'| head -n 1)

output=$(cat space.txt | tail -n +2 | grep "${earliest_flight}" | awk -F'|' '{print $3 ":" $4}')

echo "${output}"
