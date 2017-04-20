#! /bin/bash

for VAR in $(ls);do
	if [ "${VAR##*.}" = "c" ];then
	echo $VAR
	echo ${VAR%.*}
	gcc $VAR -o ${VAR%.*}.o -Wall -g -lpthread
	fi
done
