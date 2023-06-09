#!/bin/bash
if [ $# -ne 1 ]; then
	echo "Usage: $0 <dirname>"
	exit 1
fi

if [[ ! -d $1 ]]; then
	echo "expecting dir"
	exit 2
fi

#find $1 -type f -printf "%p %n %i\n"

DIR=$1

T1=$(mktemp)
T2=$(mktemp)
T3=$(mktemp)

find ${DIR} -type f -printf "%p %n %i\n" > ${T1}
# name, link_count, inode

awk '{print $1}' ${T1} | xargs md5sum > ${T2}

join -1 1 -2 2 ${T1} ${T2} > ${T3}
# name, link_count, inode, md5

rm -- ${T1}
rm -- ${T2}

MS=$(awk '{print $4}' ${T3} | sort | uniq -c | awk '$1 > 1 { print $2 }')

for M in ${MS}; do
	T4=$(mktemp)
	awk -v m=${M} '$4 == m' ${T3} > ${T4}

	LINES=$(wc -l ${T4} | awk '{print $1}')
	ONES=$(awk '$2 == 1' ${T4} | wc -l)

	if [ $ONES -eq $LINES ]; then
		tail -n +2 ${T4}
	else
		sort -k 3,4 ${T4} | uniq -f 2
	fi

	rm -- ${T4}
done | awk '{ print $1 }'

rm -- ${T3}
