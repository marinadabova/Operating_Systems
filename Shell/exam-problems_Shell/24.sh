#Зад. 24 2017-SE-01 Напишете скрипт, който получава задължителен първи позиционен параметър – директория и незадължителен втори – число.
#Скриптът трябва да проверява подадената директория и нейните под-директории и да извежда имената на:
#а) при подаден на скрипта втори параметър – всички файлове с брой hardlink-ове поне равен на параметъра;
#б) при липса на втори параметър – всички symlink-ове с несъществуващ destination (счупени symlink-ове).

#Забележка:За удобство приемаме, че ако има подаден втори параметър, то той е число.

#!/bin/bash

if [[ ! -d $1 ]]; then
        echo "Wrong argument passed"
        exit 1
fi

if [[ $# -gt 2 ]]; then
        echo "One or two argument should be passed"
        exit 2
fi

if [[ $# -eq 1 ]]; then
        find  $1 -xtype l 2> /dev/null
elif [[ $# -eq 2 && $2 =~ ^[[:digit:]]+$ ]];then
        find $1 -type f -printf "%p %n\n" 2> /dev/null | awk -v hardlink=$2 '$2 >= hardlink {print $1,$2}'
else
        echo "Wrong input"
        exit 3
fi
