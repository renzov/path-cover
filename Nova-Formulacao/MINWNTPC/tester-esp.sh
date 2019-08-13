#!/bin/bash

if [ $# -lt 1 ] || [ $# -gt 1 ]; then
	echo "Usage: tester-est numInst"
	exit 1
fi

let T=$1
let numInst=0

while [ $numInst -lt $T ]; do
	./minwpc < "./instancias/especial/in$numInst"
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

