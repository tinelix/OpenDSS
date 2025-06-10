# Microsoft Developer Studio Project File - Name="OpenDSS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=OpenDSS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OpenDSS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OpenDSS.mak" CFG="OpenDSS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OpenDSS - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "OpenDSS - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OpenDSS - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "out/release/obj"
# PROP Intermediate_Dir "out/release/intermediate"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../include" /I "../libs/pdcurses" /I "../libs/tinydir" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_MSVC" /D "_PDCURSES" /YX /FD /STACK:33554432 /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../libs/pdcurses/win32/curses.lib /nologo /subsystem:console /profile /machine:I386 /out:"out/release/bin/OpenDSS.exe"

!ELSEIF  "$(CFG)" == "OpenDSS - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "out/debug/obj"
# PROP Intermediate_Dir "out/debug/intermediate"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I "../src" /I "../libs/pdcurses" /I "../libs" /I "../../libs" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_MSVC" /D "_PDCURSES" /FR /FD /STACK:2000000 /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../libs/pdcurses/win32/pdcurses.lib ../../libs/framedir/out/bin/framedir.lib /nologo /subsystem:console /debug /machine:I386 /out:"out/debug/bin/OpenDSS.exe" /pdbtype:sept
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "OpenDSS - Win32 Release"
# Name "OpenDSS - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter "*.h"
# Begin Group "UI Controls (H)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\controls\extwnd.h
# End Source File
# Begin Source File

SOURCE=..\src\controls\listbox.h
# End Source File
# Begin Source File

SOURCE=..\src\controls\msgbox.h
# End Source File
# Begin Source File

SOURCE=..\src\controls\uictrl.h
# End Source File
# End Group
# Begin Group "Interfaces (H)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\interfaces\auddeci.h
# End Source File
# Begin Source File

SOURCE=..\src\interfaces\filemani.h
# End Source File
# Begin Source File

SOURCE=..\src\interfaces\pguimani.h
# End Source File
# End Group
# Begin Group "Utilities (H)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\utils\audtags.h
# End Source File
# Begin Source File

SOURCE=..\src\utils\extstr.h
# End Source File
# Begin Source File

SOURCE=..\src\utils\fileman.h
# End Source File
# Begin Source File

SOURCE=..\src\utils\pguiman.h
# End Source File
# End Group
# Begin Group "Windows (H)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\windows\filemanw.h
# End Source File
# Begin Source File

SOURCE=..\src\windows\playerw.h
# End Source File
# End Group
# Begin Group "Wrappers (H)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\wrappers\dsewrap.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\src\version.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "*.cpp;*.c"
# Begin Group "UI Controls (S)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\controls\extwnd.cpp
# End Source File
# Begin Source File

SOURCE=..\src\controls\listbox.cpp
# End Source File
# Begin Source File

SOURCE=..\src\controls\msgbox.cpp
# End Source File
# End Group
# Begin Group "Interfaces (S)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\interfaces\auddeci.cpp
# End Source File
# End Group
# Begin Group "Utilities (S)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\utils\audtags.cpp
# End Source File
# Begin Source File

SOURCE=..\src\utils\extstr.cpp
# End Source File
# Begin Source File

SOURCE=..\src\utils\fileman.cpp
# End Source File
# Begin Source File

SOURCE=..\src\utils\pguiman.cpp
# End Source File
# End Group
# Begin Group "Windows (S)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\windows\filemanw.cpp
# End Source File
# Begin Source File

SOURCE=..\src\windows\playerw.cpp
# End Source File
# End Group
# Begin Group "Wrapper (S)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\wrappers\dsewrap.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\src\opendss.cpp
# End Source File
# End Group
# Begin Group "Libraries"

# PROP Default_Filter ""
# Begin Group "libid3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\libs\libid3\src\hexarr.cpp
# End Source File
# Begin Source File

SOURCE=..\libs\libid3\src\hexarr.h
# End Source File
# Begin Source File

SOURCE=..\libs\libid3\src\id3.cpp
# End Source File
# Begin Source File

SOURCE=..\libs\libid3\src\id3.h
# End Source File
# End Group
# End Group
# End Target
# End Project
