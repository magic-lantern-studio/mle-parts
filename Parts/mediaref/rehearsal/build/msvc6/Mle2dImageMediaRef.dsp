# Microsoft Developer Studio Project File - Name="Mle2dImageMediaRef" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Mle2dImageMediaRef - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Mle2dImageMediaRef.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Mle2dImageMediaRef.mak" CFG="Mle2dImageMediaRef - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Mle2dImageMediaRef - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Mle2dImageMediaRef - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Mle2dImageMediaRef - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MLE2DIMAGEMEDIAREF_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../../common/include" /I "$(MLE_HOME)/include" /I "$(COINDIR)/include" /I "$(FREEIMAGEDIR)" /D "MLE_REHEARSAL" /D "MLE_MAKE_DLL" /D "MLE_DLL" /D "COIN_DLL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MLE_2DIMAGEMEDIAREF_EXPORTS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 mlert.lib DWP.lib coin2.lib FreeImaged.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /libpath:"$(MLE_HOME)\lib\rehearsal" /libpath:"$(COINDIR)\lib" /libpath:"$(FREEIMAGEDIR)"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install DLL
PostBuild_Cmds=xcopy /y ..\..\..\common\include\mle\*.h $(MLE_HOME)\include\mle	mkdir $(MLE_HOME)\lib\rehearsal	copy Release\Mle2dImageMediaRef.lib $(MLE_HOME)\lib\rehearsal\Mle2dImageMediaRef.lib	mkdir $(MLE_HOME)\bin\rehearsal	copy Release\Mle2dImageMediaRef.dll $(MLE_HOME)\bin\rehearsal\Mle2dImageMediaRef.dll
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Mle2dImageMediaRef - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MLE2DIMAGEMEDIAREF_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../common/include" /I "$(MLE_HOME)/include" /I "$(COINDIR)/include" /I "$(FREEIMAGEDIR)" /D "MLE_DEBUG" /D "MLE_REHEARSAL" /D "MLE_MAKE_DLL" /D "MLE_DLL" /D "COIN_DLL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MLE_2DIMAGEMEDIAREF_EXPORTS" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 mlertd.lib DWPd.lib coin2d.lib FreeImaged.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"Debug/Mle2dImageMediaRefd.dll" /pdbtype:sept /libpath:"$(MLE_HOME)\lib\rehearsal" /libpath:"$(COINDIR)\lib" /libpath:"$(FREEIMAGEDIR)"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install DLL
PostBuild_Cmds=xcopy /y ..\..\..\common\include\mle\*.h $(MLE_HOME)\include\mle	mkdir $(MLE_HOME)\lib\rehearsal	copy Debug\Mle2dImageMediaRefd.lib $(MLE_HOME)\lib\rehearsal\Mle2dImageMediaRefd.lib	mkdir $(MLE_HOME)\bin\rehearsal	copy Debug\Mle2dImageMediaRefd.dll $(MLE_HOME)\bin\rehearsal\Mle2dImageMediaRefd.dll
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Mle2dImageMediaRef - Win32 Release"
# Name "Mle2dImageMediaRef - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\Mle2dImageMediaRef\Mle2dImageMediaRef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\MleIv2dImageMediaRef.cxx
# End Source File
# Begin Source File

SOURCE=..\..\Mle2dImageMediaRef\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\common\include\mle\imagmref.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\Mle2dImageMediaRef.h
# End Source File
# Begin Source File

SOURCE=..\..\Mle2dImageMediaRef\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\..\Mle2dImageMediaRef\ReadMe.txt
# End Source File
# End Target
# End Project
