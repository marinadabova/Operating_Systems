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
    echo "Arguments should be 3"
    exit 1
fi

if [[ ! -f $1 ]]; then
    echo "First argument should be file"
    exit 2
fi
str1=$2
str2=$3

str1_st=$(cat $1 |grep "^$2=" | cut -d '=' -f 2-)
str2_st=$(cat $1 |grep "^$3=" | cut -d '=' -f 2-)
if [[ -z "$str2_st" ]]; then
	echo "not found key"
  exit 3
fi 
temp=$(mktemp)
echo "$str1_st" |tr ' ' '\n' > $temp
cpStr2_st=$str2_st
while read char; do
   if [[ $(echo $str2_st | grep -c "$char") -ne 0 ]]; then
        cpStr2_st=$(echo $cpStr2_st |tr -d "$char")
    fi
done< <(cat $temp)

sed -i -E "s/$str2=$escaped_str2_st/$str2=$escaped_cpStr2_st/" "$1"
rm $temp
 
#FOO=73
#BAR=42
#BAZ=
#ENABLED_OPTIONS=a b c d
#ENABLED_OPTIONS_EXTRA= e f #ima space predi e-to




