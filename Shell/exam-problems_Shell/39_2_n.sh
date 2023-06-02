#!/bin/bash

if [ $# -ne 2 ];then
        echo "We need 2 arguments" >&2;
        exit 1;
fi

if [ ! -d $1 ];then
        echo "We need dir" >&2;
        exit 2;
fi

if [ -d $2 ];then
        echo "Dir exist already" >&2;
        exit 3;
fi

mkdir $2;
mkdir $2/images;

find $1 -type f -printf "%f\n" | egrep "^.+\.jpg" | while read line;do
        base=$(basename -s .jpg "$line");

        title=$(echo $base | tr -s ' ' | sed 's/\([^\(\)]\)//g');

        #principno kato se zapazi v promenliva, premahva space otpred i otzad
#       toCheck=$(echo $title | cut -c 1);
#       if [ $toCheck == ' ' ];then
#               title=$(echo $title | cut -c 2-);
#       fi

        album="";

        if [ $(echo $base | grep -c "(") -eq 0 ];then
                album="misc";
        else
                album=$(echo $base | tr -s ' ' | rev | cut -d ')' -f 2 | cut -d '(' -f 1 | rev);

        fi
#       toCheck=$(echo $album | cut -c 1);
#       if [ $toCheck == ' '];then
#               album=$(echo $album | cut -c 2-);
#       fi

        date=$(find $1 -name "${line}" -printf "%TY-%Tm-%Td");
        path=$(find $1 -name "${line}" -printf "%p\n");

        hesh=$(sha256sum "$path" | cut -c 1-16);

        cp "$path" "$2/images/$hesh.jpg";

        mkdir -p "$2/by-date/$date/by-album/$album/by-title/";
        ln -s "$path" "$2/by-date/$date/by-album/$album/by-title/$title.jpg";

done

exit 0;
