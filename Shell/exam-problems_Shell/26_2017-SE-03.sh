#Зад. 26 2017-SE-03 Напишете скрипт, който ако се изпълнява от root потребителя:
#а) извежда обобщена информация за броя и общото количество активна памет (RSS - resident set size, non-swaped physical memory that a task has used) на текущите процеси на всеки потребител;
#б) ако процесът с най-голяма активна памет на даден потребител използва два пъти повече памет от средното за потребителя, то скриптът да прекратява изпълнението му по подходящ начин.
#За справка:
#$ ps aux | head -5
#USER PID %CPU %MEM VSZ RSS TTY STAT START TIME COMMAND
#root 1 0.0 0.0 15820 1052 ? Ss Apr21 0:06 init [2]
#root 2 0.0 0.0 0 0 ? S Apr21 0:00 [kthreadd]
#root 3 0.0 0.0 0 0 ? S Apr21 0:02 [ksoftirqd/0]
#root 5 0.0 0.0 0 0 ? S< Apr21 0:00 [kworker/0:0H]
#Алтернативно, може да ползвате изхода от ps -e -o uid,pid,rss

#!/bin/bash

#if [[ ! $(whoami) != "root" ]]; then
#       echo "You do not have permission"
#       exit 1
#fi


while read user; do
#a\
#       rssAll=$(ps -u "$user" -o rss=| awk 'BEGIN {sumAll=0}{sumAll+=$1} END {print sumAll}')
         rssAll=$(ps -u "$user" -o rss=| awk '{sumAll+=$1} END {print sumAll}')
        count=$(ps -u $user | wc -l)

        echo " user $user has  number of processes $count and  total rss: $rssAll"
#b\
        double_rss_avg=$(echo "($rssAll/$count) * 2" | bc)

        max_rss_pid=$(ps -u $user -o rss=,pid= |sort -nr -k1 |head -1)
        max_rss=$(echo "$max_rss_pid" | awk '{print $1}')
        max_pid=$(echo "$max_rss_pid" | awk '{print $2}')

        #this is the same as:
        #read max_rss max_pid < $(ps -u $line -o rss=,pid= | sort -nr -k1 | head -1)
        if [[ $max_rss -gt $double_rss_avg ]]; then
                #kill -TERM $max_pid
                #sleep 2
                #kill -KILL $max_pid
                echo "kill process $max_pid with $max_rss"
        fi

done< <(ps -e -o user=| sort |uniq)


#second solution:

#users=$(mktemp)
#ps -eo user= |sort|uniq >$users
#for i in $(cat $users);do
#        echo -n "user: $1"
#       read BROI SUM_RSS MAX_PID MAX_RSS < (ps -u $i -o pid=,rss= |sort -n -k2| awk 'BEGIN {c=0}{c+=$2}END {print NR,c,$1,$2}' ) #/a

#        number=$(echo "2*($SUM_RSS/$BROI)"|bs)
#        if [[ echo $MAX_RSS -gt $number ]];then
#                kill -TERM $MAX_PID
#                sleep 2
#                kill -KILL $MAX_PID
#       fi
#done

