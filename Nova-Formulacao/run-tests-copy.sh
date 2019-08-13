#!/bin/bash
BASE_DIR=~/Dropbox/RenzoYoshiko-Doutorado/Implementacao/Nova-Formulacao/MINPC/inteira
# Change to the directory containing the testdata
if [ ! -d "$BASE_DIR/output" ]; then
	mkdir "$BASE_DIR/output"
fi

cd ./instancias/

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
			fi
		done
		# Remenber to go back for next iteration
		cd ..
	fi
done
cd ..
