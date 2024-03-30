#Зад. 44 2021-SE-03 Напишете shell скрипт, който приема два позиционни параметъра – имена на файлове. Примерно извикване: $ ./foo.sh input.bin output.h
#Файлът input.bin е двоичен файл с елементи uint16_t числа, създаден на little-endian машина.
#Вашият скрипт трябва да генерира C хедър файл, дефиниращ масив с име arr, който:
#• съдържа всички елементи от входния файл;
#• няма указана големина;
#• не позволява промяна на данните.
#Генерираният хедър файл трябва да:
#• съдържа и uint32_t променлива arrN, която указва големината на масива;
#• бъде валиден и да може да се #include-ва без проблеми от C файлове, очакващи да “виждат” arr и arrN.
#За да е валиден един входен файл, той трябва да съдържа не повече от 524288 елемента.
#За справка, dump на съдържанието на примерен input.bin:
#00000000: 5555 5655 5955 5a55 6555 6655 6955 6a55 UUVUYUZUeUfUiUjU
#00000010: 9555 9655 9955 9a55 a555 a655 a955 aa55 .U.U.U.U.U.U.U.U

#!/bin/bash

if [[ $# -ne 2 ]]; then
        echo "Arguments should be two"
        exit 1
fi

if [[ ! -f $1 ]] || [[ ! -f $2 ]]; then
        echo "Arguments should be files"
        exit 2
fi
size=$(stat -c "%s" $1)

if [[ $(echo "$size % 2" | bc) -ne 0 ]]; then
        echo "File is not binary"
        exit 3
fi

echo -e "#include <stdio.h>\n" >> $2
arrN=$(echo "$size /2" | bc)

if [[ $arrN -gt 524288 ]]; then
        echo "File is not valid"
        exit 4
fi

echo "const uint32_t arrN = $arrN;" >> $2
echo "const uint16_t arr [] = {" >> $2

for num in $(xxd $1 | cut -c 10-49);do
        num=$(echo "$num" | sed -E "s/^([0-9]{2})([0-9]{2})$/\2\1/g")
        new="0x$num"
        echo "$new, " >> $2
done

sed -i -E "s/(^.+),([^,]+)$/\1\2/g" $2
echo -n " }" >> $2
###########
#!/bin/bash

if [ $# -ne 2 ];then
        echo "We need 2 arguments";
        exit 1;
fi

if [ ! -f $1 ];then
        echo "First argument must be file";
        exit 2;
fi

if [ ! -f $2 ];then
        echo "The second argument must be file";
        exit 3;
fi

echo "#include <stdint.h>" > $2;

size=$(stat -c "%s" $1);
elements=$(echo "$size/2" | bc);
toCheck=$(echo "$elements*2" | bc);

if [ $toCheck -ne $size ];then
        echo "Wrong format file";
        exit 4;
fi

if [ $elements -gt 524288 ];then
        echo "We have too many elements";
        exit 5;
fi

echo "uint32_t arrN = $elements;" >> $2;
echo "uint16_t arr[] = {" >> $2;

xxd $1 | cut -c 11-50 | sed -E 's/([0-9a-f]{2})([0-9a-f]{2})/\2\1/g' | tr ' ' '\n' | tr -s '\n' | sed -E 's/^(.{4})$/0x\1,/g' >> $2;

echo "}" >> $2;

exit 0;
