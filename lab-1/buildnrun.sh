#!/bin/bash

gcc main.c -o BubbleSort -lrt

step=10000

for (( i=1; i<=20; i++))
do
    N=$((i * step))
    echo -n "  N=$N: "
	./BubbleSort $N
done

echo "готово"
