#Зад. 21 2017-IN-01 Напишете скрипт, който приема три задължителни позицонни аргумента:
#• име на фаил
#• низ1
#• низ2
#Файлът е текстови, и съдържа редове във формат:
#ключ=стойност
#където стойност може да бъде:
#• празен низ, т.е. редът е ключ=
#• низ, състоящ се от един или повече термове, разделени с интервали, т.е., редът е ключ=t1 t2 t3
#Някъде във файла:
#• се съдържа един ред с ключ първия подаден низ (низ1 );
#• и може да се съдържа един ред с ключ втория подаден низ (низ2 ).
#Скриптът трябва да променя реда с ключ низ2 така, че обединението на термовете на редовете с ключове низ1 и низ2 да включва всеки терм еднократно.

#Примерен входен файл:
#$ cat z1.txt
#FOO=73
#BAR=42
#BAZ=
#ENABLED_OPTIONS=a b c d
#ENABLED_OPTIONS_EXTRA=c e f

#Примерно извикване:
#$ ./a.sh z1.txt ENABLED_OPTIONS ENABLED_OPTIONS_EXTRA

#Изходен файл:
#$ cat z1.txt
#FOO=73
#BAR=42
#BAZ=
#ENABLED_OPTIONS=a b c d
#ENABLED_OPTIONS_EXTRA=e f

#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo "Wrong arguments are passed"
    exit 1
fi
if [[ ! -f $1 ]] ; then
    echo "The first argument must be file";
    exit 2;
fi
if [[ -z ${2} || -z ${3} ]]; then
	echo "the string should be longer than zero"
	exit 3
fi
if [[ $(cat $1 |grep -c "$2=") -eq 0 ]]; then
        echo "The first key does not exist"
        exit 3
fi
if [[ $(cat $1 |grep -c "$3=") -eq 0 ]]; then
        echo "The second key does not exist"
    exit 4
fi


str1=$(cat $1 |grep "$2=")
str2=$(cat $1 |grep "$3=")

secondKey=$(echo "$str2" | cut -d '=' -f 1)
temp=$(mktemp)
echo $str1 |cut -d '=' -f 2 |tr ' ' '\n' > $temp
secondSt=$(echo $str2 |cut -d '=' -f 2)

cat $temp | while read line; do
        if [[ $(echo $secondSt | grep -c "$line") -ne 0 ]]; then
                secondSt=$(echo $secondSt | tr -d "$line")
        fi
        #echo $secondSt

        sed -i -E "s/$str2/$secondKey=$secondSt/" $1
        str2=$(cat $1 | grep "$3=")
done

rm $temp

