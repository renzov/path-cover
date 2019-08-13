#!/bin/bash

if [ $# -lt 3 ] || [ $# -gt 3 ]; then
	echo "Usage: tester numVertices density numInst"
	exit 1
fi

let N=$1
let d=$2
let T=$3
let numInst=0

while [ $numInst -lt $T ]; do
	./main < "./instancias/${N}_${d}/in$numInst" > "./instancias/${N}_${d}/out$numInst" #&
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
	echo instance $numInst processed.
	#fi
	let numInst=numInst+1
done

