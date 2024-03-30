#Зад. 23 2017-IN-03 Напишете скрипт, който извежда името на потребителския акаунт, в чиято home директория има най-скоро променен обикновен файл и кой е този файл. 
#Напишете скрипта с подходящите проверки, така че да бъде валиден инструмент.

#!/bin/bash
while read user home;do
        recents="$( find $home -u $user -type f -printf "%f %T@\n" 2>/dev/null |sort -nr -t ' ' -k 2 |head -1)"
        if [[ -n "$recents" ]]; then
                echo "$recents $user"
                else
                        continue

    #    echo $recents $user
                fi
        done < <(cat /etc/passwd | cut -d ':' -f 1,6 | tr ':' ' ') | sort -nr -t ' '  -k 1 | cut -d ' ' -f 2-

#find /home/students/ -type f -printf "%T@ %p \n" 2>/dev/null | sort -r | head -n 1 | awk -F '/' '{print $4" "$6}';
