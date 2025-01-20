@echo off
set path=..\..\mingw\gcc-9.2.0\bin
mingw32-make.exe -f ECNavigator_Makefile_64bit.mingw
rename ECNavigator.exe ECNavigator_debug.exe
copy ECNavigator_debug.exe ..\ECNavigator.exe
del  ECNavigator_debug.exe

if not exist Driver.o goto noDriver
copy Driver.o ols\Driver.o
del  Driver.o
:noDriver

if not exist OlsApi.o goto noOlsApi
copy OlsApi.o ols\OlsApi.o
del  OlsApi.o
:noOlsApi

if not exist OlsDll.o goto noOlsDll
copy OlsDll.o ols\OlsDll.o
del  OlsDll.o
:noOlsDll

if not exist stdafx.o goto nostdafx
copy stdafx.o ols\stdafx.o
del  stdafx.o
:nostdafx

pause