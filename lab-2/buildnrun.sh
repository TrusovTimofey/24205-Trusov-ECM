#!/bin/bash

for var in "-O0" "-O1" "-O2" "-O3" "-Os" "-Ofast" "-Og"
do
	echo "$var:"
	gcc $var main.c -o BubbleSort -lrt
	for n in 100000 150000 200000
	do
		echo -n "  N=$n: "
		./BubbleSort $n
	done
	echo ""
done
echo "готово"
