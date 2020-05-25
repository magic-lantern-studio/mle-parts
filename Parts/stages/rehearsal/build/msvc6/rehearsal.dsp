# Microsoft Developer Studio Project File - Name="rehearsal" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=rehearsal - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rehearsal.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rehearsal.mak" CFG="rehearsal - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rehearsal - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "rehearsal - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rehearsal - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "REHEARSAL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /I "..\..\..\inventor" /I "..\..\include" /I "$(MLE_HOME)\Parts\sets\inventor" /I "$(MLE_HOME)\include" /I "$(MLE_HOME)\ATK\common\include" /I "$(MLE_HOME)\ATK\win32\include" /I "$(COINDIR)\include" /D "MLE_REHEARSAL" /D "MLE_DLL" /D ML_MATH_DEBUG=0 /D ML_FIXED_POINT=0 /D "COIN_DLL" /D "SOWIN_DLL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "REHEARSAL_EXPORTS" /FD /c
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
# ADD LINK32 mleatk.lib Mle2dRole.lib Mle3dRole.lib Mle2dSet.lib Mle3dSet.lib DWP.lib mlert.lib mlmath.lib coin2.lib sowin1.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib /nologo /dll /machine:I386 /out:"Release/MleIvStage.dll" /libpath:"$(MLE_HOME)\lib\rehearsal" /libpath:"$(COINDIR)\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install DLL
PostBuild_Cmds=mkdir $(MLE_HOME)\bin\rehearsal	copy Release\MleIvStage.dll $(MLE_HOME)\bin\rehearsal	copy Release\MleIvStage.lib $(MLE_HOME)\lib\rehearsal	xcopy /y ..\..\..\inventor\mle\*.h $(MLE_HOME)\include\mle	xcopy /y ..\..\include\mle\*.h $(MLE_HOME)\include\mle
# End Special Build Tool

!ELSEIF  "$(CFG)" == "rehearsal - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "REHEARSAL_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "..\..\..\inventor" /I "..\..\include" /I "$(MLE_HOME)\Parts\sets\inventor" /I "$(MLE_HOME)\include" /I "$(MLE_HOME)\ATK\common\include" /I "$(MLE_HOME)\ATK\win32\include" /I "$(COINDIR)\include" /D "MLE_DEBUG" /D "MLE_REHEARSAL" /D "MLE_DLL" /D ML_MATH_DEBUG=1 /D ML_FIXED_POINT=0 /D "COIN_DLL" /D "SOWIN_DLL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "REHEARSAL_EXPORTS" /FD /GZ /c
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
# ADD LINK32 mleatkd.lib Mle2dRoled.lib Mle3dRoled.lib Mle2dSetd.lib Mle3dSetd.lib DWPd.lib mlertd.lib mlmathd.lib coin2d.lib sowin1d.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib /nologo /dll /map /debug /machine:I386 /out:"Debug/MleIvStaged.dll" /pdbtype:sept /libpath:"$(MLE_HOME)\lib\rehearsal" /libpath:"$(COINDIR)\lib" /section:".GLOBALS,rws"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install DLL
PostBuild_Cmds=mkdir $(MLE_HOME)\bin\rehearsal	copy Debug\MleIvStaged.dll $(MLE_HOME)\bin\rehearsal	copy Debug\MleIvStaged.lib $(MLE_HOME)\lib\rehearsal	xcopy /y ..\..\..\inventor\mle\*.h $(MLE_HOME)\include\mle	xcopy /y ..\..\include\mle\*.h $(MLE_HOME)\include\mle
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "rehearsal - Win32 Release"
# Name "rehearsal - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\inventor\antirot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\CubeDragger.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\CubeManip.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\ivstage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\MleIvStage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\MultiSearchAction.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\Nudger.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\Snapper.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\Source.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\SpotMarker.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\Target.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\inventor\antirot.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\CubeDragger.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\CubeDraggerGeom.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\CubeManip.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\GridGraph.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\mle\ivplatform.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\mle\ivstage.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mle\MleIvStage.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\MultiSearchAction.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\Nudger.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\Snapper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\Source.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\SpotMarker.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\SpotMarkerGraph.h
# End Source File
# Begin Source File

SOURCE=..\..\src\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inventor\Target.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\..\src\ReadMe.txt
# End Source File
# End Target
# End Project
