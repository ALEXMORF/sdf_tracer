@echo off

IF EXIST ..\build\main.exe devenv ..\build\main.exe
IF NOT EXIST ..\build\main.exe echo NO EXECUTABLE FOUND

