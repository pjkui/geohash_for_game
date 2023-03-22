echo off
set "CMAKE=cmake"
set "WORKING_DIR=%~dp0%"
set "SLN_DIR=%WORKING_DIR%\sln"

if exist %SLN_DIR% rmdir /s/q %SLN_DIR%

if not exist %SLN_DIR% md %SLN_DIR%

%CMAKE% -S %WORKING_DIR% -B %SLN_DIR% -G "Visual Studio 16 2019" -A x64
if  %ERRORLEVEL% NEQ 0 pause
start %SLN_DIR%