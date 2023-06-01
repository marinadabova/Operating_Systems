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
