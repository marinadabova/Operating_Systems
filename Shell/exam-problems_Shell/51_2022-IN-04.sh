#Зад. 51 2022-IN-04 Вашите колеги от съседната лаборатория работят със специа лизирана система Fuga,
#която се базирана два текстови файла – за автентификация (foo.pwd) и основен  конфигурационен файл (foo.conf).
#Двата файла би трябвало да се съхраняват в главната директория на системата,  която ще наричаме fuga.
#На всеки ред във първия файл има потребителско име (малки латински букви) и  паролата за този потребител, разделени с двоеточие, като паролата не е в чист  вид, а е хеширана.
#Форматът на конфигурационния файл е недефиниран. Тъй като конфигурационният  файл е твърде голям за удобна работа,
#вашите колеги са решили да го разделят на части в отделни малки  конфигураци онни файлове, които държат в директория fuga/cfg и нейните под-директории.
#Всеки такъв файл има име във формат bar.cfg, където bar е име на потребител  на системата. Колегите ви са написали скрипт (fuga/validate.sh),
#който приема един задължителен позиционен аргумент – име на конфигурационен  файл. Скриптът проверява указания конфигурационен файл за валиден синтаксис и  може да приключи с някой от следните exit code-ове:
#• 0 – файлът е валиден;
#• 1 – файлът не е валиден;
#• 2 – настъпила е грешка при изпълнение на скрипта.
#Ако файлът не е валиден, скриптът извежда на stdout редовете, на които има г решка, предхождани от Line x: където x е номера на реда. В останалите случаи  скриптът няма изход.
#Разполагате с команда pwgen, която генерира и извежда на stdout случайни пар оли (букви и цифри), и знаете, че поддържа следните два аргумента: pwgen [pas sword_length] [number_of_passwords]
#Също така разполагате с командата mkpasswd, която по подаден аргумент – паро ла в чист вид извежда нейният хеш на stdout.
#Помогнете на колегите си, като напишете шел скрипт, който приема параметър –  име на директория fuga. Скриптът трябва да:
#• извежда на stderr грешните редове от малките конфигурационни файлове, във  формата извеждан от валидиращия скрипт, като всеки ред трябва да започва с им ето на конфигурационния файл и знак двоеточие ’:’;
#• (ре-)генерира foo.conf като конкатенация на валидните малки конфигурационн и файлове;
#• провери за всеки валиден малък конфигурационен файл дали потребителят съще ствува в автентификационния файл и ако не – да го добави по подходящия начин, 
#като изведе на stdout потребителското име и паролата в чист вид, разделени с  двоеточие.

#!/bin/bash

if [[ $# -ne 1 ]]; then
        echo "One argument needed"
        exit 1
fi
if [[ ! -d $1 ]]; then
        echo "Argument must be dir name"
        exit 2
fi
if [[ "$1" != "fuga" ]]; then
        echo "Wrong dir name"
        exit 3
fi

while read filename; do
        user=$(basename $filename .cfg)

        if [[ $($1/validate.sh $filename | wc -l) -ne 0 ]];then
                $1/validate.sh $filename | awk -v name=$filename '{print name ":"$0}' >&2
        elif
                $1/validate.sh $filename

                if [[ $? -eq 0 ]];then
                        isPresent=$(cat $1/"foo.pwd" | egrep -q "^$user:.+$")
                        if [[ -z $isPresent ]]; then
                                paswd=$(pwgen 10 1)
                                heshPaswd=$(mkpasswd $paswd)

                                echo "$user:$hashPaswd" >> $1/"foo.pwd"
                                echo "$user:$paswd"

                                cat $filename >> $1/foo.conf
                        fi
                fi

        fi
done< <(find $1/cfg -type f -regextype egrep -regex "^.*\.cfg$")
--------------------------------------
#second resh

#!/bin/bash
if [[ $# -ne 1 ]]; then
        echo "One argument needed"
        exit 1
fi
if [[ ! -d $1 ]]; then
        echo "Argument must be dir name"
        exit 2
fi
if [[ "$1" != "fuga" ]]; then
        echo "Wrong dir name"
        exit 3
fi

temp=$(mktemp)
while read filename; do
    $1/validate.sh $filename >$temp
    exitCode=$?
    username=$(basename $filename .cfg)
    if [[ $exitCode -eq 0]]; then
        cat $filename >> $1/foo.conf     #(re)generira foo.conf kato konkatenaciq

        isPresent=$(cat $1/foo.pwd |egrep -q "^$username:.*$")
        if [[ -z $isPresent ]]; then
            passwd=$(pwgen 10 1)
            hashPasswd=$(mkpasswd $passwd)

            echo "$username:$hashPasswd" >> $1/foo.pwd
            echo "$username:$passwd"
        fi
    elif [[ $exitCode -eq 1]]; then
        cat $temp |awk -v name=$filename '{print name":"$0}' 1>&2
    else
        continue
    fi

done< <(find $1/cfg -type f -regextype egrep -regex "^.*\.cfg$")
rm $temp
