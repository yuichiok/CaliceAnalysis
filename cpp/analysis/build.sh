#!/bin/bash

if [[ -d "build" ]]
then
    rm -rf build/
    echo "rm -rf build/"
fi

if [[ -f "main.exe" ]]
then
    rm -rf main.exe
    echo "rm -rf main.exe"
fi

mkdir build
cd build
cmake ..
# make VERBOSE=1
make
cd -