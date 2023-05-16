
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
