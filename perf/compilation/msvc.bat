@echo off
if not exist build mkdir build
cd build
cmake ../../temp/picobench-dev/tools -G "Visual Studio 14 2015 Win64
cmake --build . --config Release
cd ..
build\Release\picobench.exe --bfile=msvc.txt
