@echo off
cd /d "%~dp0"
where g++ >nul 2>nul
if errorlevel 1 (
    echo Add MinGW-w64 or w64devkit bin folder to PATH first.
    pause
    exit /b 1
)
if not exist build mkdir build
g++ -std=c++11 -Wall -Wextra -pedantic src\main.cpp src\Application.cpp src\Console.cpp src\Line.cpp src\Symbol.cpp -o build\matrix.exe -luser32 -static
if errorlevel 1 exit /b 1
echo Done: build\matrix.exe
