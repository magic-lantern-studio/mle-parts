# Microsoft Developer Studio Project File - Name="Mle3dModelMediaRef" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Mle3dModelMediaRef - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Mle3dModelMediaRef.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Mle3dModelMediaRef.mak" CFG="Mle3dModelMediaRef - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Mle3dModelMediaRef - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Mle3dModelMediaRef - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Mle3dModelMediaRef - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MLE3DMODELMEDIAREF_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../../common/include" /I "$(MLE_HOME)/include" /I "$(COINDIR)/include" /D "MLE_REHEARSAL" /D "MLE_MAKE_DLL" /D "MLE_DLL" /D "COIN_DLL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MLE_3DMODELMEDIAREF_EXPORTS" /FD /c
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
# ADD LINK32 mlert.lib DWP.lib coin2.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /libpath:"$(MLE_HOME)\lib\rehearsal" /libpath:"$(COINDIR)\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install DLL
PostBuild_Cmds=xcopy /y ..\..\..\common\include\mle\*.h $(MLE_HOME)\include\mle	mkdir $(MLE_HOME)\lib\rehearsal	copy Release\Mle3dModelMediaRef.lib $(MLE_HOME)\lib\rehearsal\Mle3dModelMediaRef.lib	mkdir $(MLE_HOME)\bin\rehearsal	copy Release\Mle3dModelMediaRef.dll $(MLE_HOME)\bin\rehearsal\Mle3dModelMediaRef.dll
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Mle3dModelMediaRef - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MLE3DMODELMEDIAREF_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../common/include" /I "$(MLE_HOME)/include" /I "$(COINDIR)/include" /D "MLE_DEBUG" /D "MLE_REHEARSAL" /D "MLE_MAKE_DLL" /D "MLE_DLL" /D "COIN_DLL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MLE_3DMODELMEDIAREF_EXPORTS" /FD /GZ /c
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
# ADD LINK32 mlertd.lib DWPd.lib coin2d.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"Debug/Mle3dModelMediaRefd.dll" /pdbtype:sept /libpath:"$(MLE_HOME)\lib\rehearsal" /libpath:"$(COINDIR)\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install DLL
PostBuild_Cmds=xcopy /y ..\..\..\common\include\mle\*.h $(MLE_HOME)\include\mle	mkdir $(MLE_HOME)\lib\rehearsal	copy Debug\Mle3dModelMediaRefd.lib $(MLE_HOME)\lib\rehearsal\Mle3dModelMediaRefd.lib	mkdir $(MLE_HOME)\bin\rehearsal	copy Debug\Mle3dModelMediaRefd.dll $(MLE_HOME)\bin\rehearsal\Mle3dModelMediaRefd.dll
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Mle3dModelMediaRef - Win32 Release"
# Name "Mle3dModelMediaRef - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\Mle3dModelMediaRef\Mle3dModelMediaRef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\MleIv3dModelMediaRef.cxx
# End Source File
# Begin Source File

SOURCE=..\..\Mle3dModelMediaRef\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\common\include\mle\3dmodelm.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\Mle3dModelMediaRef.h
# End Source File
# Begin Source File

SOURCE=..\..\Mle3dModelMediaRef\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\..\Mle3dModelMediaRef\ReadMe.txt
# End Source File
# End Target
# End Project
