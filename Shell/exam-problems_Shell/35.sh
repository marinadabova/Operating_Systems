#Зад. 35 2019-SE-03 За удобство приемаме, че разполагате със системен инструмент sha256sum, който приема аргументи имена на файлове като за всеки файл пресмята и извежда уникална хеш стойност,
#базирана на съдържанието на файла. Изходът от инструмента е текстови, по един ред за всеки подаден като аргумент файл, в следния формат:
#• хеш стойност с дължина точно 64 знака
#• два интервала
#• име на файл
#Примерна употреба и изход:
#$ sha256sum /var/log/syslog /var/log/user.log README.md
#b2ff8bd882a501f71a144b7c678e3a6bc6764ac48eb1876fb5d11aac11014b78 /var/log/syslog
#e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855 /var/log/user.log
#e4702d8044b7020af5129fc69d77115fd4306715bd678ba4bef518b2edf01fb9 README.md
#Напишете скрипт, който приема задължителен параметър име на директория (ДИР1). Някъде в директорията ДИР1 може да съществуват архивни файлове с имена NAME_report-TIMESTAMP.tgz, където:
#• NAME е низ, който не съдържа символ ’_’
#• TIMESTAMP е във формат Unix time (POSIX time/UNIX Epoch time)
#На всяко пускане на скрипта се обработват само новосъздадените или модифицираните по съдържание  спрямо предното пускане на скрипта архивни файлове от горния тип.
#За всеки такъв архивен файл се изпълнява следното:
#• ако архивният файл съдържа файл с име meow.txt, то този текстови файл да бъде записан под името /extracted/NAME_TIMESTAMP.txt, където NAME и TIMESTAMP са съответните стойности от името на архивния файл.

#!/bin/bash

if [[ $# -ne 1 ]]; then
        echo "One argumnet needed"
        exit 1
fi

if [[ ! -d $1 ]]; then
        echo "Argument should be dir"
        exit 2
fi

dir="extracted"
file="extracted/foo"
if [[ ! -e $file ]]; then
        touch $file
fi

f=$(mktemp)
cat $file >> $f
echo " " > $file

while read arch; do
        sum=$(sha256sum "$arch" | cut -d ' ' -f1)
        echo "$sum $arch" >> $file
        if grep -qxF "$sum_$arch" "$f"; then
                continue
        fi
        found=$(tar -tzf "$arch" | grep "/meow.txt$" | head -n 1)
        if [[ -n $found ]]; then
                tar -xOzf "$arch" "$found" > "$dir/$(echo "$(basename "$arch")" | / sed -E 's/([^_]+)_report_([0-9]+)\.tgz$/\1_\2.txt/')"
        fi

done< <(find "$1" -type f -regextype egrep -regex "/?[^_]+_report-[0-9]+\.tgz$")

rm "$f"
-----------------------------------------------------------------
#!/bin/bash

if [ "$#" -ne 1 ]; then
	echo 'Usage: extract /path/to/dir' 1>&2
	exit 1
fi
dirname="$1"

destdir='/extracted'
processed="$destdir/processed.txt"

if [[ ! -e $processed ]]; then
	touch $processed
fi

find $dirname -type f -regextype posix-extended -regex '^.*/[^_]+_report-[0-9]+\.tgz$' | while read fname; do 

	#echo $fname
	name=$(basename $fname)
	proc="$(sha256sum $fname | awk '{print $1}') $name"

	found="$(tar -tzf "$fname" | grep -Ee '/meow\.txt$' )" 
	destname="$( echo $name | sed -E 's/^([^_]+)_report-([0-9]+)\.tgz$/\1_\2/')"
	if grep -xq $proc $processed;then
		#already processed
		continue
	fi

	tar -xzOf $fname $found > $destdir/$destname

	echo $proc >> $processed 
done 
