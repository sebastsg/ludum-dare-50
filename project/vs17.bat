@echo off
mkdir vs
cd vs
cmake -A Win32 -G "Visual Studio 17 2022" ../
pause
