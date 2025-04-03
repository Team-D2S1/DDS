@echo off
echo [ Build Start : MainServer.cpp → MainServer.exe ]

REM Change Drive
D:

REM Change Directory
cd D:\Project\DDS\DDSServer

REM Build with g++
g++ -o MainServer.exe MainServer.cpp -lws2_32

IF %ERRORLEVEL% NEQ 0 (
    echo [ Error ] Build Failed!
) ELSE (
    echo [ Success ] Success to Build MainServer.exe
)

pause