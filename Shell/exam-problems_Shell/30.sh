#Зад. 30 2018-SE-01 Нека съществува програма за моментна комуникация (Instant  messaging), която записва логове на разговорите в следния формат:
#• има определена директория за логове (LOGDIR)
#• в нея има директорийна структура от следния вид:
#LOGDIR/протокол/акаунт/приятел/ като на всяко ниво може да има няколко екзем пляра от съответния вид,
#т.е. няколко директории протокол, във всяка от тях може да има няколко дирек тории акаунт, и във всяка от тях – няколко директории приятел
#• във всяка от директориите приятел може да има файлове с имена от вида yyyy -mm-dd-hh-mm-ss.txt
#– година-месец-ден и т.н., спрямо това кога е започнал даден разговор
#• всеки такъв файл представлява лог на даден разговор със съответния приятел , като всякаразменена реплика между вас е на отделен ред
#• даден идентификатор приятел може да се среща няколко пъти в структурата (н апр. през различни ваши акаунти сте водили разговори със същия приятел)
#Напишете скрипт, който приема задължителен позиционен аргумент - име на лог  директория (LOGDIR).
#Скриптът трябва да извежда десетимата приятели, с които имате най-много редо ве комуникация глобално
#(без значение протокол и акаунт), и колко реда имате с всеки от тях.
#Опишете в коментар как работи алгоритъмът ви.

#!/bin/bash

if [[ $# -ne 1 ]]; then
        echo "One argument needed"
        exit 1
fi

if [[ ! -d $1 ]]; then
        echo "It should be name of a dir"
        exit 2
fi

friends=$(mktemp)
while read line; do
        friend=$(dirname $line | sed -E 's/(.+)\/([^\/]+$)/\2/g' )
        num_lines=$(cat $line |wc -l)

        if (cat $friends |egrep -q "friend" ); then

                old_num_lines=$(cat $friends | egrep "$friend" | awk '{print  $2}')
                total_lines=$(echo "$num_lines + $old_num_lines" | bc)

                sed -E -i "s/^$friend $old_num_lines$/$friend $total_lines/g"  $friends

        else
                echo "$friend $num_lines" >> $friends
        fi


done< <(find $1 -type f |egrep "^[0-9]{4}\-[0-9]{2}\-[0-9]{2}\-[0-9]{2}\-[0-9 ]{2}\-[0-9]{2}\.txt$")

cat $friends | sort -nr -k2 | head

rm $friends
###################################
#v2_n

#!/bin/bash
LOGDIR="${1}"
friends=$(find $LOGDIR -mindepth 3 -maxdepth 3 -type d -printf "%f\n" | sort |uniq) 

leaderboard=$(mktemp)
for f in $friends; do
	#echo $(find .  -type f -printf "%p\n"  | grep $f | xargs  wc -l | awk -v user=$f 'END{print user,$1}')
	echo $f $(find $LOGDIR  -mindepth 3 -type f -printf "%p\n"  | grep /$f/ | xargs cat | wc -l ) >> $leaderboard
done 

cat $leaderboard
sort -k2 -nr $leaderboard | head | cut -d ' ' -f1
rm $leaderboard
