@echo off

cls

echo Building PDCurses v3.3...
echo.

SET PROJDIR=%~dp0

mkdir out
cd out
mkdir libs
cd libs
mkdir pdcurses
cd pdcurses

SET OUTDIR=%PROJDIR%/out

SET PDCURSES_SRCDIR=%PROJDIR%/libs/pdcurses

echo.

set /p WIDECHAR_BUILD=Enable Wide-Character support for PDCurses? Press Y(es) or N(o) key 

nmake %PDCURSES_SRCDIR%/win32/vcwin32.mak WIDE=%WIDECHAR_BUILD% DLL=Y

cd ../../..

pause