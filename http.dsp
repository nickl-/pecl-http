# Microsoft Developer Studio Project File - Name="http" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=http - Win32 Release_TS
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "http.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "http.mak" CFG="http - Win32 Release_TS"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "http - Win32 Release_TS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "http - Win32 Debug_TS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "http - Win32 Release_TS"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release_TS"
# PROP BASE Intermediate_Dir "Release_TS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_TS"
# PROP Intermediate_Dir "Release_TS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\.." /I "..\..\..\Zend" /I "..\..\..\bindlib_w32" /I "..\..\..\TSRM" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMPILE_DL_HTTP" /D ZTS=1 /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\Zend" /I "..\..\..\bindlib_w32" /I "..\..\TSRM" /I "..\..\main" /I "..\..\..\php_build\curl\include" /D "WIN32" /D "HTTP_EXPORTS" /D "COMPILE_DL_HTTP" /D ZTS=1 /D HAVE_CURL=1 /D ZEND_DEBUG=0 /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ZEND_WIN32" /D "PHP_WIN32" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib php4ts.lib /nologo /dll /machine:I386
# ADD LINK32 php4ts.lib libcurl.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /dll /machine:I386 /nodefaultlib:"MSVCRT" /out:"..\..\Release_TS/php_http.dll" /libpath:"..\..\Release_TS" /libpath:"..\..\Release_TS_Inline" /libpath:"..\..\..\php_build\curl\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "http - Win32 Debug_TS"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Debug_TS"
# PROP BASE Intermediate_Dir "Debug_TS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Debug_TS"
# PROP Intermediate_Dir "Debug_TS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\.." /I "..\..\Zend" /I "..\..\..\bindlib_w32" /I "..\..\TSRM" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMPILE_DL_HTTP" /D ZTS=1 /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "..\.." /I "..\..\Zend" /I "..\..\..\bindlib_w32" /I "..\..\TSRM" /I "..\..\main" /I "..\..\..\php_build\curl\include" /D ZEND_DEBUG=1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HTTP_EXPORTS" /D "COMPILE_DL_HTTP" /D ZTS=1 /D "ZEND_WIN32" /D "PHP_WIN32" /D HAVE_CURL=1 /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib php4ts.lib /nologo /dll /machine:I386
# ADD LINK32 php4ts_debug.lib libcurl.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib wsock32.lib /nologo /dll /incremental:yes /debug /machine:I386 /nodefaultlib:"MSVCRTD" /out:"..\..\Debug_TS/php_http.dll" /libpath:"..\..\Debug_TS" /libpath:"..\..\..\php_build\curl\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF

# Begin Target

# Name "http - Win32 Release_TS"
# Name "http - Win32 Debug_TS"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\http.c
# End Source File
# Begin Source File

SOURCE=.\http_api.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\php_http.h
# End Source File
# Begin Source File

SOURCE=.\php_http_api.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
