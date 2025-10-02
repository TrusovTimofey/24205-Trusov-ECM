#!/bin/bash

gcc main.c -o BubbleSort.out -lrt

step=10000

for (( i=1; i<=20; i++))
do
    N=$((i * step))
    echo -n "  N=$N: "
	./BubbleSort.out $N
done

echo "готово"
