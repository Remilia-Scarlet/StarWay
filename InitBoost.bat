@echo off
pushd "%CD%"
cd  /d "%~dp0%"\Engine\Extern\boost

@echo on
call bootstrap.bat
call b2 headers

@echo off
popd
pause