# to find all the files where is mentioned the word ask
find . -type f | grep 'solution.sh' | xargs -I{} grep -l 'ask' {}

# if there are empty lines for instance, they are removed
egrep ".+"

# list modification
find . -type f -printf "%T@ %f\n"

# ask for hardlinks, number
stat -c '%h' filename 
# gives number of hardlinks connected to that file, including it, for actual number of hardlinks to this file should take away 1 (-1)

# all symlinks in dir with broken destination
#1
find -L "${dir}" -type l

# 2
while read symlink; do
	if [[ $( file "${symlink}" | grep "broken" ) -gt 0 ]]; then
		echo "Broken symlink"
	fi
done < <( find "${dir}" -type l)

# all files that contain str in their name
find "${dir}" -type f -name "*${str}*" 

# to find name destination
dst=readlink -f link #link is find using find -type l

file dst #gives destination: broken symlink in case of broken

# when talk about exictence of symlink's destination, we're referring to whether the file or directory that the symlink points to is present on the filesystem

# hard links are directory entries that directly reference the inode of a file, while symbolic links are separate files that contain references to other files or directories by their paths. Hardd links are restricted to the same filesystem ans share the same data blocks while symbolic links are more flexible but may become broken if their target is moved or deleted

# Hard links
# Multiple hard links can point to the same inode, meaning multiple directory entries can reference the same phusical data block on disk
# Changinh the content of one hard link changes the content of all other hard links pointing to the same inode, because they all reference the same data blocks
# Removing one hard link does not affect other hard links to the same file until all hard links are removed

# Symbolic links
# is a special type of file that contains a rreference to another file or directory in the form of the path
# Unlinke hard links, symliks are independent files that rreference other files by their pathnames
# If the target of a symlink is moved or deleted, the symlink becomes "broken" or "dangling" meaning it no longer points to a valid location
# Symbolic links provide more flexibility and can reference directories, while hard links can only reference files

# kill all processes to userr
killall -u "${user}" KILL

# kill process with given pid
kill -s TERM "${pid}"
sleep 3
kill -s KILL "${pid}"

#fgrep search for a fixed string in a file or files. Unlike regular grep, fgrep interprets its pattern as a fixed string, meaning it searches for exact matches of the given string without interpreting any special characters or regular expressions.
fgrep ","${rest}""

# think about grep -oP
grep -oP "(?<=a | an).*(?= license)"

realpath file 
#gives real path of the file

egrep -nv "{.*};$|^#" 
# matches the lines that are not comments and those ones that are not in the right format, with line number
# ex. 1:broken

id -un # gives username

# bc gets string as parameter
# for example, to calculate product like this
number=$( echo ""${num_1}" * "${num_2}"" | bc)

if [[ ${?} -ne 0 ]]; then
	echo "Exit code op the last command is not true"
fi

# when there are more whitespaces and can not handle it using tr -s ' ', use awk
awk '{$1=$1}1'
# $1=$1 -> assigns the first field to itself, and 1 -> is a pattern that always evaluates to true, causing awk to perform the default action of printing the modified record

sed -i "s/\b${word}\b/${censored_word}/gi"

# can not create symlink if destination exist, like 
ln -s "${target}" "${destination}" 
# if destination exist, can remove it first, so when create the symlink po be possible to set the wanted destination

# matches the date in () and this 01 after the date is replaced with 22
sed -E 's/(20210419)01/\122/' input1.data 

