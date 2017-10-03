@echo off

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

ctime -begin sdf_tracer.ctm

del *.pdb 1> NUL 2> NUL

REM Compile the executable -----------------------------------------------
set CompilerFlags=-MT -nologo -FC -Z7 -W4 -WX -Od -wd4100 -wd4003 -wd4127 -wd4505 -wd4201 -wd4189 -wd4996 -wd4005
set LinkerFlags=-incremental:no user32.lib gdi32.lib winmm.lib Opengl32.lib shell32.lib
cl  %CompilerFlags% ..\code\main.cpp /link -PDB:sdf_tracer_%random%.pdb %LinkerFlags% 

ctime -end sdf_tracer.ctm

popd

