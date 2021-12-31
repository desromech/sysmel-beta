#!/bin/bash
set -ex

cmake -S llvm-12.0.1.src -B llvm-build -G "Ninja" -DCMAKE_BUILD_TYPE=Release
&$CMake --build llvm-build

mkdir -p llvm-install
&$CMake --install llvm-build --prefix llvm-install
