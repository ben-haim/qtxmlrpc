@echo off

IF (%1)==() GOTO error
dir /b /ad %1 >nul 2>nul && GOTO indentDir
IF NOT EXIST %1 GOTO error
goto indentFile

:indentDir
set searchdir=%1

IF (%2)==() GOTO assignDefaultSuffix
set filesuffix=%2

GOTO run

:assignDefaultSuffix
::echo !!!!DEFAULT SUFFIX!!!
set filesuffix=*

:run
FOR /F "tokens=*" %%G IN ('DIR /B /S %searchdir%\*.%filesuffix%') DO (
echo Indenting file "%%G"
"D:/programms/UniversalIndentGUI_win32/indenters/greatcode.exe" -file-"%%G"

)
GOTO ende

:indentFile
echo Indenting one file %1
"D:/programms/UniversalIndentGUI_win32/indenters/greatcode.exe" -file-"%1"


GOTO ende

:error
echo .
echo ERROR: As parameter given directory or file does not exist!
echo Syntax is: call_GreatCode.bat dirname filesuffix
echo Syntax is: call_GreatCode.bat filename
echo Example: call_GreatCode.bat temp cpp
echo .

:ende
