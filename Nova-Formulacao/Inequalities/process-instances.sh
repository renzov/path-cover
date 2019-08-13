#!/bin/bash
BASE_DIR=~/Dropbox/RenzoYoshiko-Doutorado/Implementacao/Nova-Formulacao/Inequalities
# Change to the directory containing the testdata

cd ./instances/

for file in *.in; do
	# For each file there we check if it is a directory
	if [ -f $file ]; then
		# We process each input file
		f=$(basename $file .in)
		if [ -f "$BASE_DIR/complete-description/complete-$f.out" ]; then
			echo "Input: $file already processed"
		else
			echo "Processing instance: $file"
			$BASE_DIR/get_inequalities-ubuntu < $file > "$BASE_DIR/ineq"
			head -2 "$BASE_DIR/ineq" > "$BASE_DIR/vertex-enumeration/vertex-enum-$file"
			panda "$BASE_DIR/ineq" > "$BASE_DIR/vert"
			$BASE_DIR/transform-ubuntu < "$BASE_DIR/vert" >> "$BASE_DIR/vertex-enumeration/vertex-enum-$file"
			panda "$BASE_DIR/vertex-enumeration/vertex-enum-$file" > "$BASE_DIR/complete-description/complete-$f.out"
			echo "Input: $file processed"
		fi
	fi
done
rm "$BASE_DIR/ineq"
rm "$BASE_DIR/vert"
cd ..
