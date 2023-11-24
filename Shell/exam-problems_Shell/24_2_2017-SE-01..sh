#Зад. 24 2017-SE-01 Напишете скрипт, който получава задължителен първи позиционен параметър – директория и незадължителен втори – число. 
#Скриптът трябва да проверява подадената директория и нейните под-директории и да извежда имената на:
#а) при подаден на скрипта втори параметър – всички файлове с брой hardlink-ове поне равен на параметъра;
#б) при липса на втори параметър – всички symlink-ове с несъществуващ destination (счупени symlink-ове).
#Забележка:За удобство приемаме, че ако има подаден втори параметър, то той е число.

#########################
# ot ypr
#!/bin/bash

if [ $# -lt 1 -o $# -gt 2 ]; then
        echo "Parameters should be one or 2"
        exit 1
fi
if [ ! -d $1 ]; then
        echo "Should be dir"
        exit 2
fi

if [ -e $2 ]; then
        echo "a"
        find $1 -type l -printf "%p %n\n" | awk -v num=${2} '$2 > num {print $1}'
else
        echo "b"
        find $1 -type l -printf "%p %Y\n" | grep " N$"| cut -d ' ' -f 1

        #find $1 -xtype l | cut -d ' ' -f 1
        #find -L $1| cut -d ' ' -f 1
fi

##################################
#!/bin/bash

if [[ ! -d $1 ]] ; then
        echo "Wrong argument passed"
        exit 1
fi

if [[ $# -gt 2 ]]; then
        echo "One or two argument should be passed"
        exit 2
fi
if [[ $# -eq 1 ]]; then
        find  -L $1 -type l
elif [[ $# -eq 2 && $2 =~ ^[[:digit:]]+$ ]];then
        find $1 -printf "%p %n\n" 2>/dev/null | awk -v min=$2 '{if (min<=$2) {print $0}}'
else
        echo "Wrong input"
        exit 3
fi
