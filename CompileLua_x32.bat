@echo off
set EXEFOLDER="%1"
if "%1" EQU "" (
	set EXEFOLDER="%cd%\output\Win32"
)
cd %EXEFOLDER%
..\..\Tools\CommonCompiler\build\CommonCompiler_32.exe /config ..\..\StarWay\CommonBuilder\lua_32.json