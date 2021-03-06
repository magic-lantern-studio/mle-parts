# Microsoft Developer Studio Project File - Name="brender" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=brender - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "brender.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "brender.mak" CFG="brender - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "brender - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "brender - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "brender - Win32 Release"

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
# ADD CPP /nologo /Gz /Zp4 /W3 /GX /O2 /I ".." /I "..\..\..\common\include" /I "..\..\include" /I "..\..\..\..\sets\brender\include" /I "..\..\..\..\stages\win32\include" /I "..\..\..\..\stages\brender\include" /I "..\..\..\..\props\common" /I "..\..\..\..\mediaref\common\include" /I "..\..\..\..\mediaref\brender" /I "$(MLE_HOME)\include" /I "$(BRENDER_PATH)\..\INC" /D "MLE_NOT_DLL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=0 /D ML_FIXED_POINT=0 /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Release/brroles.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\brroles.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\brender	copy Release\brroles.lib $(MLE_HOME)\lib\brender\brroles.lib	xcopy /y ..\..\..\common\include\mle\*.h $(MLE_HOME)\include\mle	xcopy /y ..\..\include\mle\*.h $(MLE_HOME)\include\mle
# End Special Build Tool

!ELSEIF  "$(CFG)" == "brender - Win32 Debug"

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
# ADD CPP /nologo /Gz /Zp4 /W3 /Gm /GX /ZI /Od /I ".." /I "..\..\..\common\include" /I "..\..\include" /I "..\..\..\..\sets\brender\include" /I "..\..\..\..\stages\win32\include" /I "..\..\..\..\stages\brender\include" /I "..\..\..\..\props\common" /I "..\..\..\..\mediaref\common\include" /I "..\..\..\..\mediaref\brender" /I "$(MLE_HOME)\include" /I "$(BRENDER_PATH)\..\INC" /D "MLE_DEBUG" /D "MLE_NOT_DLL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=1 /D ML_FIXED_POINT=0 /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Debug/brrolesd.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\brrolesd.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\brender	copy Debug\brrolesd.lib $(MLE_HOME)\lib\brender\brrolesd.lib	xcopy /y ..\..\..\common\include\mle\*.h $(MLE_HOME)\include\mle	xcopy /y ..\..\include\mle\*.h $(MLE_HOME)\include\mle
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "brender - Win32 Release"
# Name "brender - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\3dcamc.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\3dcomapc.cxx

!IF  "$(CFG)" == "brender - Win32 Release"

!ELSEIF  "$(CFG)" == "brender - Win32 Debug"

# ADD CPP /I "..\..\stages\win32"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\3dmodelc.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\3dnodetc.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\3dtexmpc.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\3dtransc.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\br2drole.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\br3drole.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\common\include\mle\2drole.h
# End Source File
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

SOURCE=..\..\..\common\include\mle\3dnodetc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\3dtexmpc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\3dtransc.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mle\br3drole.h
# End Source File
# End Group
# End Target
# End Project
