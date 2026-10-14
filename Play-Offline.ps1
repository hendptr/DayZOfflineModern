param(
    [switch]$DryRun,
    [string]$GameDir = 'C:\Program Files (x86)\Steam\steamapps\common\DayZ'
)

$ErrorActionPreference = 'Stop'
$missionDir = Join-Path $PSScriptRoot 'missions\DayZSolo.ChernarusPlus'
$gameExe = Join-Path $GameDir 'DayZDiag_x64.exe'
$deployedMission = Join-Path $GameDir 'Missions\DayZSolo.ChernarusPlus'

if (-not (Test-Path -LiteralPath $gameExe -PathType Leaf)) {
    throw "DayZ diagnostic executable was not found: $gameExe"
}
if (-not (Test-Path -LiteralPath (Join-Path $missionDir 'init.c') -PathType Leaf)) {
    throw "Offline mission entry point was not found: $missionDir"
}
if (-not (Test-Path -LiteralPath (Join-Path $missionDir 'db\types.xml') -PathType Leaf)) {
    throw "Official Central Economy files are missing in: $missionDir"
}

$arguments = '-mission=.\Missions\DayZSolo.ChernarusPlus -nosplash -noPause -noBenchmark -filePatching -doLogs -scriptDebug=true'
Write-Host 'DayZ Solo Offline / Chernarus' -ForegroundColor Cyan
Write-Host "Game version: $((Get-Item -LiteralPath $gameExe).VersionInfo.FileVersion)"
Write-Host "Mission:      $deployedMission"
Write-Host "Executable:   $gameExe"
Write-Host "Arguments:    $arguments"
Write-Host "Logs:         $env:LOCALAPPDATA\DayZ"

if ($DryRun) {
    Write-Host 'Dry run: no game process started.' -ForegroundColor Yellow
    return
}

$running = @(Get-Process -Name 'DayZDiag_x64' -ErrorAction SilentlyContinue)
if ($running.Count -gt 0) {
    throw 'DayZ diagnostic client is already running. Close it before launching another mission.'
}

# The GUI .layout asset needs a stable $CurrentDir: path inside the DayZ folder.
# On first run deploy the official economy files; on later runs only update code/UI,
# preserving storage_-1 and the two JSON save files.
if (-not (Test-Path -LiteralPath $deployedMission -PathType Container)) {
    New-Item -ItemType Directory -Path (Split-Path -Parent $deployedMission) -Force | Out-Null
    Copy-Item -LiteralPath $missionDir -Destination $deployedMission -Recurse
    Write-Host 'Installed local mission into DayZ Missions.'
}
else {
    Copy-Item -LiteralPath (Join-Path $missionDir 'init.c') -Destination (Join-Path $deployedMission 'init.c') -Force
    $uiSource = Join-Path $missionDir 'ui'
    $uiTarget = Join-Path $deployedMission 'ui'
    if (-not (Test-Path -LiteralPath $uiTarget)) {
        New-Item -ItemType Directory -Path $uiTarget -Force | Out-Null
    }
    Get-ChildItem -LiteralPath $uiSource -File | ForEach-Object {
        Copy-Item -LiteralPath $_.FullName -Destination (Join-Path $uiTarget $_.Name) -Force
    }
}

$process = Start-Process -FilePath $gameExe -WorkingDirectory $GameDir -ArgumentList $arguments -PassThru
Write-Host "Launched DayZ diagnostic client (PID $($process.Id))." -ForegroundColor Green
