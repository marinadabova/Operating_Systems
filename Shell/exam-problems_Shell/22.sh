#Зад. 22 2017-IN-02 Напишете скрипт, който приема задължителен позиционен аргумент - име на потре-
#бител FOO. Ако скриптът се изпълнява от root:
#а) да извежда имената на потребителите, които имат повече на брой процеси от FOO, ако има
#такива;
#б) да извежда средното време (в секунди), за което са работили процесите на всички потребители
#на системата (TIME, във формат HH:MM:SS);
#в) ако съществуват процеси на FOO, които са работили над два пъти повече от средното време,
#скриптът да прекратява изпълнението им по подходящ начин.
#За справка:
#$ ps -e -o user,pid,%cpu,%mem,vsz,rss,tty,stat,time,command | head -5
#USER PID %CPU %MEM VSZ RSS TT STAT TIME COMMAND
#root 1 0.0 0.0 15820 1920 ? Ss 00:00:05 init [2]
#root 2 0.0 0.0 0 0 ? S 00:00:00 [kthreadd]
#root 3 0.0 0.0 0 0 ? S 00:00:01 [ksoftirqd/0]
#root 5 0.0 0.0 0 0 ? S< 00:00:00 [kworker/0:0H]

#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Wrong arguments are passed"
    exit 1
fi

user_processes=$(ps -u $1| wc -l)
echo "All processes of user "$1" :$user_processes"
echo "All names with more processes than "$1":"
while read line;do
        #a/
        echo "$line" | awk -v user=$user_processes '$1 > user {print $2}'
done< <(ps -eo user=| sort| uniq -c)

#b/
#avg_time=$(ps -eo time= |sed -E 's/(^[0-9]{2}):([0-9]{2}):([0-9]{2})/\1*3600 + \2*60 + \3/g'|bc |awk '{sum+=$1} END {print sum}' )

avg_time=$(ps -eo time= | awk -F ':' '{print ($1 *3600) + ($2*60)+ $3}'|paste -s -d '+' - |bc )
echo "Average time in seconds: "$avg_time" "



#c/

#double_avg=$(echo "$avg_time *$avg_time" |bc)
#echo "$double_avg"

ps -u $1 -o user= | while read user; do
        user_time=$(ps -u $1 -o time= | awk -F: '{ print ($1 * 3600) + ($2 * 60) + $3 }')
if [[ $(echo "$user_time > 2*$avg_time" | bc -l) ]]; then
                #kill -KILL $(ps -u $1 -o pid=)
                echo "killing"
        fi
done

