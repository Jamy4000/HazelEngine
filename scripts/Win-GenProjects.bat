@echo off
pushd ..\
call ThirdParties\bin\premake\premake5.exe vs2022
popd
PAUSE