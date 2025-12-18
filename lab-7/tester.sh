#!/bin/bash

for opt in "-O0" "-O3" "-Ofast"
do
    for flag in "" "-march=native" "-funroll-loops" "-ffast-math" "-march=native -funroll-loops -ffast-math"
    do
        echo "$opt $flag:"
        gcc $opt $flag NoVec.c -o out
        ./out
    done
    echo ""
done
echo "готово"

