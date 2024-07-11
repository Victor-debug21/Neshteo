@echo off

IF NOT EXIST ..\..\build mkdir ..\..\build
pushd ..\..\build
cl -DHANDMADE_INTERNAL=1 -DHANDMADE_SLOW=1 -DNESHTEO_WIN32=1 -FC -Zi ..\neshteo\code\win32_neshteo.cpp user32.lib gdi32.lib
popd
