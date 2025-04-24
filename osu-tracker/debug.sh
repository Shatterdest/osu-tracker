#!/bin/bash

if [ "$1" == "configure" ]; then
    echo "Configuring the CMake project..."
    cmake --preset x86-linux-debug
elif [ "$1" == "build" ]; then
    echo "Building the CMake project..."
    cmake --build out/build/x86-linux-debug
else
    echo "Usage: $0 {configure|build}"
    exit 1
fi