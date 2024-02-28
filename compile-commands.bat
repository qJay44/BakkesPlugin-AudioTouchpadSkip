@echo off
cls

if not exist build mkdir build
cd build
cmake -S ../ -B . -G"MinGW Makefiles" -D CMAKE_EXPORT_COMPILE_COMMANDS=ON
cd ..
