# Microsoft Developer Studio Project File - Name="complier" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=complier - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "complier.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "complier.mak" CFG="complier - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "complier - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "complier - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "complier - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "complier - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "complier - Win32 Release"
# Name "complier - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "global"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\errorlist.cpp
# End Source File
# Begin Source File

SOURCE=.\errorlist.h
# End Source File
# Begin Source File

SOURCE=.\InsCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\InsCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Quadcode.cpp
# End Source File
# Begin Source File

SOURCE=.\Quadcode.h
# End Source File
# Begin Source File

SOURCE=.\SSTable.cpp
# End Source File
# Begin Source File

SOURCE=.\SSTable.h
# End Source File
# Begin Source File

SOURCE=.\symbol.cpp
# End Source File
# Begin Source File

SOURCE=.\symbol.h
# End Source File
# Begin Source File

SOURCE=.\SymbolTable.cpp
# End Source File
# Begin Source File

SOURCE=.\SymbolTable.h
# End Source File
# Begin Source File

SOURCE=.\topUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\topUnit.h
# End Source File
# Begin Source File

SOURCE=.\utility.cpp
# End Source File
# Begin Source File

SOURCE=.\utility.h
# End Source File
# End Group
# Begin Group "front"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\codeBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\codeBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\Parser.h
# End Source File
# Begin Source File

SOURCE=.\SyntaxTree.cpp
# End Source File
# Begin Source File

SOURCE=.\SyntaxTree.h
# End Source File
# Begin Source File

SOURCE=.\TLeaf.cpp
# End Source File
# Begin Source File

SOURCE=.\TLeaf.h
# End Source File
# Begin Source File

SOURCE=.\TNode.cpp
# End Source File
# Begin Source File

SOURCE=.\TNode.h
# End Source File
# Begin Source File

SOURCE=.\token.cpp
# End Source File
# Begin Source File

SOURCE=.\token.h
# End Source File
# Begin Source File

SOURCE=.\tokenizer.cpp
# End Source File
# Begin Source File

SOURCE=.\tokenizer.h
# End Source File
# End Group
# Begin Group "middle"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\BBlock.h
# End Source File
# Begin Source File

SOURCE=.\Dag.cpp
# End Source File
# Begin Source File

SOURCE=.\Dag.h
# End Source File
# Begin Source File

SOURCE=.\Data_stream.cpp
# End Source File
# Begin Source File

SOURCE=.\Data_stream.h
# End Source File
# Begin Source File

SOURCE=.\FBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\FBlock.h
# End Source File
# End Group
# Begin Group "end"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\asm_translator.cpp
# End Source File
# Begin Source File

SOURCE=.\asm_translator.h
# End Source File
# Begin Source File

SOURCE=.\AsmCode.cpp
# End Source File
# Begin Source File

SOURCE=.\AsmCode.h
# End Source File
# Begin Source File

SOURCE=.\Mem_alloc.cpp
# End Source File
# Begin Source File

SOURCE=.\Mem_alloc.h
# End Source File
# Begin Source File

SOURCE=.\MemCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MemCtrl.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
