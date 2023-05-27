#Зад. 29 2017-SE-06 Напишете скрипт, който ако се изпълнява от root потребителя, намира процесите на потребителите, които не са root потребителя и е изпълнено поне едно от следните неща:
#• имат зададена несъществуваща home директория;
#• не са собственици на home директорията си;
#• собственика на директорията не може да пише в нея.
#Ако общото количество активна памет (RSS - resident set size, non-swaped physical memory that a task has used) на процесите на даден такъв потребител
#е по-голямо от общото количество активна памет на root потребителя, то скриптът да прекратява изпълнението на всички процеси на потребителя.
#За справка:
#$ ps aux | head -n 5
#USER PID %CPU %MEM VSZ RSS TTY STAT START TIME COMMAND
#root 1 0.0 0.0 15820 1052 ? Ss Apr21 0:06 init [2]
#root 2 0.0 0.0 0 0 ? S Apr21 0:00 [kthreadd]
#root 3 0.0 0.0 0 0 ? S Apr21 0:02 [ksoftirqd/0]
#root 5 0.0 0.0 0 0 ? S< Apr21 0:00 [kworker/0:0H]
#Алтернативно, може да ползвате изхода от ps -e -o uid,pid,rss
#root:x:0:0:root:/root:/bin/bash
#daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
#s61934:x:1177:504:Mariq Cholakova:/home/SI/s61934:/bin/bash

#!/bin/bash

if [[ $(whoami) != "root" ]]; then
        echo "Not root"
        exit 1
fi

rss_root=$(ps -u "root" -o rss=|awk '{totalRss+=$1} END {print totalRss}')
while read line; do
        homeDir=$(echo "$line" | awk '{print $2}')
        user=$(echo "$line" | awk '{print $1}')

        if [[ ! -e $homeDir ]] || [[ $(stat -c "%U" $homeDir) != "$user" ]] || [[ $(stat -c "%A" $homeDir| cut -c 3) != "w" ]]; then

                rss_user=$(ps -u "$user" -o rss=| awk '{totalRss+=$1} END {print totalRss}')

                if [[ $rss_user -gt $rss_root ]]; then
                        #killall -KILL $(ps -u $user pid=)
                        #killall -u $user -TERM
                        echo "killing $(ps -u $user -o pid=)"
                fi
        fi

done< <(cat /etc/passwd |grep -v "root"| cut -d ':' -f 1,6| tr ':' ' ')
