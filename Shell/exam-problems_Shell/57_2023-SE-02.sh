#2023-SE-02
#!/bin/bash

[[ "${#}" -eq 1 ]] || { echo "Wrong number of arguments" >&2; exit 1; }
[[ -d "${1}" ]] || { echo "${1} not a directory"  >&2; exit 2; }

TMP_HASHES=$(mktemp -d)

while read -rd $'\0' FILE; do
    HASH=$(sha1sum "${file}" | cut -d' ' -f1)
    echo -n "${file}" >> "${TMP_HASHES}/${HASH}"
    echo -ne '\0' >> "${TMP_HASHES}/${HASH}"
done < <(find "${1}" -type f -print0)

TOTAL_GROUPS=0
TOTAL_BYTES=0

while read GROUP; do
    MAIN=$(head -zn 1 "${GROUP}")
    read M_INODE SIZE < <(stat -c "%i %s" "${MAIN}")

    GROUP_CHANGED=0

    while read -rd $'\0' OTHER; do
        read O_INODE O_COUNT < <(stat -c '%i %h' "${OTHER}")

        if [[ "${O_INODE}" -ne "${M_INODE}" ]]; then
            GROUP_CHANGED=1
            rm "${OTHER}"
            ln "${MAIN}" "${OTHER}"
            if [[ "${O_COUNT}" -eq 1 ]]; then
                TOTAL_BYTES=$((TOTAL_BYTES + SIZE))
            fi
        fi
    done < <(tail -zn +2 "${GROUP}")
    TOTAL_GROUPS=$((TOTAL_GROUPS + GROUP_CHANGED))
done < <(find "${TMP_HASHES}" -type f)

echo "Deduplicated groups: ${TOTAL_GROUPS}"
echo "Bytes freed: ${TOTAL_BYTES}"

rm -r "${TMP_HASHES}"

##############################
#!/bin/bash
 
 
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
