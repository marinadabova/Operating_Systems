#!/bin/bash

if [[ ${#} -ne 1 ]]; then 
	echo "It should be one parameter"
	exit 1
fi

if [[ ! -d ${1} ]]; then
	echo "Parameter should be dir"
	exit 2
fi

find -L ${1} -type l
