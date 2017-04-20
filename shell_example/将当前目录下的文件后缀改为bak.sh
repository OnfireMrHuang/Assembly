# !/bin/bash

ls
for VAR in $(ls);do
	echo $VAR
	file=${VAR%.*}
	mv $VAR ${file}.bak
done
