@echo off
setlocal
cd /d "%~dp0"
echo Choose reset type:
echo 1. Character only (keep buildings and map loot)
echo 2. ALL (new character, remove placed objects, reset world loot)
choice /c 12 /n /m "Select 1 or 2: "
if errorlevel 2 (
  powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0Reset-Session.ps1" -Mode All
) else (
  powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0Reset-Session.ps1" -Mode Character
)
echo.
pause
