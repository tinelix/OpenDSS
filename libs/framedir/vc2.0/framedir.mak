# Microsoft Visual C++ Generated NMAKE File, Format Version 2.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

!IF "$(CFG)" == ""
CFG=Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Win32 Release" && "$(CFG)" != "Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "framedir.mak" CFG="Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

################################################################################
# Begin Project
# PROP Target_Last_Scanned "Win32 Debug"
CPP=cl.exe

!IF  "$(CFG)" == "Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../out/vc2.0/win32/obj"
# PROP Intermediate_Dir "../out/vc2.0/win32/interm"
OUTDIR=../out/vc2.0/win32/obj
INTDIR=../out/vc2.0/win32/interm

ALL : \
\Users\tretdm\Sources\OpenDSS\libs\framedir\out\vc2.0\win32\bin\framedir.lib\
 $(OUTDIR)/framedir.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

$(INTDIR) : 
    if not exist $(INTDIR)/nul mkdir $(INTDIR)

# ADD BASE CPP /nologo /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /W3 /GX /YX /O2 /I "../include" /D "NDEBUG" /D "_MSVC" /D "_MSVC2" /D "WIN32" /D "_WIN32" /D "_WINDOWS" /FR /c
CPP_PROJ=/nologo /W3 /GX /YX /O2 /I "../include" /D "NDEBUG" /D "_MSVC" /D\
 "_MSVC2" /D "WIN32" /D "_WIN32" /D "_WINDOWS" /FR$(INTDIR)/\
 /Fp$(OUTDIR)/"framedir.pch" /Fo$(INTDIR)/ /c 
CPP_OBJS=.\../out/vc2.0/win32/interm/
BSC32=bscmake.exe
BSC32_SBRS= \
	$(INTDIR)/framedir.sbr
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"framedir.bsc" 

$(OUTDIR)/framedir.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=lib.exe
DEF_FLAGS=
DEF_FILE=
LIB32_OBJS= \
	$(INTDIR)/framedir.obj
# ADD BASE LIB32 /NOLOGO
# ADD LIB32 /NOLOGO /OUT:"../out/vc2.0/win32/bin/framedir.lib"
LIB32_FLAGS=/NOLOGO /OUT:"../out/vc2.0/win32/bin/framedir.lib" 

\Users\tretdm\Sources\OpenDSS\libs\framedir\out\vc2.0\win32\bin\framedir.lib : \
$(OUTDIR)  $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../out/vc2.0/win32/obj"
# PROP Intermediate_Dir "../out/vc2.0/win32/interm"
OUTDIR=../out/vc2.0/win32/obj
INTDIR=../out/vc2.0/win32/interm

ALL : \
\Users\tretdm\Sources\OpenDSS\libs\framedir\out\vc2.0\win32\bin\framedir.lib\
 $(OUTDIR)/framedir.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

$(INTDIR) : 
    if not exist $(INTDIR)/nul mkdir $(INTDIR)

# ADD BASE CPP /nologo /W3 /GX /Z7 /YX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /W3 /GX /Z7 /YX /Od /I "../include" /D "_DEBUG" /D "_MSVC" /D "_MSVC2" /D "WIN32" /D "_WIN32" /D "_WINDOWS" /FR /c
CPP_PROJ=/nologo /W3 /GX /Z7 /YX /Od /I "../include" /D "_DEBUG" /D "_MSVC" /D\
 "_MSVC2" /D "WIN32" /D "_WIN32" /D "_WINDOWS" /FR$(INTDIR)/\
 /Fp$(OUTDIR)/"framedir.pch" /Fo$(INTDIR)/ /c 
CPP_OBJS=.\../out/vc2.0/win32/interm/
BSC32=bscmake.exe
BSC32_SBRS= \
	$(INTDIR)/framedir.sbr
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"framedir.bsc" 

$(OUTDIR)/framedir.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=lib.exe
DEF_FLAGS=
DEF_FILE=
LIB32_OBJS= \
	$(INTDIR)/framedir.obj
# ADD BASE LIB32 /NOLOGO
# ADD LIB32 /NOLOGO /OUT:"../out/vc2.0/win32/bin/framedir.lib"
LIB32_FLAGS=/NOLOGO /OUT:"../out/vc2.0/win32/bin/framedir.lib" 

\Users\tretdm\Sources\OpenDSS\libs\framedir\out\vc2.0\win32\bin\framedir.lib : \
$(OUTDIR)  $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Group "Source Files"

################################################################################
# Begin Source File

SOURCE=\Users\tretdm\Sources\OpenDSS\libs\framedir\src\framedir.c

$(INTDIR)/framedir.obj :  $(SOURCE)  $(INTDIR)
   $(CPP) $(CPP_PROJ)  $(SOURCE) 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\tretdm\Sources\OpenDSS\libs\framedir\include\framedir.h
# End Source File
# End Group
# End Project
################################################################################
