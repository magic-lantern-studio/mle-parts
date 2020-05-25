# Microsoft Developer Studio Project File - Name="inventor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=inventor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "inventor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "inventor.mak" CFG="inventor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "inventor - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "inventor - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "inventor - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I ".." /I "..\..\..\common\include" /I "..\..\include" /I "..\..\..\..\sets\inventor" /I "..\..\..\..\stages\inventor\include" /I "..\..\..\..\props\common" /I "..\..\..\..\mediaref\common\include" /I "..\..\..\..\mediaref\inventor" /I "$(MLE_HOME)\include" /I "$(COINDIR)\include" /D "MLE_REHEARSAL" /D "MLE_DLL" /D "COIN_DLL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=0 /D ML_FIXED_POINT=0 /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\ivroles.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=copy Release\ivroles.lib $(MLE_HOME)\lib\rehearsal\ivroles.lib	xcopy /y ..\..\..\common\include\mle\*.h $(MLE_HOME)\include\mle	xcopy /y ..\..\include\mle\*.h $(MLE_HOME)\include\mle
# End Special Build Tool

!ELSEIF  "$(CFG)" == "inventor - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I ".." /I "..\..\..\common\include" /I "..\..\include" /I "..\..\..\..\sets\inventor" /I "..\..\..\..\stages\inventor\include" /I "..\..\..\..\props\common" /I "..\..\..\..\mediaref\common\include" /I "..\..\..\..\mediaref\inventor" /I "$(MLE_HOME)\include" /I "$(COINDIR)\include" /D "MLE_DEBUG" /D "MLE_REHEARSAL" /D "MLE_DLL" /D "COIN_DLL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=1 /D ML_FIXED_POINT=0 /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\ivrolesd.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=copy Debug\ivrolesd.lib $(MLE_HOME)\lib\rehearsal\ivrolesd.lib	xcopy /y ..\..\..\common\include\mle\*.h $(MLE_HOME)\include\mle	xcopy /y ..\..\include\mle\*.h $(MLE_HOME)\include\mle
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "inventor - Win32 Release"
# Name "inventor - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\MleIv3dCameraCarrier.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\MleIv3dColorMapCarrier.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\MleIv3dModelCarrier.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\MleIv3dTextureMapCarrier.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\MleIv3dTransformCarrier.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\common\include\mle\3dcamc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\3dcomapc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\3dmodelc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\3dtexmpc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\3dtransc.h
# End Source File
# End Group
# End Target
# End Project
