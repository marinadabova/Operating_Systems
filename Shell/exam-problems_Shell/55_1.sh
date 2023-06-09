# Зад. 55 2023-IN-02
#!/bin/bash

if [[ $# -ne 1 ]]; then
	echo "wrong input"
	exit 1
fi

if [[ ! -d "${1}" ]]; then
	echo "not a dir"
	exit 2
fi

T=$(mktemp -d)

while read inode num file; do
	hash_sum=$(sha1sum "$file" | cut -d' ' -f1)
	echo "$inode" $num >> "$T/$hash_sum"
done < <(find "$1" -type f -printf '%i %n %p\n')

while read file;do
	data=$(sort "$file" | uniq)
	count=$(echo "$data" | cut -d' ' -f2)

	if [[ -z "(echo "$data" | egrep -v ' 1$')" ]]; then
		while read inode; do
			find "$1" -type f -inum "$inode"
		done < <(echo "$data" | tail -n +2 | cut -d' ' -f1)
	elif ! echo "$count" | egrep -q ' 1$'; then
		while read inode; do
			find "$1" -type f -inum "$inode" | tail -n 1
		done < <(echo "$data" |  cut -d' ' -f1)
	else
		while read inode; do
			find "$1" -type f -inum "$inode" | tail -n 1
		done < <(echo "$data" | egrep ' 1$' | cut -d' ' -f1)
		
		while read inode; do
			find "$1" -type f -inum "$inode" | tail -n 1
		done < <(echo "$data" | egrep -v ' 1$' | cut -d' ' -f1)
	fi
done < <(find "$T" -type f)

rm -r "$T"
