@echo off
pushd ..\
call vendor\bin\premake\premake5.exe vs2022 --file=premake5_release.lua
popd
PAUSE