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

while IFS=':' read user homedir; do
        #echo "$user"
        if [[ ! -d $homedir ]]; then 
                echo "$user doesn not have homedir"
                exit 2
        else
        	perm=$(stat -c '%a' $homedir | cut -c 1)  # perm=$(stat -c "%a" $homedir | sed 's/[0-9]{1}/&/')
		if [[  ${perm} -ne 7 && ${perm} -ne 6 && ${perm} -ne 3 && ${perm} -ne 2 ]] ; then
                	echo "$user does not have write permission "
                	exit 3
        	fi
        fi
        
done < <(cat /etc/passwd | cut -d ':' -f 1,6)
fi
