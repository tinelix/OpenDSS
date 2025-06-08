# Microsoft Developer Studio Project File - Name="OpenDSE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=OpenDSE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OpenDSE.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OpenDSE.mak" CFG="OpenDSE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OpenDSE - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OpenDSE - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OpenDSE - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "out/obj"
# PROP Intermediate_Dir "out/interm"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MSVC" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x409 /i "../include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /dll /machine:I386 /out:"out/bin/OpenDSE.dll"

!ELSEIF  "$(CFG)" == "OpenDSE - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "out/obj"
# PROP Intermediate_Dir "out/interm"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "../include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MSVC" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"out/bin/OpenDSE.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "OpenDSE - Win32 Release"
# Name "OpenDSE - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Group "Platform-specific (H)"

# PROP Default_Filter ""
# Begin Group "Windows (H)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\platform\windows\dsemodl.h
# End Source File
# Begin Source File

SOURCE=..\include\platform\windows\dsewin.h
# End Source File
# Begin Source File

SOURCE=..\include\platform\windows\errcodes.h
# End Source File
# Begin Source File

SOURCE=..\include\platform\windows\error.h
# End Source File
# End Group
# End Group
# Begin Group "Utilities (H)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\utils\errcodes.h
# End Source File
# Begin Source File

SOURCE=..\include\utils\extstr.h
# End Source File
# Begin Source File

SOURCE=..\include\utils\fileext.h
# End Source File
# Begin Source File

SOURCE=..\include\utils\player.h
# End Source File
# End Group
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ".c;.cpp"
# Begin Group "Platform-specific (S)"

# PROP Default_Filter ""
# Begin Group "Windows (S)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\platform\windows\dsemodl.c
# End Source File
# Begin Source File

SOURCE=..\src\platform\windows\dsewin.c
# End Source File
# Begin Source File

SOURCE=..\src\platform\windows\fileext.c
# End Source File
# End Group
# Begin Group "Utilities (S)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\utils\extstr.c
# End Source File
# Begin Source File

SOURCE=..\src\utils\player.c
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=..\src\dse.c
# End Source File
# Begin Source File

SOURCE=..\src\pch.c
# End Source File
# End Group
# End Target
# End Project
