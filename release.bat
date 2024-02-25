@echo off
cls

if not exist build mkdir build
cd build
cmake -S .. -B minGW\ -G"MinGW Makefiles" -D CMAKE_EXPORT_COMPILE_COMMANDS=ON
if not exist compile_commands.json copy minGW\compile_commands.json .
cmake.exe -S .. -B . -G"Visual Studio 17 2022" -D CMAKE_COMPILE_COMMANDS=ON
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.com" MyProject.sln /Rebuild "Release|x64" /Out log.txt
Release\MyProject.exe
cd ..
