#Зад. 49 2022-IN-02 Името на дадена машина можете да вземете с командата hostname -s. Разполагате с машина, на която е инсталиран специализиран софтуер,
#който ползва два потребителски акаунта – oracle и grid.
#Всеки от потребителите би трябвало да има environment променлива ORACLE_BASE, която указва абсолютен път до директория във формат /път/до/дир.
#Всеки от потребителите би трябвало да има environment променлива ORACLE_HOME, която указва абсолютен път до директория във формат /път/до/дир.
#В поддиректорията bin на зададената директория би трябвало да има изпълним файл с име sqlplus.
#Всеки от потребителите би трябвало да има environment променлива ORACLE_SID с някакъв низ като стойност.
#Ако горните три environment променливи съществуват, всеки от потребителите може да изпълнява неговото копие на командата sqlplus със следните параметри:
#sqlplus -SL "/ as роля" @foo.sql където роля трябва да бъде низът SYSDBA при изпълнение от oracle и SYSASM при изпълнение от grid.
#И в двата случая sqlplus изпълнява SQL заявките от файла (foo.sql, името на файла няма значение) и извежда изхода на stdout. Ако съдържанието на sql файла е:
#SELECT value FROM v$parameter WHERE name = ’diagnostic_dest’;
#EXIT;
#изходът ще бъде стойността на търсения параметър diagnostic_dest в следния вид:
#oracle@astero:~$ sqlplus -SL "/ as sysdba" @a.sql
#VALUE
#--------------------------------------------------------------------------------
#/u01/app/oracle
#oracle@astero:~$
#Параметърът diagnostic_dest може да няма стойност, в който случай изведеният низ ще е празен.
#Изходът винаги е 5 реда, стойността винаги е на 4-и ред. Ако командата sqlplus не се изпълни успешно, тя ще върне ненулев exit code.
#За всеки от двата акаунта съществува директория, която ще наричаме diag_base. Конкретната директория е:
#• същата като ORACLE_BASE, ако diagnostic_dest няма стойност
#• същата като diagnostic_dest, ако diagnostic_dest има стойност
#За всеки от двата акаунта би трябвало да съществува под-директория на diag_base с име diag, която ще наричаме diag_dir.
#Съществуват три множества интересни за нас файлове:
#• множество crs – за потребител grid, в diag_dir има под-директория crs, в която има поддиректория с името на
#машината, в която има под-директория crs, в която има под-директория trace. Интересни за нас файлове в тази директория са такива,
#чието име завършва на подчертавка-число и имат разширение trc или trm, например foo_356.trc, bar_40001.trm.
#• множество tnslsnr – за потребител grid, в diag_dir има под-директория tnslsnr, в която има под-директория с името на машината,
#в която има няколко директории с различни имена. Във всяка от тези директории има под-директории alert и trace.
#Интересни за нас са файловете в alert, чието име завършва на подчертавка-число и имат разширение xml (напр. baz_78.xml)
#и файловете в trace, чието име завършва на подчертавка-число и имат разширение log (напр. qux_88231.log).
#• множество rdbms – за потребител oracle, в diag_dir има под-директория rdbms, в която има няколко под-директории, във всяка от които може да има няколко под-директории.
#Интересни за нас са само файловете в тези директории на второ ниво, чието име завършва на подчертавка-число и имат разширение trc или trm, например corge_27.trc, grault_1024.trm.
#Напишете скрипт, който може да се изпълнява само от някой от тези два акаунта, и приема задължителен първи позиционен аргумент число (в дни).
#В зависимост от това кой потребител изпълнява скрипта, той трябва да извежда на stdout за всяко множество на съответния потребител общият размер (в килобайти) на
#описаните по-горе интересни файлове за които времето на последната промяна (по съдържание) на файла е по-голямо от зададеното като параметър на скрипта.
#Примерно изпълнение и изход:
#oracle@astero:~$ ./foo.sh 42
#rdbms: 14400
#grid@astero:~$ ./foo.sh 73
#crs: 28800
#tnslsnr: 33600

#!/bin/bash

if [[ $# -ne 1 ]]; then
        echo "One argument needed"
        exit 1
fi

if [[ ! $1 =~ ^[0-9]+$ ]]; then
        echo "First argument should be number"
        exit 2
fi

user=$(whoami)
if [[ $user != "oracle" ]] && [[ $user != "grid" ]];then
        echo "You have no permissions to execute this file"
        exit 3
fi

if [[ -z $ORACLE_HOME ]] || [[ -z $ORACLE_BASE ]] || [[ -z $ORACLE_SID ]];then
        echo "Three environment variables are needed"
        exit 4
fi

if [[ $user == "oracle" ]];then
        role="sysdba"
else
        role="sysasm"
fi

name_host=$(hostname -s)
diagnostic_dest=$($ORACLE_HOME/bin/sqlplus -SL "/ as $role" @a.sql | awk 'NR==4 {print $0}')
#sed -n '4p'

if [[ ! -z $diagnostic_dest ]];then
        diag_base=$diagnostic_dest
else
        diag_base=$ORACLE_HOME
diag_dir=diag_base/diag;
if [[ $user == "oracle" ]]; then
        sum_rdbms=$(find $diag_dir/rdms -maxdepth 2 -mindepth 2 -type f -regextype egrep -regex "^.*\_[0-9]+\.(trc|trm)$" -mtime +"$1" -printf "%k\n"| awk '{sum+=$1}END{print sum}')
        echo "rdbms: $sum_rdbms"

elif [[ $user == "grid" ]]; then
        sum_crs=$(find $diag_dir/crs/$name_host/crs/trace  -type f -regextype egrep -regex "^.*\_[0-9]+\.(trc|trm)$" -mtime +"$1" -printf "%k\n"| awk '{sum+=$1}END{print sum}')
         echo "crs: $sum_crs"

        #sum_tnslsnr=$(find $diag_dir/$name_host -maxdepth 2 -type f -regextype egrep -regex "^.*\_[0-9]+\.(xml|log)$" -mtime +"$1" -printf "%k\n"| awk '{sum+=$1}END{print sum}')

        sumAlert=$(find $diag_dir/tnslnsr/$name_host/*/alert -type f -regextype egrep -regex "^.*\_[0-9]+\.xml$" -mtime +"$1" -printf "%k\n"| awk '{sum+=$1}END{print sum}')
        sumTrace=$(find $diag_dir/tnslnsr/$name_host/*/trace -type f -regextype egrep -regex "^.*\_[0-9]+\.log$" -mtime +"$1" -printf "%k\n"| awk '{sum+=$1}END{print sum}')
        sum_tnslnsnr=$(echo "$sumAlert + $sumTrace" | bc)

echo "tnslsnr:: $sum_tnslnsr"
fi
