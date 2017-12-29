@echo off
set EXEFOLDER="%1"
if "%1" EQU "" (
	set EXEFOLDER="%cd%\output\Win32"
)
cd %EXEFOLDER%
..\..\Tools\CommonCompiler\build\CommonCompiler.exe /config ..\..\StarWay\CommonBuilder\lua_32.json
..\..\Tools\CommonCompiler\build\CommonCompiler.exe /config ..\..\StarWay\CommonBuilder\shader_ps.json
..\..\Tools\CommonCompiler\build\CommonCompiler.exe /config ..\..\StarWay\CommonBuilder\shader_vs.json