#Зад. 42 2021-SE-01 Разполагате с машина, на която е инсталиран специализиран софтуер, който ползва два потребителски акаунта – oracle и grid.
#Всеки от потребителите би трябвало да има environment променлива ORACLE_HOME, която указва абсолютен път до директория във формат /path/to/dir.
#В поддиректория bin на зададената директория би трябвало да има изпълним файл с име adrci. Всеки от двата потребителя има собствена директория diag_dest,
#която е във вида /u01/app/потребител. Когато някой от потребителите изпълни неговото копие на командата adrci с параметър
#exec="show homes" може да получи на STDOUT един от следните два изхода:
#• вариант 1: (неуспех): No ADR homes are set
#• вариант 2: (успех):
#ADR Homes:
#diag/rdbms/orclbi/orclbi1
#diag/rdbms/orclbi/orclbi2
#И в двата случая командата приключва с exit code 0. Ако командата се изпълни успешно, тя връща списък с един или повече ADR Homes,
#които са релативни имена на директории спрямо diag_dest на съответният потребител.
#Напишете скрипт, който може да се изпълнява само от някой от тези два акаунта, и извежда на STDOUT размера в мегабайти и абсолютният път на всеки ADR Home.
#Примерен изход:
#0 /u01/app/oracle/diag/rdbms/orclbi/orclbi1
#389 /u01/app/oracle/diag/rdbms/orclbi/orclbi2

#!/bin/bash


if [[ "$(whoami)" != "oracle" ]] && [[ "$(whoami)" != "grid" ]]; then
        echo "Wrong user accounts"
        exit 1
fi

if [[ -z $ORACLE_HOME ]]; then
        echo "You must have environmental variable"
        exit 2
fi

if [[ ! -x $ORACLE_HOME/bin/adrci ]]; then
        echo "You dont have executable file adrci"
        exit 3
fi
temp=$(mktemp)

if [[ $($ORACLE_HOME/bin/adrci exec="show homes" | wc -l) -gt 1 ]];then

        $ORACLE_HOME/bin/adrci exec="show homes" >> $temp
        while read line; do
                path=$(/u01/app/$user/$line)
                size=$(echo "$(stat $path -c "%s") / 1048576" | bc)
                echo "$size $path"
        done < <(cat $temp | tail -n +2)
else
        echo "No ADR homes are set"
fi
rm -- $temp
