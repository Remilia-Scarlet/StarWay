@echo off
pushd "%CD%"
cd  /d "%~dp0%"
call Tools\Sharpmake\build\Sharpmake.Application.exe /sources("Starway.sharpmake")
call Tools\Sharpmake\build\Sharpmake.Application.exe /sources("Tools\\CommonCompiler\\src\\CommonCompiler.sharpmake")
popd
pause