#Зад. 45 2021-SE-04 Разполагате с машина, на която е инсталиран специализиран софтуер, който ползва два потребителски акаунта – oracle и grid.
#За всеки от двата акаунта съществува директория, която ще наричаме diag_dest и тя е от вида /u01/app/потребител.
#Всеки от потребителите би трябвало да има environment променлива ORACLE_HOME, която указва абсолютен път до директория във формат /път/до/дир.
#В поддиректорията bin на зададената директория би трябвало да има изпълним файл с име adrci.
#Всеки от потребителите може да подаде серия от подкоманди, които неговото копие на adrci да изпълни, като го извика с параметър exec="cmd1; cmd2; cmd3".
#Отделните подкоманди се разделят с точка и запетая (;). Командата adrci винаги приключва с exit code 0. Нека дефинираме следните подкоманди:
#• SET BASE – за да се гарантира правилна работа на командата adrci, при всяко нейно извикване първата подкоманда трябва да бъде от вида SET BASE diag_dest,
#където diag_dest е абсолютният път на съответната директория за дадения потребител
#• SHOW HOMES – подкоманда SHOW HOMES извежда на STDOUT един от следните два изхода:
#– вариант 1: (неуспех): No ADR homes are set
#– вариант 2: (успех):
#ADR Homes:
#diag/rdbms/orclbi/orclbi1
#diag/rdbms/orclbi/orclbi2
#Ако командата се изпълни успешно, тя връща списък с един или повече ADR Homes, които са релативни имена на директории спрямо diag_dest на съответният потребител.
#От полученият списък с релативни пътища интересни за нас са само тези, които за име на директория на второ ниво имат една от следните директории:
#crs, tnslsnr, kfod, asm или rdbms.
#• SET HOMEPATH – подкоманда SET HOMEPATH път задава активна работна директория, спрямо която ще се изпълняват последващи подкоманди в рамките на същото изпълнение на adrci;
#път е релативен път до директория, спрямо изхода на SHOW HOMES
#• PURGE – подкоманда PURGE -AGE минути изтрива определени файлове в текущо активната работна директория, по-стари от дефинираното време в минути.
#Забележка: изтриват се само безопасни файлове, т.е. такива, чието изтриване няма да доведе до проблем. Дефиницията на безопасни файлове е извън обхвата на тази задача.
#Напишете shell скрипт, който може да се изпълнява само от някой от указаните два акаунта и приема задължителен първи позиционен аргумент число (в часове, минимална стойност 2 часа).
#Скриптът трябва да почиства безопасните файлове по-стари от зададените часове в интересните ADR Home-ове на съответния потребител.

#!/bin/bash

if [[ "$(whoami)" != "oracle" ]] && [[ "$(whoami)" != "grid" ]]; then
        echo "Not a valid account"
        exit 1
fi
if [[ $# -ne 1 ]]; then
        echo "One argument needed"
        exit 2
fi
if ! [[ "${1}" =~ ^[0-9]+$ ]]; then
        echo "Argument should be a digit"
        exit 3
fi
if [[ $1 -lt 2 ]]; then
        echo "First argument should be at least 2"
        exit 5
fi
if [[ -z $ORACLE_HOME ]];then
        echo "There is no environment variable ORACLE_HOME"
        exit 3
fi
if [[ ! -x $ORACLE_HOME/bin/adrci ]];then
        "There is no executable file adrci"
        exit 4
fi
if [[ $1 -lt 2 ]];then
        echo "First argument must be at least 2"
        exit 5
fi

temp=$(mktemp)
minutes=$(echo "$1 * 60" | bc)

$ORACLE_HOME/bin/adrci exec="SET BASE /u01/app/$user; SHOW HOMES" | egrep "^[^/]+/(rdbms|crs|tnslsnr|kfod|asm)/.+" >> $temp

if (cat $temp | egrep -q "No ADR homes are set"); then
        echo "$user has no ADR Homes"
        exit 6
else
        while read line; do

                $ORACLE_HOME/bin/adrci exec="SET BASE /u01/app/$user; SET HOMEPATH ${line}; PURGE -AGE $minutes"

        done < <(cat $temp)
fi
