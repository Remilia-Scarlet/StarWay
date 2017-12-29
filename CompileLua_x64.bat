@echo off
set EXEFOLDER="%1"
if "%1" EQU "" (
	set EXEFOLDER="%cd%\output\win64"
)
if not exist %EXEFOLDER% md %EXEFOLDER%
cd %EXEFOLDER%
..\..\Tools\CommonCompiler\build\CommonCompiler.exe /config ..\..\StarWay\CommonBuilder\lua_64.json
..\..\Tools\CommonCompiler\build\CommonCompiler.exe /config ..\..\StarWay\CommonBuilder\shader_ps.json
..\..\Tools\CommonCompiler\build\CommonCompiler.exe /config ..\..\StarWay\CommonBuilder\shader_vs.json