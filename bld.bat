@echo off
SET PROGNAME=terr.exe
mkdir output 2>NUL:
rem call "c:\Program Files\PellesC\Bin\povars32.bat"
del %PROGNAME% 2>NUL:
dir %PROGNAME% >NUL: 2>NUL:
IF NOT ERRORLEVEL 1 GOTO ErrDel
echo compiling
pocc.exe -std:C11 -I..\mylibs -Tx86-coff -Ot -Ob1 -fp:precise -W1 -Gz -Ze -Go -DUNICODE -D_UNICODE main.c -Fo"output\main.obj"
porc.exe "main.rc" -Fo"output\main.res"
echo linking
polink.exe -subsystem:windows -machine:x86 kernel32.lib user32.lib gdi32.lib comctl32.lib comdlg32.lib advapi32.lib delayimp.lib shell32.lib -out:"%PROGNAME%" "output\main.obj" "output\main.res"
dir %PROGNAME%
goto TheEnd
:ErrDel
echo ERROR! Can't delete existing file %PROGNAME%
:TheEnd
