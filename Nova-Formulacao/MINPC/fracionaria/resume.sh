#!/bin/bash

if [ $# -lt 2 ] || [ $# -gt 2 ]; then
	echo "Usage: resume numVertices numInst"
	exit 1
fi

let N=$1
let T=$2
let numInst=0

while [ $numInst -lt $T ]; do
	printf "instance %d: " $numInst 
	./digress < "./instancias/$N/out$numInst" #&
	#let pid=$!
	#echo $pid
	#let cnt=0
	#while [ $cnt -lt 1200 ]; do
	#	sleep 1
	#	if [ ! -n "$(ps -p $pid -o pid=)" ]; then  
	#		break
	#	fi
	#	let cnt=cnt+1
	#done
	#if [ -n "$(ps -p $pid -o pid=)" ]; then
	#	echo instance $numInst not terminated in 1200 seconds
	#	kill $pid
	#else
	#fi
	let numInst=numInst+1
done


