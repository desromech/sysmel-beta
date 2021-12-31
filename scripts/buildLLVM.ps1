$ErrorActionPreference = "Stop"
$CMake = 'C:\Program Files\CMake\bin\cmake.exe'

&$CMake -S llvm-12.0.1.src -B llvm-build -G "Visual Studio 16 2019" -A x64  -Thost=x64
&$CMake --build llvm-build --config Release

mkdir llvm-install
&$CMake --install llvm-build --config Release --prefix llvm-install
