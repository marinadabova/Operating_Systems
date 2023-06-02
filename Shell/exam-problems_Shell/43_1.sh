#!/bin/bash

for file in "$@"
do
        if [[ ! -f $file ]];then
                echo "$file is not a file"
                continue
        fi

        if !(cat $file | head -1 | egrep -q ".+[[:space:]]+SOA[[:space:]]+.+") || [[ $(cat $file | egrep ".+[[:space:]]+SOA[[:space:]]+.+" | wc -l) -ne 1 ]];then
                echo "This is not valid file"
                continue
        fi

        if (cat $file | head -1 | egrep -q "^([[:alnum:]]+\.)+([[:space:]]+[[:digit:]]+)?[[:space:]]+IN[[:space:]]+SOA[[:space:]]+\1[[:space:]]+\1[[:digit:]]{10}([[:space:]]+[[:digit:]]+){4}[[:space:]]+\($");then
                prev_serial=$(cat $file | head -2 | tail -1 | awk '{print $1}')
                date=$(echo $prev_serial | cut -c -8)
                today=$(date -I | sed 's/-//g')
                if [[ $date -lt $today ]];then
                        serial="${today}"00
                elif [[ $date -eq $today ]];then
                        serial=$(echo "$prev_serial + 1" | bc)
                fi

                sed -E -i "s/(^[[:space:]]*)$prev_serial([[:space:]]*;.+$)/\1$serial\2/g" $file

        else
                prev_serial=$(cat $file | head -1 | sed -E 's/^(.*\.)([[:space:]]+)([[:digit:]]*)([[:space:]0-9]+)/\3/g')
                date=$(echo $prev_serial | cut -c -8)
        today=$(date -I | sed 's/-//g')

                 if [[ $date -lt $today ]];then
              serial="${today}"00
         elif [[ $date -eq $today ]];then
              serial=$(echo "$prev_serial + 1" | bc)
                 fi

                sed -E -i "s/^(.*\.)([[:space:]]+)([[:digit:]]*)([[:space:]0-9]+)/\1\2$serial\4/g" $file
        fi

done
