#!/bin/bash

if [[ "$(whoami)" != "root" ]]; then
        echo "You do not have permissions"
        exit 1
fi

if (env | egrep -q "CTRSLOTS" ); then
	slots=$(env | egrep "CTRSLOTS" | cut -d'=' -f2)
else
	slots="0"
fi

if [[ -z ${CTRSLOTS} ]]; then
	CTRSLOTS=0
fi

if [[ ${1} == "autoconf ]]; then
	echo "yes"
else
	for num in ${slots}; do
		if [[ ${1} == "config" ]]; then
			echo "graph_title SSA drive temperatures"
			echo "graph_vlabel Celsius"
			echo "graph_category sensors"
			echo "graph_info This graph shows SSA drive temp"
			
			ssacli ctrl slot=$num pd all show detail | awk '$1=="Smart" {model=$3;slot=$6} \ 
									$1 == "Array" {name=$2} \
									$1 == "Unassigned" {name="UN"} \
									$1 == "physicaldrive" {drive=$2;print
 "SSA" slot model name drive".label SSA"slot,model,name,drive "\n" "SSA" slot model name drive".type GAUGE" }' | 
sed 's/://;s/://' 
		elif [[ $# -eq 0 ]]; then
			ssacli ctrl slot=$num pd all show detail | awk "$1=="Smart" {model=$3;slot=$6}  
				$1 == "Unassigned" {name=$2} 
				$1 == "physicaldrive" {drive=$2}
				$1 == "Current"	{temp=$4;print "SSA" slot model name drive ".value " temp}' |
sed 's/://;s/://'
		fi
	done
fi
