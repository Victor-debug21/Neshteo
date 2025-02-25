@echo off

set CommonCompilerFlags=-MT -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -DNESHTEO_INTERNAL=1 -DNESHTEO_SLOW=1 -DNESHTEO_WIN32=1 -FC -Z7
set CommonLinkerFlags= -incremental:no -opt:ref user32.lib gdi32.lib winmm.lib

REM TODO - can we just build both with one exe?

IF NOT EXIST ..\..\build mkdir ..\..\build
pushd ..\..\build

REM 32-bit build
REM cl %CommonCompilerFlags% ..\neshteo\code\win32_neshteo.cpp /link -subsystem:windows,5.1 %CommonLinkerFlags%

REM 64-bit build

del *.pdb > NUL 2> NUL

:: Tarih ve saat deðiþkenlerini düzgün formatta oluþtur
set CURRENT_DATE=%date:~-4,4%%date:~-10,2%%date:~-7,2%
set CURRENT_TIME=%time:~0,2%%time:~3,2%%time:~6,2%
set CURRENT_TIME=!CURRENT_TIME: =0!  REM Boþluklarý sýfýr ile deðiþtir

:: Derleme ve linkleme iþlemleri
cl %CommonCompilerFlags% ..\neshteo\code\neshteo.cpp -Fmneshteo.map -LD /link -incremental:no -opt:ref -PDB:neshteo_!CURRENT_DATE!_!CURRENT_TIME!.pdb -EXPORT:GameGetSoundSamples -EXPORT:GameUpdateAndRender
cl %CommonCompilerFlags% ..\neshteo\code\win32_neshteo.cpp -Fmwin32_neshteo.map /link %CommonLinkerFlags%
popd
