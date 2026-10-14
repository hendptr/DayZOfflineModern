@echo off
setlocal
cd /d "%~dp0"
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0Play-Persistent.ps1" %*
if errorlevel 1 (
  echo.
  echo Persistent DayZ launcher could not complete. Review the message above.
  pause
  exit /b 1
)
endlocal
