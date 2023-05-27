#!/bin/bash
if [[ $# -ne 1 ]]; then
        echo "One argument needed"
        exit 1
fi

if [[ ! -d $1 ]]; then
        echo "It should be name of a dir"
        exit 2
fi


T=$(mktemp)

while read friend; do
        count=$(find $1 -mindepth 4 -maxdepth 4 -type f 2>dev/null | egrep ".*/${friend}/[0-9]+-[0-9]+-[0-9]+-[0-9]+-[0-9]+-[0-9]+\.txt" | xargs cat | wc -l)
        echo "$count ${friend}" | cat >> $T

done< <(find $1 -mindepth 3 -maxdepth 3 -type d -printf "%f\n" |sort |uniq)

cat $T | sortn -n |tail -n 10

rm -- $T
