# Microsoft Developer Studio Project File - Name="actors" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=actors - Win32 CDECL Float Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "actors.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "actors.mak" CFG="actors - Win32 CDECL Float Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "actors - Win32 Float Release" (based on "Win32 (x86) Static Library")
!MESSAGE "actors - Win32 Float Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "actors - Win32 CDECL Float Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "actors - Win32 CDECL Float Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "actors - Win32 Float Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "actors___Win32_Float_Release"
# PROP BASE Intermediate_Dir "actors___Win32_Float_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /Zp4 /W3 /GX /O2 /I "..\..\common\include" /I "..\..\..\roles\common\include" /I "$(MLE_HOME)\include" /D "MLE_NOT_DLL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=0 /D ML_FIXED_POINT=0 /YX /FD /c
# ADD CPP /nologo /Gz /Zp4 /W3 /GX /O2 /I "..\..\common\include" /I "..\..\..\roles\common\include" /I "$(MLE_HOME)\include" /D "MLE_NOT_DLL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=0 /D ML_FIXED_POINT=0 /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Release\actors.lib"
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\runtime\stdcall	copy Release\actors.lib $(MLE_HOME)\lib\runtime\stdcall\actors.lib	xcopy /y ..\..\common\include\mle\*.h $(MLE_HOME)\include\mle
# End Special Build Tool

!ELSEIF  "$(CFG)" == "actors - Win32 Float Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "actors___Win32_Float_Debug"
# PROP BASE Intermediate_Dir "actors___Win32_Float_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /Zp4 /W3 /Gm /GX /ZI /Od /I "..\..\common\include" /I "..\..\..\roles\common\include" /I "$(MLE_HOME)\include" /D "MLE_NOT_DLL" /D "MLE_DEBUG" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=1 /D ML_FIXED_POINT=0 /YX /FD /GZ /c
# ADD CPP /nologo /Gz /Zp4 /W3 /Gm /GX /ZI /Od /I "..\..\common\include" /I "..\..\..\roles\common\include" /I "$(MLE_HOME)\include" /D "MLE_NOT_DLL" /D "MLE_DEBUG" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=1 /D ML_FIXED_POINT=0 /Fp"Debug/actorsd.pch" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"Debug/actorsd.bsc"
# ADD BSC32 /nologo /o"Debug/actorsd.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\actorsd.lib"
# ADD LIB32 /nologo /out:"Debug\actorsd.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\runtime\stdcall	copy Debug\actorsd.lib $(MLE_HOME)\lib\runtime\stdcall\actorsd.lib	xcopy /y ..\..\common\include\mle\*.h $(MLE_HOME)\include\mle
# End Special Build Tool

!ELSEIF  "$(CFG)" == "actors - Win32 CDECL Float Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "actors___Win32_CDECL_Float_Debug"
# PROP BASE Intermediate_Dir "actors___Win32_CDECL_Float_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CdeclFloatDebug"
# PROP Intermediate_Dir "CdeclFloatDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /Zp4 /W3 /Gm /GX /ZI /Od /I "..\..\common\include" /I "..\..\..\roles\common\include" /I "$(MLE_HOME)\include" /D "MLE_NOT_DLL" /D "MLE_DEBUG" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=1 /D ML_FIXED_POINT=0 /Fp"Debug/actorsd.pch" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\common\include" /I "..\..\..\roles\common\include" /I "$(MLE_HOME)\include" /D "MLE_NOT_DLL" /D "MLE_DEBUG" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=1 /D ML_FIXED_POINT=0 /Fp"Debug/actorsd.pch" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"Debug/actorsd.bsc"
# ADD BSC32 /nologo /o"CdeclFloatDebug/actorsd.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\actorsd.lib"
# ADD LIB32 /nologo /out:"CdeclFloatDebug\actorsd.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\runtime\cdecl	copy CdeclFloatDebug\actorsd.lib $(MLE_HOME)\lib\runtime\cdecl\actorsd.lib	xcopy /y ..\..\common\include\mle\*.h $(MLE_HOME)\include\mle
# End Special Build Tool

!ELSEIF  "$(CFG)" == "actors - Win32 CDECL Float Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "actors___Win32_CDECL_Float_Release"
# PROP BASE Intermediate_Dir "actors___Win32_CDECL_Float_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "CdeclFloatRelease"
# PROP Intermediate_Dir "CdeclFloatRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /Zp4 /W3 /GX /O2 /I "..\..\common\include" /I "..\..\..\roles\common\include" /I "$(MLE_HOME)\include" /D "MLE_NOT_DLL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=0 /D ML_FIXED_POINT=0 /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\common\include" /I "..\..\..\roles\common\include" /I "$(MLE_HOME)\include" /D "MLE_NOT_DLL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=0 /D ML_FIXED_POINT=0 /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\runtime\cdecl	copy CdeclFloatRelease\actors.lib $(MLE_HOME)\lib\runtime\cdecl\actors.lib	xcopy /y ..\..\common\include\mle\*.h $(MLE_HOME)\include\mle
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "actors - Win32 Float Release"
# Name "actors - Win32 Float Debug"
# Name "actors - Win32 CDECL Float Debug"
# Name "actors - Win32 CDECL Float Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\common\src\2dimga.cxx
# End Source File
# Begin Source File

SOURCE=..\..\common\src\3dannoa.cxx
# End Source File
# Begin Source File

SOURCE=..\..\common\src\3dcama.cxx
# End Source File
# Begin Source File

SOURCE=..\..\common\src\3dcamera.cxx
# End Source File
# Begin Source File

SOURCE=..\..\common\src\3dchara.cxx
# End Source File
# Begin Source File

SOURCE=..\..\common\src\3ddirlta.cxx
# End Source File
# Begin Source File

SOURCE=..\..\common\src\3dgeoma.cxx
# End Source File
# Begin Source File

SOURCE=..\..\common\src\3dlighta.cxx
# End Source File
# Begin Source File

SOURCE=..\..\common\src\3dlta.cxx
# End Source File
# Begin Source File

SOURCE=..\..\common\src\3dmodela.cxx
# End Source File
# Begin Source File

SOURCE=..\..\common\src\3dpntlta.cxx
# End Source File
# Begin Source File

SOURCE=..\..\common\src\3dposa.cxx
# End Source File
# Begin Source File

SOURCE=..\..\common\src\3dsptlta.cxx
# End Source File
# Begin Source File

SOURCE=..\..\common\src\3dtransa.cxx
# End Source File
# Begin Source File

SOURCE=..\..\common\src\3dtrsa.cxx
# End Source File
# Begin Source File

SOURCE=..\..\common\src\escapea.cxx
# End Source File
# Begin Source File

SOURCE=..\..\common\src\evader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\common\src\obstacle.cxx
# End Source File
# Begin Source File

SOURCE=..\..\common\src\pursuer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\common\src\wanderer.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\common\include\mle\2dimga.h
# End Source File
# Begin Source File

SOURCE=..\..\common\include\mle\3dannoa.h
# End Source File
# Begin Source File

SOURCE=..\..\common\include\mle\3dcama.h
# End Source File
# Begin Source File

SOURCE=..\..\common\include\mle\3dcamera.h
# End Source File
# Begin Source File

SOURCE=..\..\common\include\mle\3dchara.h
# End Source File
# Begin Source File

SOURCE=..\..\common\include\mle\3ddirlta.h
# End Source File
# Begin Source File

SOURCE=..\..\common\include\mle\3dgeoma.h
# End Source File
# Begin Source File

SOURCE=..\..\common\include\mle\3dlighta.h
# End Source File
# Begin Source File

SOURCE=..\..\common\include\mle\3dlta.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\mle\3dltap.h
# End Source File
# Begin Source File

SOURCE=..\..\common\include\mle\3dmodela.h
# End Source File
# Begin Source File

SOURCE=..\..\common\include\mle\3dpntlta.h
# End Source File
# Begin Source File

SOURCE=..\..\common\include\mle\3dposa.h
# End Source File
# Begin Source File

SOURCE=..\..\common\include\mle\3dsptlta.h
# End Source File
# Begin Source File

SOURCE=..\..\common\include\mle\3dtransa.h
# End Source File
# Begin Source File

SOURCE=..\..\common\include\mle\3dtrsa.h
# End Source File
# Begin Source File

SOURCE=..\..\common\include\mle\escapea.h
# End Source File
# Begin Source File

SOURCE=..\..\common\include\mle\evader.h
# End Source File
# Begin Source File

SOURCE=..\..\common\include\mle\obstacle.h
# End Source File
# Begin Source File

SOURCE=..\..\common\include\mle\pursuer.h
# End Source File
# Begin Source File

SOURCE=..\..\common\include\mle\wanderer.h
# End Source File
# End Group
# End Target
# End Project
