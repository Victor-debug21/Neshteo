@echo off

mkdir ..\..\build
pushd ..\..\build
cl -FC -Zi ..\neshteo\code\win32_neshteo.cpp user32.lib gdi32.lib
popd
