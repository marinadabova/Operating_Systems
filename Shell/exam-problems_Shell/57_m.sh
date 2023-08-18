#|/bin/bash
 
 
#Напишете скрипт, който открива еднакви по съдържание файлове в дадена директория и използва тази информация, за да намали заетото място на диска.
#Скриптът трябва да направи две неща:
#• ако има файлове с еднакво съдържание, да направи така, че имената на тези файлове да сочат към едно и също копие на съответните данни
#• да изведе съобщение, съдържащо следната информация:
#– колко групи файлове са дедупликирани
#– колко байта от мястото на диска се е освободило
#Забележки:
#• считаме, че цялата дадена директория се намира върху една файлова система
#• ако два файла имат еднакви хеш-суми, считаме, че са еднакви по съдържание
 
 
if [ $# -ne 1 ]; then
        echo "One parameter expected"
        exit 1
fi
 
if [ ! -d "${1}" ]; then
        echo "Directory expected"
        exit 2
fi
 
tempFile=$(mktemp)
 
groupCnt=0
clearedBytes=0
 
while read file; do
        hash=$(sha256sum "$file" | awk '{print $1}')
        #store every file in tempDir with sha name
        dupFile=$(grep "$hash" "$tempFile" | cut -d ' ' -f 2)
 
        if [ ! -z "$dupFile" ]; then
                #create hard link to the duplicated file
                ln -sf "$dupFile" "$file"
                currFileBytes=$(stat -c %s "$file")
                clearedBytes=$((clearedBytes + currFileBytes))
                groupCnt=$((groupCnt + 1))
        else
                echo "$hash $file" >> "$tempFile"
        fi
done < <(find "$1" -type f)
 
echo "$groupCnt deduplicated file groups"
echo "$clearedBytes bytes saved"
 
rm -- $tempFile
 
exit 0
