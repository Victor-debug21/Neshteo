@echo off

set CommonCompilerFlags=-MT -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -DHANDMADE_INTERNAL=1 -DHANDMADE_SLOW=1 -DNESHTEO_WIN32=1 -FC -Z7 -Fmwin32_neshteo.map
set CommonLinkerFlags= -opt:ref user32.lib gdi32.lib winmm.lib

REM TODO - can we just build both with one exe?

IF NOT EXIST ..\..\build mkdir ..\..\build
pushd ..\..\build



cl %CommonCompilerFlags% ..\neshteo\code\win32_neshteo.cpp /link %CommonLinkerFlags%
popd
