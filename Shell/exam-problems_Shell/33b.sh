#Зад. 33 2019-SE-01 Напишете два скрипта (по един за всяка подточка), които четат редове от STDIN.
#Скриптовете трябва да обработват само редовете, които съдържат цели положителни или отрицателни числа;
#останалите редове се игнорират. Скриптовете трябва да извежда на STDOUT:
#б) всички най-малки уникални числа от тези, които имат максимална сума на цифрите си
#Примерен вход:
#We don’t
#n11d n0
#educat10n
#12.3
#6
#33
#-42
#-42
#111
#111
#-111
#Примерен изход за б):
#-42

#!/bin/bash


file=$(mktemp)
sums=$(mktemp)


while read line; do
        if [[ ! "$line" =~ ^-?[0-9]+$ ]]; then
                continue
        fi
        echo "$line" >> $file

done
while read num; do
        sum=0
        saveNum=$num
        num=$(echo $num| tr -d '-')

        while [ "$num" -ne 0 ]; do
                cif=$(echo "$num%10" |bc)
                sum=$(echo "$cif + $sum" |bc)
                num=$(echo "$num /10" |bc)
        done
        echo "$saveNum $sum" >> "$sums"

done< <(cat $file)

max_sum=$(cat $sums |sort -nr -t' ' -k 2 |head -1 |cut -d ' ' -f 2)
result=$(cat $sums | grep "$max_sum" | sort -n -t ' ' -k1 |head -1|cut -d ' ' -f 1)
echo "$result"

rm -- $file
rm -- $sums
