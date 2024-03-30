#!/bin/bash
if [[ $# -ne 3 ]]; then
        echo "Parameters should be 3"
        exit 1
fi

if [[ ! -d $1 || ! -d $2 ]]; then
        echo "First and second parameters should be dir"
        exit 2
fi

if [[ ! $3 =~ ^[a-zA-Z]+$ ]]; then
        echo "Third argument should be string"
        exit 3
fi

checkEmptyDir=$(find $2 -maxdepth 0 -empty)
if [[ -z $checkEmptyDir ]]; then
        echo "Not empty dir"
        exit 4
fi
#if [[ "$(whoami)" != "root"  ]]; then
#       echo "You do not have permission"
#       exit 5
#fi

while read line
do
        dir=$(echo "$line" | sed -E 's/(^[^\/]+\/)(.+)/\2/g' | sed -E 's/(.+)\/([^\/]+$)/\1/g')
        file=$(echo "$line" | sed -E 's/(^[^\/]+)(.+)/\2/g')
        echo "$dir"

        if [ -d $(echo "$1/$dir") ];then
            mkdir -p $2/$dir
        fi
        mv  $line $2$file
        #cp $line $2$file
done < <(find $1 -type f 2>/dev/null | egrep "$3" )
