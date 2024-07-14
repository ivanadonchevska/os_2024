#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

repo_path="${1}"
package_path="${2}"

# not done
# think how to solve it effective
