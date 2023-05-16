#Зад. 25 2017-SE-02 Напишете скрипт, който приема три задължителни позиционни параметра - директория SRC, директория DST (която не трябва да съдържа файлове) и низ АBC. 
#Ако скриптът се изпълнява от root потребителя, то той трябва да намира всички файлове в директорията SRC и нейните под-директории, които имат в името си като под-низ АBC, 
#и да ги мести в директорията DST, запазвайки директорийната структура (но без да запазва мета-данни като собственик и права,
#т.е. не ни интересуват тези параметри на новите директории, които скриптът би генерирал в DST).

#Пример:
#• в SRC (/src) има следните файлове:
#/src/foof.txt
#/src/1/bar.txt
#/src/1/foo.txt
#/src/2/1/foobar.txt
#/src/2/3/barf.txt
#• DST (/dst) е празна директория
#• зададения низ е foo

#Резултат:

#• в SRC има следните файлове:
#/src/1/bar.txt
#/src/2/3/barf.txt

#• в DST има следните файлове:
#/dst/foof.txt
#/dst/1/foo.txt
#/dst/2/1/foobar.txt



#!/bin/bash
if [[ $# -ne 3 ]]; then
        echo "Parameters should be 3"
        exit 1
fi

if [[ ! -d $1 || ! -d $2 ]]; then
        echo "First and second parameters should be dir"
        exit 2
fi

if [[ "$(whoami)" = "root"  ]]; then

        find $1 -type f 2> /dev/null | grep "$3*" | xargs -I {} mv {} $2
else
        echo "Yo have no permission"
        exit 3
fi
