@echo off
pushd "%CD%"
cd  /d "%~dp0%"\Engine\Extern\boost

@echo on
call bootstrap.bat
call b2 --build-type=complete address-model=64

@echo off
popd
pause