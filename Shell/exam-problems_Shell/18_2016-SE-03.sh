#В текущата директория има само обикновени файлове (без директории). Да се
#напише bash script, който приема 2 позиционни параметъра – числа, който мести файловете от
#текущата директория към нови директории (a, b и c, които трябва да бъдат създадени), като
#определен файл се мести към директория ’a’, само ако той има по-малко редове от първи позиционен
#параметър, мести към директория ’b’, ако редове са между първи и втори позиционен параметър
#и в ’c’ в останалите случаи.

#ot ypr
#!/bin/bash

if [ $# -ne 2 ]; then
        echo "Wrong number of arg"
        exit 1
fi

if ! echo $1 | grep -q "^[1-9][0-9]*$" ; then
        echo "We need number"
        exit 1
fi

if ! echo $2 | grep -q "^[1-9][0-9]*$" ; then
        echo "We need number"
        exit 1
fi

if [ $1 -gt $2 ]; then
        echo "First arg should be lesser than second arg"
        exit 1
fi

mkdir -p {a,b,c}

checkDirEmpty=$(find . -maxdepth 0 -empty)
if [[ ! -z checkDirEmpty ]]; then
    echo "Dir is empty"
    exit 3
fi

for file in $(find ./test -type f ); do
        numLines=$(cat $file | wc -l)
        if [ $numLines -lt $1 ]; then

                #cp $file a
                mv $file a
        elif [ $numLines -ge $1 -a $numLines -lt $2 ]; then
                #cp $file b
                mv $file b
        else
                #cp $file c
                mv $file c
        fi

done

########################################
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

