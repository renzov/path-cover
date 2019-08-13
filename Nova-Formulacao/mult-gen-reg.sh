#!/bin/bash

readonly NUMINST=100

if [ $# -ne 6 ]; then
	echo "Usage: mult-gen-reg.sh minN maxN stepN minK maxK stepK"
	exit 1
fi

let N=$1

while [ $N -le $2 ]; do
	# Process for this number of instances	
	let P=$4
	while [ $P -le $5 ]; do
		# Call the generator script
		python3 gen2-regular.py $NUMINST $N $P	
		echo "Processed N=$N K=$P"		
		# Check if we processed all the values of P
		if [ $P -eq $5 ]; then
			break
		fi
		let NP=P+$6
		# Check if we need to correct the value of P
		if [ $NP -gt $5 ]; then
			let P=$5
		else 
			let P=NP
		fi
	done
	# Check if we processed all the values of N
	if [ $N -eq $2 ]; then
		break
	fi
	let T=N+$3
	# Check if we need to correct the value of N
	if [ $T -gt $2 ]; then
		let N=$2
	else
		let N=T
	fi
done

