# Microsoft Developer Studio Project File - Name="packages" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=packages - Win32 Fixed Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "packages.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "packages.mak" CFG="packages - Win32 Fixed Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "packages - Win32 Float Release" (based on "Win32 (x86) Static Library")
!MESSAGE "packages - Win32 Float Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "packages - Win32 Fixed Release" (based on "Win32 (x86) Static Library")
!MESSAGE "packages - Win32 Fixed Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "packages - Win32 Float Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "packages___Win32_Float_Release"
# PROP BASE Intermediate_Dir "packages___Win32_Float_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "FloatRelease"
# PROP Intermediate_Dir "FloatRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\..\common\include" /I "$(MLE_HOME)\Core\mlert\common\src\foundation" /I "$(MLE_HOME)\Core\math\common\include" /I "$(MLE_HOME)\Core\util\common\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=0 /D ML_FIXED_POINT=0 /D "MLE_NOT_DLL" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"FloatRelease/pkgs.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"FloatRelease\pkgs.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\runtime\cdecl	copy FloatRelease\pkgs.lib $(MLE_HOME)\lib\runtime\cdecl\pkgs.lib	mkdir $(MLE_HOME)\include\mle	xcopy /Y ..\..\..\common\include\mle\* $(MLE_HOME)\include\mle
# End Special Build Tool

!ELSEIF  "$(CFG)" == "packages - Win32 Float Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "packages___Win32_Float_Debug"
# PROP BASE Intermediate_Dir "packages___Win32_Float_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "FloatDebug"
# PROP Intermediate_Dir "FloatDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\..\common\include" /I "$(MLE_HOME)\Core\mlert\common\src\foundation" /I "$(MLE_HOME)\Core\math\common\include" /I "$(MLE_HOME)\Core\util\common\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=1 /D ML_FIXED_POINT=0 /D "MLE_NOT_DLL" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\common\include" /I "$(MLE_HOME)\Core\mlert\common\src\foundation" /I "$(MLE_HOME)\Core\math\common\include" /I "$(MLE_HOME)\Core\util\common\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=1 /D ML_FIXED_POINT=0 /D "MLE_NOT_DLL" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"FloatDebug/pkgsd.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"FloatDebug\pkgsd.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\runtime\cdecl	copy FloatDebug\pkgsd.lib $(MLE_HOME)\lib\runtime\cdecl\pkgsd.lib	mkdir $(MLE_HOME)\include\mle	xcopy /Y ..\..\..\common\include\mle\* $(MLE_HOME)\include\mle
# End Special Build Tool

!ELSEIF  "$(CFG)" == "packages - Win32 Fixed Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "packages___Win32_Fixed_Release"
# PROP BASE Intermediate_Dir "packages___Win32_Fixed_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "FixedRelease"
# PROP Intermediate_Dir "FixedRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "..\..\..\common\include" /I "$(MLE_HOME)\Core\mlert\common\src\foundation" /I "$(MLE_HOME)\Core\math\common\include" /I "$(MLE_HOME)\Core\util\common\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=0 /D ML_FIXED_POINT=0 /D "MLE_NOT_DLL" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\..\common\include" /I "$(MLE_HOME)\Core\mlert\common\src\foundation" /I "$(MLE_HOME)\Core\math\common\include" /I "$(MLE_HOME)\Core\util\common\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=0 /D ML_FIXED_POINT=1 /D "MLE_NOT_DLL" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"FixedRelease/fpkgs.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"FixedRelease\fpkgs.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\runtime\cdecl	copy FixedRelease\fpkgs.lib $(MLE_HOME)\lib\runtime\cdecl\fpkgs.lib	mkdir $(MLE_HOME)\include\mle	xcopy /Y ..\..\..\common\include\mle\* $(MLE_HOME)\include\mle
# End Special Build Tool

!ELSEIF  "$(CFG)" == "packages - Win32 Fixed Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "packages___Win32_Fixed_Debug"
# PROP BASE Intermediate_Dir "packages___Win32_Fixed_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "FixedDebug"
# PROP Intermediate_Dir "FixedDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\..\common\include" /I "$(MLE_HOME)\Core\mlert\common\src\foundation" /I "$(MLE_HOME)\Core\math\common\include" /I "$(MLE_HOME)\Core\util\common\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=1 /D ML_FIXED_POINT=0 /D "MLE_NOT_DLL" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\common\include" /I "$(MLE_HOME)\Core\mlert\common\src\foundation" /I "$(MLE_HOME)\Core\math\common\include" /I "$(MLE_HOME)\Core\util\common\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=1 /D ML_FIXED_POINT=1 /D "MLE_NOT_DLL" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"FixedDebug/fpkgsd.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"FloatDebug\packagesd.lib"
# ADD LIB32 /nologo /out:"FixedDebug\fpkgsd.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\runtime\cdecl	copy FixedDebug\fpkgsd.lib $(MLE_HOME)\lib\runtime\cdecl\fpkgsd.lib	mkdir $(MLE_HOME)\include\mle	xcopy /Y ..\..\..\common\include\mle\* $(MLE_HOME)\include\mle
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "packages - Win32 Float Release"
# Name "packages - Win32 Float Debug"
# Name "packages - Win32 Fixed Release"
# Name "packages - Win32 Fixed Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\common\src\align.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\avoidsph.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\banker.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\behavlib.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\cohere.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\gvehicle.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\path.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\pe.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\separate.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\simpmass.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\sphobs.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\spiral.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\stabiliz.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\steermas.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\vehicle.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\wander.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\wraptest.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\common\include\mle\align.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\avoidsph.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\banker.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\behavlib.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\cohere.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\gvehicle.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\path.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\pe.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\separate.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\simpmass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\sphobs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\spiral.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\stabiliz.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\steermas.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\vehicle.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\wander.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\include\mle\wraptest.h
# End Source File
# End Group
# End Target
# End Project
