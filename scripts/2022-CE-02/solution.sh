#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

device="${1}"

echo "${device}" | egrep -q "^[A-Z0-9]{,4}$"
if [[ ${?} -ne 0 ]]; then
	echo "Invalid device name"
	exit 1
fi

grep -q "${device}" /proc/acpi/wakeup
if [[ ${?} -ne 0 ]]; then
	echo "Device name does not exist in wakeup file"
	exit 1
fi

status=$( grep "${device}" /proc/acpi/wakeup | awk '{ print $3 }')
if [ "${status}" == "*disabled" ]; then
	# device status is already disabled
	exit 0
else
	# just write the device in file and this will change it's status
	echo "${device}" > /proc/acpi/wakeup
fi



