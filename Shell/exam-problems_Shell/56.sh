#Зад. 56 2023-SE-01
#Напишете скрипт, който цензурира всички срещания на “забранени” думи в дадени текстове.
#Примерно извикване: ./redact.sh bad_words.lst ./my_texts.
#Първият аргумент на скрипта е име на текстов файл, съдържащ по една забранена дума на ред:
#cake
#cakes
#shake
#banana
#pine_apple42
#shakinator
#Вторият аргумент е име на директория: интересуват ни всички файлове в нея и в нейните подди-
#ректории, чиито имена завършват на .txt.
#Скриптът ви трябва да подмени всички срещания на забранени думи във въпросните файлове с
#брой звездички, съответстващ на дължината на думата. Подменят се само цели срещания на думи.
#Например, ако имаме файл ./my_texts/shake.txt със съдържание:
#to make banana shake, we start by blending four bananas.
#след изпълнение на скрипта, съдържанието му трябва да е:
#to make ****** *****, we start by blending four bananas.
#Под “дума” разбираме последователност от букви, цифри и долни черти.
#За улеснение, може да приемете, че разглеждаме само малки букви (никъде не се срещат главни
#букви).
#За бонус точки: премахнете улесненито, правейки операцията по цензуриране case-insensitive
#(файлът със забранени думи пак съдържа само малки букви, но в текстовете могат да се срещнат варианти на думите с произволни комбинации от малки и главни букви): ако в примерния текст се срещне думата BaNaNa, тя трябва да бъде заменена с ******, защото banana е забранена дума.

#!/bin/bash

if [[ $# -ne 2 ]]; then
        echo "Not enough arguments"
        exit 1
fi

if [[ ! -f $1 ]]; then
        echo "Expecting first arguments file"
        exit 2
fi

if [[ ! -d $2 ]]; then
        echo "Expecting second argument directory"
        exit 3
fi


while read file; do
        while read word; do
                result=$(echo $word |tr '[a-z0-9_]' '*')
                sed -i -E "s/\b$word\b/$result/gI" $file

        done < $1

done< <(find $2 -type f -regextype egrep -regex "^.*\.txt$")
#second solution:
#while read file; do
#        while read word ; do
#
#               word_length="${#word}"
#                result=""
#                for i in $(seq 1 $word_length); do
#                       result+='*'
#                done
#        done < $1

#done< <(find $2 -type f -name "*.txt")
