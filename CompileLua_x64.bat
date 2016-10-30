@echo off
set EXEFOLDER="%1"
if "%1" EQU "" (
	set EXEFOLDER="%cd%\output\x64"
)
cd %EXEFOLDER%
..\..\Tools\CommonCompiler\build\CommonCompiler_64.exe /config ..\..\StarWay\CommonBuilder\lua_64.json