#!/bin/bash

temp=$(mktemp)
while read line
do
        stat $line | head -1 | sed -E 's/(^[^\/+])(\/.+)/\2/g' >> $temp

done < <(find $1 -type l 2> /dev/null | egrep -v "$(find -L $1 -type l)")

echo "Broken Symlinks: $(find -L $1 -type l | wc -l)" >> $temp

#readlink
if [[ $# -eq 2 ]];then
        cat $temp > $2
else
        cat $temp
fi
