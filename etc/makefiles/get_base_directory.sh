#!/bin/bash
dir2search=$1
max_tries=$2
if [ -z "$max_tries" ]; then
    max_tries=3
fi
if [ -z "$3" ]; then
    root_dir_stop="/"
else
    root_dir_stop=$(dirname $3)
fi
iteration_count_index=0
#echo "dir2search ${dir2search}  max_tries = ${max_tries} root_dir_stop ${root_dir_stop} iteration_count_index ${iteration_count_index} starting from $(pwd)"
while [ -z $(find -L . -maxdepth 2 -name ${dir2search}) ] && [ $(pwd) != ${root_dir_stop} ] && [  ${iteration_count_index} -lt ${max_tries}  ]; do 
#    echo didnt find ${dir2search} at $(pwd) iteration_count_index ${iteration_count_index}
    cd ..
    iteration_count_index=$((iteration_count_index+1))
done

if [ ${iteration_count_index} -eq ${max_tries} ]; then
    echo "error"
    exit 1
else
    item2find=$(find -L . -maxdepth 2 -name ${dir2search} | echo "$(pwd)`sed 's/\.//'`" | xargs dirname)
    if [ -z "$item2find" ]; then
        exit 1
    fi
    echo ${item2find}
    exit 0
fi
