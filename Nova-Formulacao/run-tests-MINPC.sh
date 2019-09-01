#!/bin/bash
BASE_DIR=~/path-cover/Nova-Formulacao/MINPC/inteira
# Change to the directory containing the testdata
if [ ! -d "$BASE_DIR/output" ]; then
	mkdir "$BASE_DIR/output"
fi

cd ./instancias/

let cnt=0

for file in *; do
	# For each file there we check if it is a directory
	if [ -d $file ]; then
		# Check for output directory
		if [ ! -d "$BASE_DIR/output/$file" ]; then
			mkdir "$BASE_DIR/output/$file"
		fi
		# In that case, we enter this directory
		cd $file
		# create directory	
		echo "Processing dataset: $file"
		# We process each input file
		for inst in "in"*; do
			# We execute the program
			if [ -f "$BASE_DIR/output/$file/$inst" ]; then
				echo "Input: $inst already processed"
			else
				$BASE_DIR/main < $inst >  "$BASE_DIR/output/$file/$inst"
				echo "Input: $inst processed"
				let cnt=cnt+1
			fi
			# Check if we have processed 10 instances
			if [ $cnt -ge 10 ]; then 
				# sleep 1m
				let cnt=0
			fi
		done
		# Remenber to go back for next iteration
		cd ..
	fi
done
cd ..
