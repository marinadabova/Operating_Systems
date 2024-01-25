#Зад. 48 2022-IN-01 Както знаете, при отваряне на файл с редактора vi, той създава в същата директория временен файл с име в следния формат:
#точка, името на оригиналния файл, точка, swp. Например, при редактиране на файл с име foo.txt ще се създаде временен файл с име .foo.txt.swp.
#Напишете shell скрипт, който приема два задължителни позиционни аргумента – имена на директории. Примерно извикване: ./foo.sh ./dir1 /path/to/dir2/
#В dir1 може да има файлове/директории, директорията dir2 трябва да е празна. Скриптът трябва да копира всички обикновенни файлове от dir1 (и нейните под-директории) в dir2,
#запазвайки директорийната структура, но без да копира временните файлове, създадени от редактора vi (по горната дефиниция).
#Забележка: За удобство приемаме, че не ни вълнува дали метаданните на обектите (owner, group, permissions, etc.) ще се запазят.
#Примерни обекти:
#dir1/
#dir1/a
#dir1/.a.swp
#dir1/b
#dir1/c/d
#dir1/c/.bar.swp
#Обекти след изпълнението:
#dir2/
#dir2/a
#dir2/b
#dir2/c/d
#dir2/c/.bar.swp


#!/bin/bash

if [ $# -ne 2 ];then
        echo "Wrong numb of arg"
        exit 1
fi
if [ ! -d $1 ]; then
        echo "first arg should be dir"
        exit 2
fi
if [ ! -d $2 ]; then
        echo "first arg should be dir"
        exit 2
fi
DIR2=$2

checkDir=$(find $2 -maxdepth 0 -empty)
if [ -z $checkDir ]; then
        echo "Dir should be empty"
        exit 3
fi

#/dir1/c/.bar.swp  - sled find imame c/.bar.swp
while read file; do
        name=$(basename "$file" ) # .bar.swp
        dir=$(dirname "$file")    # c

        if [[ $name =~ ^\..*\.swp$ ]] ; then
                nameswp=$(echo "$name" | sed -E "s/^(\.)(.*)(\.swp)$/\2/g")
                allNames=$(echo "$(find $1 -type f -printf "%P\n")" |egrep "$nameswp$")
                if [ ! -z $allNames ]; then
                        continue
                fi
        fi
        mkdir -p "$2"/"$dir"
        cp "$1"/"$file" "$2"/"$dir"

done< <(find $1 -type f -printf "%P\n")

______________________________
#!/bin/bash

if [[ $# -ne 2 ]]; then
        echo "Arguments should be two"
        exit 1
fi

if [[ ! -d $1 ]] || [[ ! -d $2 ]]; then
        echo "First and second arguments should be dir"
        exit 2
fi

checkDirEmpty=$(find $2 -maxdepth 0 -empty)
if [[ -z $checkDirEmpty ]]; then
        echo "Second dir should be empty"
        exit 3
fi

while read file; do

        directory=$(dirname $file) #dir1/c
        filename=$(basename $file) #.a.swp
        new_dir=$(echo $directory | sed -E "s/(^.*$1\/?)(.*$)/\2/g") #c/.a.swp
        if [[ ! -z $new_dir ]] && [[ ! -e $2/$new_dir ]]; then
                mkdir -p $2/$new_dir
        fi

        if [[ "$filename" =~ ^.*\.swp$ ]]; then
                name=$(echo $filename | sed -E "s/(\.)(.*)(\.swp$)/\2/g")
                name_swp=$(find $directory -maxdepth 1 -type f -printf "%f\n")
                if [[ "$name_swp" != "$name" ]]; then
                        cp $file $2/$new_dir
                fi
        else
                 cp $file $2/$new_dir
        fi

done< <(find $1 -type f)
