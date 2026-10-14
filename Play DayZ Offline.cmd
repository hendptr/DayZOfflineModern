@echo off
setlocal
cd /d "%~dp0"
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0Play-Offline.ps1" %*
if errorlevel 1 (
  echo.
  echo Offline launcher reported a problem. Review the text above.
  pause
  exit /b 1
)
endlocal
