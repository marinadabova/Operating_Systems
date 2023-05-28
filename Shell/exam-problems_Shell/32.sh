#Зад. 32 2018-SE-03 Напишете скрипт, който приема два позиционни аргумента – имена на текстови файлове в CSV формат:
#8,foo,bar,baz
#2,quz,,foo
#12,1,3,foo
#3,foo,,
#5,,bar,
#7,,,
#4,foo,bar,baz
#Валидни са следните условия:
#• CSV файловете представляват таблица, като всеки ред на таблицата е записан на отделен ред;
#• на даден ред всяко поле (колона) е разделено от останалите със запетая;
#• броят на полетата на всеки ред е константа;
#• в полетата не може да присъства запетая, т.е., запетаята винаги е разделител между полета;
#• ако във файла присъстват интервали, то това са данни от дадено поле;
#• първото поле на всеки ред е число, което представлява идентификатор на реда (ID).
#Примерно извикване: ./foo.sh a.csv b.csv
#Скриптът трябва да чете a.csv и на негова база да създава b.csv по следния начин:
#• някои редове във файла се различават само по колоната ID, и за тях казваме, че формират множество Ai
#• за всяко такова множество Ai да се оставя само един ред - този, с най-малка стойност на ID-то;
#• редовете, които не са членове в някое множество Ai се записват в изходния файл без промяна.

#!/bin/bash


if [[ $# -ne 2 ]]; then
        echo "Parameters should be 2"
        exit 1
fi

if [[ ! -f $1 ]]; then
        echo "First argument should be file"
        exit 2
fi
if [[ ! -f $2 ]]; then
    echo "Second argument should be file"
    exit 3
fi

#if [[ -e $2 ]]; then
#        echo "Second argument should be non existing file"
#        exit 3
#fi

#if ! [[  "$1" =~ ^[a-zA-Z-]+\.csv$ ]] || ! [[  "$2" =~ ^[a-zA-Z-]+\.csv$ ]]; then
#       echo "not csv files"
#       exit 4
#fi


while read line; do
        secondPart=$(echo $line| cut -d ',' -f 2-)

        if [[ $(cat $1 |egrep -c "^[0-9]+,$secondPart$") -eq 1 ]]; then
                echo $line >> $2
        else
                min_id_row=$(cat $1 | egrep "^[0-9]+,$secondPart$" | sort -n -t ',' -k1 | head -1)
                if !( cat $2 |egrep -q $min_id_row); then #ako go nqma go dobavi
                        echo $min_id_row >> $2
                fi
        fi
done< <(cat $1)
