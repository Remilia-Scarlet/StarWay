@echo off
pushd "%CD%"
cd  /d "%~dp0%"

set EXEFOLDER="%cd%\output\Win32"
if not exist %EXEFOLDER% md %EXEFOLDER%
cd %EXEFOLDER%

..\..\Tools\CommonCompiler\build\CommonCompiler.exe /config ..\..\StarWay\CommonBuilder\lua.json /d Platform=Win32 
..\..\Tools\CommonCompiler\build\CommonCompiler.exe /config ..\..\StarWay\CommonBuilder\shader.json /d Platform=Win32 /d ShaderType=ps 
..\..\Tools\CommonCompiler\build\CommonCompiler.exe /config ..\..\StarWay\CommonBuilder\shader.json /d Platform=Win32 /d ShaderType=vs 

popd