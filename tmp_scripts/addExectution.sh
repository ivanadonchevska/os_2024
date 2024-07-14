#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Parameter should be only one"
	exit 1
fi

if [ ! -f "$1" ]; then
	echo "Parameter is not a valid file"
	exit 2
fi

chmod +x "$1"

echo "Success"
