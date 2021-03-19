@echo off
pushd "%CD%"
cd  /d "%~dp0%"
mkdir build
cd build 
cmake -A x64 ..
popd
pause