@echo off

SET PROJECT_PATH=%~dp0

MKDIR out\libs\pdcurses
CD out\libs\pdcurses

SET PDCURSES_SRCDIR=%PROJECT_PATH%\libs\pdcurses

NMAKE %PROJECT_PATH%\libs\pdcurses\win32\vcwin32.mak DLL=Y

CD ..\..\..