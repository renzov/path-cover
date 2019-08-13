#!/bin/bash
BASE_DIR=~/Dropbox/RenzoYoshiko-Doutorado/Implementacao/Nova-Formulacao
OUTPUT_DIR=~/Dropbox/RenzoYoshiko-Doutorado/Implementacao/Nova-Formulacao/MINPC/inteira/output
INPUT_DIR=~/Dropbox/RenzoYoshiko-Doutorado/Implementacao/Nova-Formulacao/instancias
# Change to the directory containing the testdata
cd $OUTPUT_DIR

for file in *; do
	# For each file there we check if it is a directory
	if [ -d $file ]; then
		# we enter this directory
		cd $file
		# create directory	
		echo "Checking dataset: $file"
		# We check each output file
		for inst in "in"*; do
			# We execute the checker
			$BASE_DIR/checker2 "$INPUT_DIR/$file/$inst" $inst $inst 
			echo "Output: $inst processed"
		done
		# Remenber to go back for next iteration
		cd ..
	fi
done
cd ..
