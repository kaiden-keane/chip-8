@echo off
setlocal enabledelayedexpansion

if not exist out mkdir out

set name=chip-8

set flags=-D WINDOWS_BUILD -I ../../include -MD -link -LIBPATH:"../..\lib"
set other=raylib.lib kernel32.lib opengl32.lib shell32.lib winmm.lib user32.lib gdi32.lib
set src=
for %%f in ("%../src/*.c") do (
    set src=!src! ../../src/%%~nxf
)

pushd out
cl /Zi /EHsc /Fe%name%.exe !src! %flags% %other%


rem cleanup
@REM move %name%.exe ../

popd

endlocal