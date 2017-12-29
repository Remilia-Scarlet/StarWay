@echo off
pushd "%CD%"
cd  /d "%~dp0%"
call ..\..\Sharpmake\build\Sharpmake.Application.exe /sources("lua.sharpmake")
popd
pause