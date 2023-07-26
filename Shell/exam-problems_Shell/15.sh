#Зад. 15 2016-SE-01 Напишете shell скрипт, който по подаден един позиционен параметър, ако този параметър е директория, намира всички symlink-ове в нея и под-директориите `и с несъществуващ
#destination.
#!/bin/bash

if [[ ${#} -ne 1 ]]; then 
	echo "It should be one parameter"
	exit 1
fi

if [[ ! -d ${1} ]]; then
	echo "Parameter should be dir"
	exit 2
fi

find -L ${1} -type l
#find $1 -xtype l
