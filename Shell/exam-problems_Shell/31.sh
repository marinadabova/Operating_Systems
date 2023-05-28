#Зад. 31 2018-SE-02 Напишете скрипт, който приема два позиционни аргумента – име на текстови файл и директория.
#Директорията не трябва да съдържа обекти, а текстовият файл (US-ASCII) е стенограма и всеки ред е в следния формат:
#ИМЕ ФАМИЛИЯ (уточнения): Реплика
#където:
#• ИМЕ ФАМИЛИЯ присъстват задължително;
#• ИМЕ и ФАМИЛИЯ се състоят само от малки/главни латински букви и тирета;
#• (уточнения) не е задължително да присъстват;
#• двоеточието ‘:’ присъства задължително;
#• Репликата не съдържа знаци за нов ред;
#• в стринга преди двоеточието ‘:’ задължително има поне един интервал между ИМЕ и ФАМИЛИЯ;
#• наличието на други интервали където и да е на реда е недефинирано.
#Примерен входен файл:
#John Lennon (The Beatles): Time you enjoy wasting, was not wasted.
#Roger Waters: I’m in competition with myself and I’m losing.
#John Lennon:Reality leaves a lot to the imagination.
#Leonard Cohen:There is a crack in everything, that’s how the light gets in.
#Скриптът трябва да:
#• създава текстови файл dict.txt в посочената директория, който на всеки ред да съдържа:
#ИМЕ ФАМИЛИЯ;НОМЕР
#където:
#– ИМЕ ФАМИЛИЯ е уникален участник в стенограмата (без да се отчитат уточненията);
#– НОМЕР е уникален номер на този участник, избран от вас.
#• създава файл НОМЕР.txt в посочената директория, който съдържа всички (и само) редовете на дадения участник.

#!/bin/bash


if [[ $# -ne 2 ]]; then
        echo "Arguments should be two"
        exit 1
fi

if [[ ! -f $1 ]]; then
        echo "First argument should be file"
        exit 2
fi
if [[ ! -d $2 ]]; then
        echo "Second argument should be dir"
        exit 3
fi

checkEmptyDir=$(find $2 -maxdepth 0 -empty)
if [[ -z $checkEmptyDir ]]; then
        echo "Not empty dir"
        exit 4
fi

#lines=$(egrep "(^[a-zA-Z-]+[[:space:]]+([a-zA-Z-]+)[[:space:]]?(:?\([a-zA-Z-]+\))?:([^\\n]+)" "$1")
#for line in "${lines}"; do

touch "$2/dict.txt"
num=1
while read line; do
        names=$(echo "$line" | cut -d ':' -f1 |sed -E 's/\(.*\)//' |tr -s ' ')
        number=$(cat $2/dict.txt | grep  "$names"| cut -d ';' -f2)

        if [  -z $number ]; then
                echo "$names;$num" >> "$2/dict.txt"
                number=$num
                num=$(echo "$num+1" | bc)
    fi
    echo $line | egrep "$name" >> "$2/$number.txt"


done< <(cat $1)
