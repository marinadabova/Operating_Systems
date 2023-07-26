#!/bin/bash
if [[ ${#} -ne 2 ]] ;then
        echo "Number of arguments should be 2"
        exit 1
fi
if [[ ! -f ${1}   || ! -f ${2} ]] ;then
        echo "Arguments should be files"
        exit 2
fi
if  [[ ! "$1" =~ ^[a-zA-Z]+$ ]] ||  [[ ! "$2" =~ ^[a-zA-Z]+$ ]]; then
    echo "Wrong format"
    exit 3
fi
count1=$(cat ${1} | grep "${1}"| wc -l)
count2=$(cat ${2} | grep "${2}"|wc -l)

if [[ ${count1} -gt ${count2} ]] ;then
        cat ${1}| cut -d ' ' -f 4- |sort > $1.songs   
        # cat ${1}| cut -d ' ' -f 4- |sort > $(basename "$1").songs
elif [[ ${count2} -gt ${count1} ]] ;then
        cat ${2}| cut -d ' ' -f 4- |sort > $2.songs
else
        cat "No winner"
fi
