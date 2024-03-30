#Зад. 52 2022-SE-01 Съвременните компютри могат да влизат в различни режими за енергоспестяване (suspend) и излизат от този режим (wake-up) при настъпването на определени събития.
#Linux kernel предоставя специален виртуален файл /proc/acpi/wakeup, чрез който може да се проверява илипроменя настройката за “събуждане” при настъпване на различни събития,
#в общия случай - приактивност на някое устройство. Тъй като този файл е интерфейс към ядрото, “четенето” от файла и “писането” във файла изглеждат различно.
#За улеснение на задачата ще ползваме опростено описание на този файл. Под whitespace разбираме произволна комбинация от табове и интервали.
#При четене от файла изходът представлява четири колони, разделени с whitespace, в полетата не може да има whitespace;
#първият ред е header на съответната колона. Примерно съдържание на файла:
#Device S-state Status Sysfs node
#GLAN S4 *enabled pci:0000:00:1f.6
#XHC S3 *enabled pci:0000:00:14.0
#XDCI S4 *disabled
#LID S4 *enabled platform:PNP0C0D:00
#HDAS S4 *disabled pci:0000:00:1f.3
#RP01 S4 *enabled pci:0000:00:1c.0
#където:
#• първата колона дава уникално име на устройство, което може да събуди машината, като името е ограничено до четири знака главни латински букви и цифри;
#• третата колона описва дали това устройство може да събуди машината. Възможните стойности са enabled/disabled, предхождани от звездичка;
#• втората и четвъртата колона ги игнорираме в рамките на задачата.
#При записване на име на устройство във файла /proc/acpi/wakeup, неговото състояние се променя от disabled на enabled или обратно.
#Например, ако файлът изглежда както примера по-горе, при запис на XHC в него, третият ред ще се промени на:
#XHC S3 *disabled pci:0000:00:14.0
#При запис на HDAS, шестият ред ще се промени на:
#HDAS S4 *enabled pci:0000:00:1f.3
#Дефиниран е формат на конфигурационен файл, който описва желан комплект от настройки на wakeup събития. Примерен файл:
## comment bar
#GLAN disabled
#LID enabled # comment foo
#където:
#• знакът диез (#) е знак за коментар до края на реда;
#• редовете би трябвало да са комбинация от име на устройство и желаното състояние на настройката за събуждане при събития от това устройство, разделени с whitespace.
#Напишете скрипт, който при подаден първи параметър име на конфигурационен файл в горния формат осигурява исканите настройки за събуждане.
#Ако във файла има ред за устройство, което не съществува, скриптът да извежда предупреждение.
#Обърнете внимание на обработката за грешки и съобщенията към потребителя – искаме скриптът да бъде удобен и валиден инструмент.

#!/bin/bash

if [[ $# -ne 1 ]]; then
        echo "One argument needed"
        exit 1
fi
if [[ ! -f "${1}" ]]; then
        echo "First argument should be file"
        exit 2
fi

file="wakeup.txt" #for testing
#file='/proc/acpi/wakeup'

while read line; do
        name=$(echo $line| awk '{print $1}')
        status=$(echo $line| awk '{print $2}')

        if ( echo "$name" | egrep -q "^[A-Z0-9]{1,4}$"); then
                if !(  cat $file |egrep -q "^$name[[:space:]]+.*$" ); then
                        echo "Do not exist"
                else
                        orig_status=$(cat $file| egrep "^$name[[:space:]]+.*$" |awk '{print $3}'| tr '*' ' ')
                        if [[ $orig_status != $status ]]; then
                                sed -i -E "s/^($name[[:space:]].*)[[:space:]]\*(enabled|disabled)/\1 *$status/" "$file"
                                #sed -i -E "s/^($name[[:space:]].*)[[:space:]]\*$orig_status/\1 *$status/" "$file"

                        fi
                fi
        fi
done< <(cat $1| egrep -v "^#.*$")
###########3
#!/bin/bash

if [ "${#}" -ne 1 ]; then
        echo "One arg needed"
        exit 1
fi

if [ ! -f "${1}" ]; then
        echo "Argument should be file"
        exit 2
fi

while read DEVICE STATE; do
        if ! grep -q "${DEVICE}" /proc/acpi/wakeup ; then #sravnqvame error code-a
                echo "WARN: ${DEVICE} not in wakeup" >&2
                continue
        fi
        CURSTATE=$(cat /proc/acpi/wakeup |grep ${DEVICE}  | sed -E 's/^.*\*(.*) .*$/\1/')
        if [ "${CURSTATE}" != "${STATE}" ]; then
                echo "$DEVICE" >> "/proc/acpi/wakeup"

        fi
done< <(cat "${1}" | cut -d '#' -f 1 | grep -v "^ *$")
