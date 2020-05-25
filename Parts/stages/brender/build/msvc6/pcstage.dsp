# Microsoft Developer Studio Project File - Name="pcstage" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=pcstage - Win32 GDK Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pcstage.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pcstage.mak" CFG="pcstage - Win32 GDK Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pcstage - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "pcstage - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "pcstage - Win32 GDK Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "pcstage - Win32 Release"

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
# ADD CPP /nologo /Gz /Zp4 /W3 /GX /O2 /I "..\..\..\win32\include" /I "$(MLE_HOME)\Core\mlert\win32\include" /I "$(MLE_HOME)\include" /I "$(BRENDER_PATH)\..\INC" /D "MLE_NOT_DLL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=0 /D ML_FIXED_POINT=0 /YX /FD /c
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
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\brender	copy Release\pcstage.lib $(MLE_HOME)\lib\brender	xcopy /y ..\..\..\win32\include\mle\*.h $(MLE_HOME)\include\mle
# End Special Build Tool

!ELSEIF  "$(CFG)" == "pcstage - Win32 Debug"

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
# ADD CPP /nologo /Gz /Zp4 /W3 /Gm /GX /ZI /Od /I "..\..\..\win32\include" /I "$(MLE_HOME)\Core\mlert\win32\include" /I "$(MLE_HOME)\include" /I "$(BRENDER_PATH)\..\INC" /D "MLE_DEBUG" /D "MLE_NOT_DLL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=1 /D ML_FIXED_POINT=0 /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Debug/pcstaged.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\pcstaged.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\brender	copy Debug\pcstaged.lib $(MLE_HOME)\lib\brender	xcopy /y ..\..\..\win32\include\mle\*.h $(MLE_HOME)\include\mle
# End Special Build Tool

!ELSEIF  "$(CFG)" == "pcstage - Win32 GDK Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "pcstage___Win32_GDK_Debug"
# PROP BASE Intermediate_Dir "pcstage___Win32_GDK_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GdkDebug"
# PROP Intermediate_Dir "GdkDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /Zp4 /W3 /Gm /GX /ZI /Od /I "..\..\..\win32\include" /I "$(MLE_HOME)\Core\mlert\win32\include" /I "$(MLE_HOME)\include" /I "$(BRENDER_PATH)\..\INC" /D "MLE_DEBUG" /D "MLE_NOT_DLL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=1 /D ML_FIXED_POINT=0 /YX /FD /GZ /c
# ADD CPP /nologo /Gz /Zp4 /W3 /Gm /GX /ZI /Od /I "..\..\..\win32\include" /I "$(MLE_HOME)\Core\mlert\win32\include" /I "$(MLE_HOME)\include" /I "$(BRENDER_PATH)\..\INC" /D "_GDK_" /D "MLE_DEBUG" /D "MLE_NOT_DLL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D ML_MATH_DEBUG=1 /D ML_FIXED_POINT=0 /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"Debug/pcstaged.bsc"
# ADD BSC32 /nologo /o"GdkDebug/pcstagegd.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\pcstaged.lib"
# ADD LIB32 /nologo /out:"GdkDebug\pcstagegd.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\brender	copy GdkDebug\pcstagegd.lib $(MLE_HOME)\lib\brender	xcopy /y ..\..\..\win32\include\mle\*.h $(MLE_HOME)\include\mle
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "pcstage - Win32 Release"
# Name "pcstage - Win32 Debug"
# Name "pcstage - Win32 GDK Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\win32\src\comstg.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\win32\src\gdkpal.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\win32\src\pcpal.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\win32\src\pcstage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\win32\src\win32pal.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\win32\include\mle\comstg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\win32\include\mle\gdkpal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\win32\include\mle\pcpal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\win32\include\mle\pcstage.h
# End Source File
# Begin Source File

SOURCE=..\..\..\win32\include\mle\win32pal.h
# End Source File
# End Group
# End Target
# End Project
