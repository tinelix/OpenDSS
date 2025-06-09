# Microsoft Developer Studio Project File - Name="dse_mp3dec" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=dse_mp3dec - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mp3dec.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mp3dec.mak" CFG="dse_mp3dec - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dse_mp3dec - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "dse_mp3dec - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "dse_mp3dec - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MSVC" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"out/bin/dse_mp3dec.dll"

!ELSEIF  "$(CFG)" == "dse_mp3dec - Win32 Debug"

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
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"out/bin/mp3dec.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "dse_mp3dec - Win32 Release"
# Name "dse_mp3dec - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ".c;.cpp"
# Begin Group "Platform-specific (S)"

# PROP Default_Filter ""
# Begin Group "Windows (S)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\platform\windows\mp3decw.c
# End Source File
# End Group
# End Group
# Begin Group "MPEG-ES (S)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\mpeges\stream.c
# End Source File
# End Group
# Begin Group "Mathematics (S)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\math\descale.c
# End Source File
# Begin Source File

SOURCE=..\src\math\huffman.c
# End Source File
# Begin Source File

SOURCE=..\src\math\imdct.c
# End Source File
# End Group
# Begin Group "Utilities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\utils\intext.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\src\mp3dec.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Group "Platform-specific (H)"

# PROP Default_Filter ""
# Begin Group "Windows"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\platform\windows\mp3decw.h
# End Source File
# End Group
# End Group
# Begin Group "MPEG-ES (H)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\mpeges\stream.h
# End Source File
# End Group
# Begin Group "Mathematics (H)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\math\descale.h
# End Source File
# Begin Source File

SOURCE=..\include\math\huffman.h
# End Source File
# Begin Source File

SOURCE=..\include\math\imdct.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\include\mp3dec.h
# End Source File
# End Group
# End Target
# End Project
