@echo off
pushd "%CD%"
cd  /d "%~dp0%"
set EXEFOLDER="%1"
if "%1" EQU "" (
	set EXEFOLDER="%cd%\output\win64"
)
if not exist %EXEFOLDER% md %EXEFOLDER%
cd %EXEFOLDER%

..\..\Tools\CommonCompiler\build\CommonCompiler.exe /config ..\..\StarWay\CommonBuilder\lua.json /d Platform=Win64 
..\..\Tools\CommonCompiler\build\CommonCompiler.exe /config ..\..\StarWay\CommonBuilder\shader.json /d Platform=Win64 /d ShaderType=ps 
..\..\Tools\CommonCompiler\build\CommonCompiler.exe /config ..\..\StarWay\CommonBuilder\shader.json /d Platform=Win64 /d ShaderType=vs 

popd