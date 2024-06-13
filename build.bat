@echo off

cls

echo Preparing to build PDCurses v3.3...
echo.
if not exist "nmake" echo This is requires Microsoft Visual Studio .NET 2003/2005/2008.

SET PROJDIR=%~dp0

if not exist "out" mkdir out
cd out
if not exist "libs" mkdir libs
cd libs
if not exist "pdcurses" mkdir pdcurses
cd pdcurses

SET OUTDIR=%PROJDIR%/out

SET PDCURSES_SRCDIR=%PROJDIR%/libs/pdcurses

echo.

echo Enable Wide Character Set support for PDCurses?
echo.
echo NOTE: Building a Win32 application on MSVC2005 with a 
echo       Multi-Byte Character Set adds support for Windows 98.
echo.
set /p WIDECHAR_BUILD=Press Y(es) or N(o) key 


IF "%WIDECHAR_BUILD%"=="N" (
	nmake %PDCURSES_SRCDIR%/win32/vcwin32.mak DLL=Y
) ELSE (
	nmake %PDCURSES_SRCDIR%/win32/vcwin32.mak WIDE=Y DLL=Y
)

cd ../../..

pause