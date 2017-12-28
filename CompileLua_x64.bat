@echo off
set EXEFOLDER="%1"
if "%1" EQU "" (
	set EXEFOLDER="%cd%\output\win64"
)
cd %EXEFOLDER%
..\..\Tools\CommonCompiler\build\CommonCompiler_64.exe /config ..\..\StarWay\CommonBuilder\lua_64.json
..\..\Tools\CommonCompiler\build\CommonCompiler_32.exe /config ..\..\StarWay\CommonBuilder\shader_ps.json
..\..\Tools\CommonCompiler\build\CommonCompiler_32.exe /config ..\..\StarWay\CommonBuilder\shader_vs.json