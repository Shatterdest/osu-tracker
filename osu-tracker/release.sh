#!/bin/bash

if [ "$1" == "configure" ]; then
    echo "Cleaning up previous CMake configuration..."
    rm -rf out/build/x86-release-linux
    echo "Configuring the CMake project..."
    cmake --preset x86-release-linux
elif [ "$1" == "build" ]; then
    echo "Building the CMake project..."
    cmake --build out/build/x86-release-linux
else
    echo "Usage: $0 {configure|build}"
    exit 1
fi