#!/bin/bash

if [ $# -lt 1 ] || [ $# -gt 1 ]; then
	echo "Usage: tester-bip.sh numInst"
	exit 1
fi

let N=$1
let id=1
let numInst=0

while [ $numInst -lt $N ]; do
	./minwpc < "./instancias/bipartite/in$id" > "./instancias/bipartite/out$id" #&
	echo instance $id processed.
	let numInst=numInst+1
	let id=id+1
done

