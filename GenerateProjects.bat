@echo off
pushd "%CD%"
cd  /d "%~dp0%"
mkdir build
cd build 
cmake ..
popd
pause