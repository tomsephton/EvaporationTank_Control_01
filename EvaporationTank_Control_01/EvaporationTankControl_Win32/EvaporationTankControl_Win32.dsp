# Microsoft Developer Studio Project File - Name="EvaporationTankControl_Win32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=EvaporationTankControl_Win32 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EvaporationTankControl_Win32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EvaporationTankControl_Win32.mak" CFG="EvaporationTankControl_Win32 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EvaporationTankControl_Win32 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "EvaporationTankControl_Win32 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EvaporationTankControl_Win32 - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "EvaporationTankControl_Win32 - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "EvaporationTankControl_Win32 - Win32 Release"
# Name "EvaporationTankControl_Win32 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\AnalogInput.cpp
# End Source File
# Begin Source File

SOURCE=..\AnalogOutput.cpp
# End Source File
# Begin Source File

SOURCE=..\AnalogSensor.cpp
# End Source File
# Begin Source File

SOURCE=..\BinaryInput.cpp
# End Source File
# Begin Source File

SOURCE=..\BinaryOutput.cpp
# End Source File
# Begin Source File

SOURCE=..\BinarySensor.cpp
# End Source File
# Begin Source File

SOURCE=..\DaqDevice.cpp
# End Source File
# Begin Source File

SOURCE=..\DaqRoutines.cpp
# End Source File
# Begin Source File

SOURCE=..\DataBox.cpp
# End Source File
# Begin Source File

SOURCE=..\DBK15.cpp
# End Source File
# Begin Source File

SOURCE=..\DBK23.cpp
# End Source File
# Begin Source File

SOURCE=..\DBK25.cpp
# End Source File
# Begin Source File

SOURCE=..\DBK5.cpp
# End Source File
# Begin Source File

SOURCE=..\DistillateSupplyTank.cpp
# End Source File
# Begin Source File

SOURCE=..\EvaporationTank_Control_01.cpp
# End Source File
# Begin Source File

SOURCE=..\EvaporationTank_Control_01.rc
# End Source File
# Begin Source File

SOURCE=..\EvaporationTankA.cpp
# End Source File
# Begin Source File

SOURCE=..\EvaporationTankB.cpp
# End Source File
# Begin Source File

SOURCE=..\LevelSensor.cpp
# End Source File
# Begin Source File

SOURCE=..\LevelSwitch.cpp
# End Source File
# Begin Source File

SOURCE=..\LiquidTank.cpp
# End Source File
# Begin Source File

SOURCE=..\LogData.cpp
# End Source File
# Begin Source File

SOURCE=..\LogFileOutput.cpp
# End Source File
# Begin Source File

SOURCE=..\MathUtilities.cpp
# End Source File
# Begin Source File

SOURCE=..\MeteringPump.cpp
# End Source File
# Begin Source File

SOURCE=..\ProcessErrors.cpp
# End Source File
# Begin Source File

SOURCE=..\ProgramException.cpp
# End Source File
# Begin Source File

SOURCE=..\ProgramExceptions.cpp
# End Source File
# Begin Source File

SOURCE=..\PumpLogFileOutput.cpp
# End Source File
# Begin Source File

SOURCE=..\Pyranometer.cpp
# End Source File
# Begin Source File

SOURCE=..\Sensor.cpp
# End Source File
# Begin Source File

SOURCE=..\stdafx.cpp
# End Source File
# Begin Source File

SOURCE=..\SystemTime.cpp
# End Source File
# Begin Source File

SOURCE=..\TankFillLogFileOutput.cpp
# End Source File
# Begin Source File

SOURCE=..\TemperatureProbe.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\AnalogInput.h
# End Source File
# Begin Source File

SOURCE=..\AnalogOutput.h
# End Source File
# Begin Source File

SOURCE=..\AnalogSensor.h
# End Source File
# Begin Source File

SOURCE=..\BinaryInput.h
# End Source File
# Begin Source File

SOURCE=..\BinaryOutput.h
# End Source File
# Begin Source File

SOURCE=..\BinarySensor.h
# End Source File
# Begin Source File

SOURCE=..\DaqDevice.h
# End Source File
# Begin Source File

SOURCE=..\DaqRoutines.h
# End Source File
# Begin Source File

SOURCE=..\Daqx.h
# End Source File
# Begin Source File

SOURCE=..\DataBox.h
# End Source File
# Begin Source File

SOURCE=..\DBK15.h
# End Source File
# Begin Source File

SOURCE=..\DBK23.h
# End Source File
# Begin Source File

SOURCE=..\DBK25.h
# End Source File
# Begin Source File

SOURCE=..\DBK5.h
# End Source File
# Begin Source File

SOURCE=..\DistillateSupplyTank.h
# End Source File
# Begin Source File

SOURCE=..\EvaporationTank_Control_01.h
# End Source File
# Begin Source File

SOURCE=..\EvaporationTankA.h
# End Source File
# Begin Source File

SOURCE=..\EvaporationTankB.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\Gl\Glaux.h
# End Source File
# Begin Source File

SOURCE=..\LevelSensor.h
# End Source File
# Begin Source File

SOURCE=..\LevelSwitch.h
# End Source File
# Begin Source File

SOURCE=..\LiquidTank.h
# End Source File
# Begin Source File

SOURCE=..\LogData.h
# End Source File
# Begin Source File

SOURCE=..\LogFileOutput.h
# End Source File
# Begin Source File

SOURCE=..\MathUtilities.h
# End Source File
# Begin Source File

SOURCE=..\MeteringPump.h
# End Source File
# Begin Source File

SOURCE=..\ProcessErrors.h
# End Source File
# Begin Source File

SOURCE=..\ProgramException.h
# End Source File
# Begin Source File

SOURCE=..\ProgramExceptions.h
# End Source File
# Begin Source File

SOURCE=..\PumpLogFileOutput.h
# End Source File
# Begin Source File

SOURCE=..\Pyranometer.h
# End Source File
# Begin Source File

SOURCE=..\resource.h
# End Source File
# Begin Source File

SOURCE=..\Sensor.h
# End Source File
# Begin Source File

SOURCE=..\stdafx.h
# End Source File
# Begin Source File

SOURCE=..\SystemTime.h
# End Source File
# Begin Source File

SOURCE=..\TankFillLogFileOutput.h
# End Source File
# Begin Source File

SOURCE=..\targetver.h
# End Source File
# Begin Source File

SOURCE=..\TemperatureProbe.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\LIB\GLAUX.LIB
# End Source File
# Begin Source File

SOURCE=.\LIB\BCB5DaqX.lib
# End Source File
# Begin Source File

SOURCE=.\LIB\DAQX.lib
# End Source File
# End Target
# End Project
