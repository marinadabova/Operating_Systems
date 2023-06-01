#!/bin/bash

if [ $# -ne 3 ];then
        echo "We need 3 arguments" >&2;
        exit 1;
fi

if [ ! -f $1 ];then
        echo "We need file" >&2;
        exit 2;
fi

if [ ! -f $2 ];then
        echo "We need file" >&2;
        exit 2;
fi

if [ ! -d $3 ];then
        echo "We need dir" >&2;
        exit 3;
fi

find $3 -type f -printf "%f\n" | egrep "^.+\.cfg$" | while read line;do
        if [ $(cat $line | egrep -n -v "(^#.*|^$|^{ .+ };$|^{ .+; };$)" | wc -l) -ne 0 ];then
                echo "Error in $line";
                cat $line | egrep -n -v "(^#.*|^$|^{ .+ };$|^{ .+; };$)" | sed -E 's/(.+)/Line \1/';
                continue;
        fi

        cat $line >> $2;

        username=$(basename -s .cfg $line);

        if [ $(cat $1 | cut -d ':' -f 1 | grep -c "$username") -eq 0 ];then
                password=$(pwgen 16 1);
                echo "${username}:${password}" >> $1;
                echo "${username} ${password}";
        fi

done

exit 0;
