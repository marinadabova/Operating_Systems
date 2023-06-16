#Зад. 16 2016-SE-02 Напишете shell скрипт, който приема един позиционен параметър - число. 
#Ако скриптът се изпълнява като root, да извежда обобщена информация за общото количество активна памет 
#(RSS - resident set size, non-swapped physical memory that a task has used) на процесите на всеки потребител.
#Ако за някой потребител обобщеното число надвишава подадения параметър, да изпраща подходящи сигнали за прекратяване на процеса с най-много активна памет на потребителя.
#Забележка: Приемаме, че изхода в колоната RSS е число в същата мерна единица, като числото, подадено като аргумент. 
#Примерен формат:
#USER PID %CPU %MEM VSZ RSS TTY STAT START TIME COMMAND
#root 1 0.0 0.0 15816 1884 ? Ss May12 0:03 init [2]
#root 2 0.0 0.0 0 0 ? S May12 0:00 [kthreadd]
#root 3 0.0 0.0 0 0 ? S May12 0:02 [ksoftirqd/0]
#Забележка: Алтернативно може да ползвате изхода от ps -e -o uid,pid,rss

#!/bin/bash
if [[ ${#} -ne 1 ]]; then
        echo "It should be one parameter"
        exit 1
fi

if [[ ! ${1} =~ ^[0-9]+$ ]];then
        echo "Parameter should be number"
        exit 2
fi

if [[ "$(whoami)" != "root" ]]; then
        echo "Not root"
        exit 3
fi

for user in $(ps -e -o user= |sort |uniq); do
        echo "$user"
        process=$(ps -u $user -o  pid=,rss= | sort -t ' ' -n -k 2  |awk '{count+=$2} END {print count, $1}')
        pid_max=$(echo $process| cut -d ' ' -f 2)
        mem=$(echo $process |cut -d ' ' -f 1)
       
       	if [[ $mem -gt $1 ]]; then
                 #kill -KILL $pid
                 echo "KILL $pid_max of user: $user with rss: $mem"
        fi
done
