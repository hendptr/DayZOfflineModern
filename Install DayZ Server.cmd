@echo off
echo Opening the official DayZ Server installation in Steam.
echo Use your signed-in Steam account to finish installing the Tool.
powershell.exe -NoProfile -Command "Start-Process 'steam://install/223350'"
if errorlevel 1 pause
