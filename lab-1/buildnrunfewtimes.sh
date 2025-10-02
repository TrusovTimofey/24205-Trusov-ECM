#!/bin/bash

gcc main.c -o BubbleSort.out -lrt

for (( i=1; i<=100; i++))
do
	./BubbleSort.out 1
done

echo "готово"
