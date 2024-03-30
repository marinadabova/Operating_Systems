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
#######################
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
########################
#!/bin/bash

if [[ ${#} -lt 1 ]]; then
        echo "expecting 1 argument at least."
        exit 1
fi

dir="$1"
f="$2"

if [ ! -d "$dir" ]; then
    echo "Error: directory does not exist" >&2
    exit 1
fi



broken_symlink=$(find -L "${dir}" -type l| wc -l)

solution() {
        while read link; do
                linked_to=$(readlink ${link})
                if [[ -e "${link}" ]]; then
                        echo "$(basename ${link}) -> ${linked_to}"
                fi

        done < <(find ${dir} -type l)

        echo "Broken symlinks: ${broken_symlink}"
}
if [[ ${#} -eq 2 ]]; then
        if [[ -f  ${f} ]]; then
                solution >> "${f}"
    else
        echo "File doesn't exist"
    fi
else
        solution
fi
