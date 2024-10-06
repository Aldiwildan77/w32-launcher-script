@echo OFF
rem Get the directory of the script
SET "SCRIPT_DIR=%~dp0"
SET "APP_PATH=%SCRIPT_DIR%app.exe"

rem Start app.exe minimized
start /MIN "" "%APP_PATH%" http -e app.cfg -w "web/dist"

exit