#!/usr/bin/env bash

num=${1:?"No argument entered"}
sum=0
if [[ "$num" =~ ^[0-9]+$ ]]
then
	//logic here
	for (( i=0; i < ${#num}; i++ ))
	do
		sum=$(( sum + ${num:i:1} ))
	done
	echo $sum
else
	echo "Not a number"
fi

	
