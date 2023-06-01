#Зад. 41 2020-SE-06 Под конфигурационен файл ще разбираме файл, в който има редове от вида key=value,
#където key и value могат да се състоят от букви, цифри и знак “долна черта” (“_”). Освен това, ъв файла може да има празни редове;
#може да има произволен whitespace в началото и в края на редовете, както и около символа “=”.
#Също така са допустими и коментари в даден ред: всичко след символ “#” се приема за коментар.
#Под <date> ще разбираме текущото време, върнато от командата date без параметри; под <user> ще разбираме името на текущият потребител.
#Напишете shell скрипт set_value.sh, който приема 3 позиционни аргумента – име на конфигурационен файл, ключ (foo) и стойност (bar). Ако ключът:
#• присъства във файла с друга стойност, скриптът трябва да:
#– да закоментира този ред като сложи # в началото на реда и добави в края на реда # edited at <date> by <user>
#– да добави нов ред foo = bar # added at <date> by <user> точно след стария ред
#• не присъства във файла, скриптът трябва да добави ред от вида foo = bar # added at <date> by <user> на края на файла
#Примерен foo.conf:
## route description
#from = Sofia
#to = Varna # my favourite city!
#type = t2_1
#Примерно извикване:
#./set_value.sh foo.conf to Plovdiv
#Съдържание на foo.conf след извикването:
## route description
#from = Sofia
## to = Varna # my favourite city! # edited at Tue Aug 25 15:48:29 EEST 2020 by human
#to = Plovdiv # added at Tue Aug 25 15:48:29 EEST 2020 by human
#type = t2_1

#!/bin/bash

if [[ $# -ne 3 ]]; then
        echo "Arguments should be 3"
        exit 1
fi

if [[ ! -f $1 ]]; then
        echo "First argument should be file"
        exit 2
fi

if [[ ! -n $2 ]] || [[ ! -n $3 ]]; then
        echo "Second and third argument should be not empty strings"
        exit 3
fi

if [[ ! "$2" =~ ^[a-zA-Z0-9_]+$ ]] || [[ ! "$3" =~ ^[a-zA-Z0-9_]+$ ]]; then
        echo "Not valid key and value"
        exit 4
fi

date=$(date)
user=$(whoami)

if ( cat $1 | egrep -q "^[[:space:]]*$2[[:space:]]*=[[:space:]]*[^($3)]") && ! (cat $1 | egrep -q "^[[:space:]]*$2[[:space:]]*=[[:space:]]*$3"); then
        sed -E -i "s/(^[[:space:]]*$2[[:space:]]*=[[:space:]]*[^($3)].*$)/# \1 edited at $date by $user\n$2 = $3 # added at $date by $user/g" $1

elif [[ ! $(cat $1 |egrep -q "^[[:space:]]*[#]*$2") ]]; then
        echo "$2 = $3 # added at $date by $user" >> $1
fi
