#!/bin/bash
if [[ ${#} -ne 2 ]];then
	echo "Parameters should be 2"
	exit 1
fi

if [[ !( ${1} =~ ^[0-9]+$ && ${2} =~ ^[0-9]+$ ) ]]; then
	echo "Parameters should be numbers"
	exit 2
fi

mkdir -p {a,b,c}

checkDirEmpty=$(find . -maxdepth 0 -empty)
if [[ ! -z checkDirEmpty ]]; then
    echo "Dir is empty"
    exit 3
fi

while read filename ;do
	count_lines=$(cat $filename| wc -l) 
	if [[ $count_lines -lt $1 ]]; then
		mv $filename a
	#elif [[ $count_lines -lt $2 ]]; then
	 elif [[ $count_lines -ge $1 && $count_lines -lt $2 ]]; then
		 mv $filename b
	else
		mv $filename c
	fi
done < <(find . -type f 2>/dev/null)

