#Зад. 27 2017-SE-04 Напишете shell script, който получава задължителен първи позиционен параметър – директория и незадължителен втори – име на файл.
#Скриптът трябва да намира в подадената директория и нейните под-директории всички symlink-ове
#и да извежда (при подаден аргумент файл – добавяйки към файла, а ако не е – на стандартния изход) за тях следната информация:
#• ако destination-a съществува – името на symlink-а -> името на destination-а;
#• броя на symlink-овете, чийто destination не съществува.
#Примерен изход:
#lbaz -> /foo/bar/baz
#lqux -> ../../../qux
#lquux -> /foo/quux
#Broken symlinks: 34


#!/bin/bash

if [[ $# -eq 0 ]] || [[ $# -gt 2 ]]; then
        echo "One or two  arguments needed"
        exit 1
fi

if [[ ! -d $1 ]]; then
        echo "First argument should be dir"
        exit 2
fi
temp=$(mktemp)
brokenCount=0;
while read -d "\0" file; do
        #if realpath -qe $file ; then
                #readlink = find "$1" -type l -printf '%l\n'
        #       echo "$(basename "${file}") -> $(readlink ${file})"
        #else
                #brokenCount=$(echo "brokenCount+1" |bc)
#       fi
echo "Broken Symlinks: $(find -L $1 -type l | wc -l)" >> $temp

done< <(find $1 -type l -print0 2>/dev/null)
#echo "Broken sumlinks: $brokenCount"
if [[ $# -eq 2 ]];then
        cat $temp > $2
else
        cat $temp
fi
