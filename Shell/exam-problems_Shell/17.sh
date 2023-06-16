#Зад. 17 2016-SE-03 Напишете shell скрипт който, ако се изпълнява от root, проверява кои потребители на системата нямат homedir или не могат да пишат в него.
#Примерен формат:
#root:x:0:0:root:/root:/bin/bash
#daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
#bin:x:2:2:bin:/bin:/usr/sbin/nologin

#!/bin/bash
if [[ "$(whoami)" != "root" ]]; then
        echo "Not root"
        exit 1
fi
if [[ "$(whoami)" = "root" ]]; then

while IFS=':' read user homedir; do
        echo "$user"
        if [[ ! -d $home ]]; then #check if this works it was without !
                echo "Dir does not exist"
                exit 2
        else
                perm=$(stat -c "%a" $homedir | sed 's/[0-9]{1}/&/')

        fi
        if [[ ! ($perm -eq 7 || $perm -eq 2 || $perm -eq 3 ) ]];then #-eq 6?
                echo "$user doesn not have permission "
                exit 3
        fi



done < <(cat /etc/passwd | cut -d ':' -f 1,6)
fi
