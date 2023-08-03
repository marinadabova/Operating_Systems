# Зад. 53 2022-SE-02 Дефинирана е система за бекъпване на сървъри, която държи направените архиви в главна директория (която ще наричаме fubar ), в която има четири под-директории за различни класове бекъпи:
# • 0 – съдържа годишни бекъпи
# • 1 – съдържа месечни бекъпи
# • 2 – съдържа седмични бекъпи
# • 3 – съдържа дневни бекъпи
# Всяка директория съдържа архивни файлове с имена във формат hostname-area-YYYYMMDD.tar.xz, където:
# • hostname е името на някаква машина, която е бекъпвана;
# • area е типът бекъп за съответната машина;
# • YYYYMMDD е датата, на която е направен бекъпа;
# • никое от горните полета не може да съдържа тире или точка;
# • някои от файловете могат да са symlink-ове.
# Примерни имена на файлове:
# astero-etc-20220323.tar.xz stratios-etc-20220428.tar.xz nestor-db-20220404.tar.xz
# gila-srv-20220405.tar.xz catalyst-var-20220406.tar.xz drake-home-20220404.tar.xz
# dominix-var-20220404.tar.xz
# Комбинацията от hostname и area дефинира уникален обект за бекъпване. Всички архивни файлове са пълноценни бекъпи на даден обект и са достатъчни за неговото възстановяване при нужда
#(заб. извън обхвата на задачата).
# Ако даден файл е symlink, то той може да е валиден или счупен. Symlink-овете са създадени за удобство и не ги считаме за пълноценни бекъпи.
# Политиката ни за бекъп казва, че за да имаме валиден бекъп на даден обект, за него трябва да имаме минимум 1 годишен, 2 месечни, 3 седмични и 4 дневни пълноценни бекъпа.
# Важност:
# • обектите са равни по важност помежду си;
# • важността на класовете бекъпи е във възходящ ред по горния списък, т.е. при равни други условия дневните бекъпи са по-ценни от седмичните и т.н.;
# • в рамките на един клас бекъпи по-новите бекъпи са по-важни от по-старите.
# Напишете shell скрипт, който приема два два задължителни позиционни аргумента – име на директория и число в интервала [1, 99]. Примерно извикване: ./foo.sh ./bar/ 30 където:
# • директорията представлява главна директория (fubar ) на описаната система за бекпъване;
# • числото дефинира колко процента е максималното допустимо използвано място на файловата система, в която се намира подадената директория.
# За удобство приемаме, че директорията fubar и всички обекти в нея се намират в една и съща файлова система.
# Упътване: Командата df извикана с аргумент име на файл/директория връща информация за файловата система, в която той се намира. Пример:
# $ df ./README.md
# Filesystem 1K-blocks Used Available Use% Mounted on
# /dev/mapper/o7-hm 100663296 61735732 37288764 63% /home
# Скриптът трябва да изтрива минимален брой пълноценни архивни файлове така, че:
# • всеки обект да има валиден бекъп;
# • обръща внимание на описаните по-горе важности;
# • процентите използвано място върху файловата система да е не повече от подаденият параметър
#(a ако това е невъзможно, скриптът да освободи колкото може повече място, без да нарушава валидностите на бекъпите на обектите);
# • не е допустимо след работата на скрипта да останат счупени symlink–ове.

#!/bin/bash

if [[ $# -ne 2 ]]; then
        echo "Arguments should be two"
        exit 1
fi
if [[ ! -d $1 ]]; then
        echo "First argument should be dir"
        exit 2
fi
if [[ ! $2 =~ ^[1-9]+$ ]]; then
        echo "Second argument should be number"
        exit 3
fi
if [[ $2 -lt 1 ]] || [[ $2 -gt 99  ]]; then
        echo "not a num between 1 and 99"
        exit 4
fi
for link in $(find -L $1 -type l); do
        #rm -r $link
        echo "remove broken symlinks"

done

for file in $(find $1 -type f -regextype egrep -regex "^[a-zA-Z0-9]+\-[a-zA-Z0-9]+\-[0-9]{8}\.tar\.xz$"); do
        dfPercent=$(df $file | tail -1  |awk '{print $5}' |tr -d "%")
        if [[ $dfPercent -gt $2 ]]; then
                #rm $file
        fi

        count_yearly=$(find "$1/0" -type f -name "${file}" | wc -l)
        count_monthly=$(find "$1/1" -type f -name "${file}" | wc -l)
        count_weekly=$(find "$1/2" -type f -name "${file}" | wc -l)
        count_daily=$(find "$1/3" -type f -name "${file}" | wc -l)

        if [[ $count_yearly -eq 1 || $count_yearly -gt 1 ]] && [[ $count_monthly -eq 2 || $count_montly -gt 2 ]]
        && [[ $count_weekly -eq 3 || $count_weekly -gt 3 ]] && [[ $count_daily -eq 4 || $count_daily -gt 4 ]];
                echo "this ${file} is valid backup"
        else
                #rm "${file}"
        fi

done
---------------------------------------------
#second sol
#!/bin/bash

if [[ ${#} -ne 2 ]] ; then
        echo "Expected 2 arguments"
        exit 1
fi
DIR=${1}
if [[ ! -d ${DIR} ]] ; then
        echo "First argument should be a directory"
        exit 1
fi
NUMBER=${2}
if [[ ${NUMBER} -lt 1 ]] || [[ ${NUMBER} -gt 99 ]] ; then
        echo "Number should be in interval [1,99]"
        exit 1
fi
objects=$(find "${DIR}" -mindepth 1 -type f -printf "%f\n" | awk -F '{print $1 "-" $2}' | sort -u)

for object in $(echo "${objects}") ; do
        result=$(find "${DIR}" -type f -name "${object}*")
        yearBackup=$(echo "${result}" | awk -F '/' '{if ($(NF-1)=="0") {print $0}')
        monthBackup=$(echo "${result}" | awk -F '/' '{if ($(NF-1)=="1") {print $0}')
        weeklyBackup=$(echo "${result}" | awk -F '/' '{if ($(NF-1)=="2") {print $0}')
        dailyBackup=$(echo "${result}" | awk -F '/' '{if ($(NF-1)=="3") {print $0}')
        yearCount=$(echo "${yearBackup}" | wc -l)
        monthCount=$(echo "${monthBackup}" | wc -l)
        weeklyBackup=$(echo "${weeklyBackup}" | wc -l)
        dailyBackup=$(echo "${dailyBackup}" | wc -l)
        if [[ ${yearCount} -gt 1 ]] ; then
                toDelete=$(( yearCount - 1 ))
                for file in $(find "${DIR}/0" -mindepth 1 -name "${object}*" -type f | sort | head -n ${toDelete}) ; do
                        rm ${file}
                        usage=$(df ${DIR} | tail -n1 |awk '{print $5}' | sed -E 's/%$//')
                        if [[ ${usage} -le ${NUMBER} ]] ; then
                                exit 0
                        fi
                done
        elif [[ ${monthCount} -gt 2 ]] ; then
                toDelete=$(( monthCount - 2 )) ; then
                for file in $(find "${DIR}/1" -mindepth 1 -name "${object}*" -type f | sort | head -n ${toDelete}) ; do
                        rm ${file}
                        usage=$(df ${DIR} | tail -n1 | awk '{print $5}' | sed -E 's/%$//')
                        if [[ ${usage} - le ${NUMBER} ]] ; then
                                exit 0
                        fi
                done
        elif [[ ${weeklyCount} -gt 3 ]] ; then
                toDelete=$(( weeklyCount - 3 ))
                for file in $(find "${DIR}/2" -mindepth 1 -name "${object}*" -type f | sort | head -n ${toDelete}) ; do
                        rm ${file}
                        usage=$(df ${DIR} | tail -n1 |awk '{print $5}' | sed -E 's/%$//')
                        if [[ ${usage} -le ${NUMBER} ]] ; then
                                exit 0
                        fi
                done
        elif [[ ${dailyCount} -gt 4 ]] ; then
                toDelete=$(( dailyCount - 4 )) ; then
                for file in $(find "${DIR}/3" -mindepth 1 -name "${object}*" -type f | sort | head -n ${toDelete}) ; do
                        rm ${file}
                        usage=$(df ${DIR} | tail -n1 | awk '{print $5}' | sed -E 's/%$//')
                        if [[ ${usage} - le ${NUMBER} ]] ; then
                                exit 0
                        fi
                done
        fi
done
