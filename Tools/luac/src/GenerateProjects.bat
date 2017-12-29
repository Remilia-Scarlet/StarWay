@echo off
pushd "%CD%"
cd  /d "%~dp0%"
call ..\..\Sharpmake\build\Sharpmake.Application.exe /sources("luac.sharpmake")
popd
pause