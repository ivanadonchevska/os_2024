#!/bin/bash

echo "${#}"
# "${{#}count" -> in case when we want to add somenthing to this output, if there is no need it's okay to use it without {}
# number of arguments

echo "${@}"
#reference to all arguments, it's string
# print all the given argumennts

# ; in bash separates commands given on the same line
# && -> 

# for iterates through worrds
for I in 1 2 3; do
	echo "FOR LOOP"
	echo "${I}"
done

# echio $? -> contains the exit code of the last executed command
# if there are more pipes and it's called the $? it gives the status code of the last executed command
# grep -q -> quite grep, there is no output from this command

# use [] instead of [[ ]] in if loop -> it's better
# test command -> man test -> check this one

# test -n "" -> returns 0
# [] is equal to test

# [[ ]] are not part of test
# they have more functionalities
# for example use [[ ]] when check if some variable is string, number or so on, =~
