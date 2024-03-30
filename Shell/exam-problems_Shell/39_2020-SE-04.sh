#Зад. 39 2020-SE-04 Напишете скрипт, който приема два аргумента - имена на директории. Първата (SRC ) съществува, докато втората (DST) трябва да бъде създадена от скрипта.
#Директорията SRC и нейните поддиректории може да съдържат файлове, чиито имена завършат на .jpg. Имената на файловете може да съдържат интервали, както и поднизове, оградени със скоби, например:
#A single (very ugly) tree (Outdoor treks) 2.jpg
#Falcons.jpg
#Gorgonzola (cheese).jpg
#Leeches (two different ones) (Outdoor treks).jpg
#Pom Pom Pom.jpg
#За даден низ ще казваме, че е почистен, ако от него са премахнати leading и trailing интервалите и всички последователни интервали са сведени до един.
#За всеки файл дефинираме следните атрибути:
#• заглавие - частта от името преди .jpg, без елементи оградени в скоби, почистен. Примери:
#A single tree 2
#Falcons
#Gorgonzola
#Leeches
#Pom Pom Pom
#• албум - последният елемент от името, който е бил ограден в скоби, почистен. Ако албум е празен стринг, ползваме стойност по подразбиране misc. Примери:
#Outdoor treks
#misc
#cheese
#Outdoor treks
#misc
#• дата - времето на последна модификация на съдържанието на файла, във формат YYYYMM-DD
#• хеш - първите 16 символа от sha256 сумата на файла.
#Забележка: приемаме, че в тази идеална вселена първите 16 символа от sha256 сумата са уникални за всеки файл от тези, които ще се наложи да обработваме.
#Скриптът трябва да създава в директория DST необходимата структура от под-директории, файлове и symlink–ове, така че да са изпълнени следните условия за всеки файл от SRC :
#• DST/images/хеш.jpg - копие на съответния файл
#• следните обекти са относителни symlink–ове към хеш.jpg:
#– DST/by-date/дата/by-album/албум/by-title/заглавие.jpg
#– DST/by-date/дата/by-title/заглавие.jpg
#– DST/by-album/албум/by-date/дата/by-title/заглавие.jpg
#– DST/by-album/албум/by-title/заглавие.jpg
#– DST/by-title/заглавие.jpg

#!/bin/bash

if [[ $# -ne 2 ]]; then
        echo "Arguments should be two"
        exit 1
fi

if [[ ! -d $1 ]]; then
        echo "First argument should be dir"
        exit 2
fi

if [[ ! -e $1 ]]; then
        echo "Dir does not exist"
        exit 3
fi

if [[  -e $2 ]]; then
    echo "Dir2 should not exist"
        exit 4
fi

mkdir -p ./$2/images


while read file; do
        name=$(basename "$file" .jpg | sed -E 's/\([^\(\)]+\)//g'|tr -s ' ')
#album=$(basename "$file" .jpg | sed -E 's/^(.*)(\([^\(\)]*\))([^\(\)]*)$/\2/g' | sed -E 's/[\(\)]//g'|tr -s ' ' )
        #album=$(basename "$file" | sed -E 's/^(.*)(\([^\(\)]+\))([^\(\)]*)(\.jpg$)/\2/g' | sed 's/[\(\)]//g')
        file1=$(basename "${file}")
        if ! echo "${file1}" | grep -qP '\(.*\)'; then
        album=misc
    else
        album="$(basename "$file" .jpg | sed 's/.*(\(.*\))/\1/' | tr -s ' ')"
    fi

        #if [[ -z $album ]]; then
        #       album="misc"
        #fi
        date=$(stat -c "%y" "$file" | cut -d ' ' -f1)
        heshS=$(sha256sum "$file"| cut -c 1-16)

        #echo "$album"

        touch ./$2/images/$heshS.jpg
        cp "$file" "$2/images/$heshS.jpg"

        first_sym=$2/by-date/"$date"/by-album/"$album"/by-title
        if [[ ! -e "$first_sym" ]];then
                mkdir -p "$first_sym"

        fi
        ln -s $(realpath $2/images/$heshS.jpg) "$first_sym"/"$name".jpg

        second_sym=$2/by-date/"$date"/by-title
        if [[ ! -e "$second_sym" ]];then
                mkdir -p "$second_sym"
        fi
        ln -s $(realpath $2/images/$heshS.jpg) "$second_sym"/"$name".jpg

        third_sym=$2/by-album/"$album"/by-date/"$date"/by-title
    if [[ ! -e "$third_sym" ]];then
          mkdir -p "$third_sym"
    fi
    ln -s $(realpath $2/images/$heshS.jpg) "$third_sym"/"$name".jpg

        fourth_sym=$2/by-album/"$album"/by-title
    if [[ ! -e "$fourth_sym" ]];then
           mkdir -p "$fourth_sym"
    fi
    ln -s $(realpath $2/images/$heshS.jpg) "$fourth_sym"/"$name".jpg

         fifth_sym=$2/by-title
    if [[ ! -e "$fifth_sym" ]];then
           mkdir -p "$fifth_sym"
    fi
    ln -s $(realpath $2/images/$heshS.jpg) "$fifth_sym"/"$name".jpg
done< <(find $1 -type f -regextype egrep -regex ".*\.jpg" )
