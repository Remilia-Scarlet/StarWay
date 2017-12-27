@echo off
pushd
cd "%~d0"
call Tools\Sharpmake\build\Sharpmake.Application.exe /sources("Starway.sharpmake","Engine\\Ash\\project\\Ash.sharpmake","Engine\\Graphic\\project\\Graphic.sharpmake","Engine\\Math\\project\\Math.sharpmake")
popd
pause