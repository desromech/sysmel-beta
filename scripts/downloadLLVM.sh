#!/bin/bash
set -ex

curl -L https://github.com/llvm/llvm-project/releases/download/llvmorg-12.0.1/llvm-12.0.1.src.tar.xz --output llvm-12.0.1.src.tar.xz
tar -xf llvm-12.0.1.src.tar.xz
