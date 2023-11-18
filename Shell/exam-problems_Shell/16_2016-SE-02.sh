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

#ot ypr 
#!/bin/bash

if ! [ $# -eq 1 ]; then
        echo "Wrong number of arg"
        exit 1
fi
IS_ROOT=0
if [ $(whoami) == "root" ]; then
        IS_ROOT=1
fi
if ! echo $1 |grep -q "^[1-9][0-9]*$" ; then
        echo "Pls enter num"
        exit 1

for user in $(cat /etc/passwd | cut -d ':' -f 1); do
        rss=$(ps -u $user -o rss= | awk '{sumRSS+=$1} END {print sumRSS}')
        #pid=$(ps -u $user -o pid= )

        if [ $IS_ROOT -eq 1 ]; then
                if [ $1 -gt $rss ]; then
                        pid=$(ps -u $user -o pid=,rss= | sort -k2 -n | cut -d ' ' -f1| head -1)

                        echo kill -9 $pid
                fi
        fi
done

###########################

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
        #echo "$user"
        process=$(ps -u $user -o  pid=,rss= | sort -t ' ' -n -k 2  |awk '{count+=$2} END {print count, $1}')
        pid_max=$(echo $process| cut -d ' ' -f 2)
        mem_max=$(echo $process |cut -d ' ' -f 1)
        echo "$user  $mem_max" 
       	if [[ $mem_max -gt $1 ]]; then
                #kill -TERM $pid_max
                #sleep 2
                #kill -KILL $pid_max
                echo "KILL $pid_max of user: $user with rss: $mem_max"
        fi
done
#########################

