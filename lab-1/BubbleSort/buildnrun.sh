#!/bin/bash

rm -r build
mkdir build
cd build 
cmake ..
make
chmod +x build/BubbleSort
./BubbleSort
echo "готово"
