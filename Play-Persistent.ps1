param(
    [switch]$DryRun,
    [switch]$ServerOnly,
    [string]$GameDir = 'C:\Program Files (x86)\Steam\steamapps\common\DayZ',
    [string]$ServerDir = 'C:\Program Files (x86)\Steam\steamapps\common\DayZServer'
)

$ErrorActionPreference = 'Stop'
$serverExe = Join-Path $ServerDir 'DayZServer_x64.exe'
$clientExe = Join-Path $GameDir 'DayZDiag_x64.exe'
$missionTemplate = Join-Path $ServerDir 'mpmissions\dayzOffline.chernarusplus'
$soloMission = Join-Path $ServerDir 'mpmissions\dayzSolo.chernarusplus'
$config = Join-Path $ServerDir 'DayZSolo.serverDZ.cfg'
$sourceConfig = Join-Path $PSScriptRoot 'DayZSolo.serverDZ.cfg'
$profileDir = Join-Path $PSScriptRoot 'persistent-profiles'
$gamePort = 2302

Write-Host 'DayZ Solo - Saved Local Survival' -ForegroundColor Cyan
Write-Host "Server:  $serverExe"
Write-Host "Client:  $clientExe"
Write-Host "Mission: $soloMission"

if ($DryRun) {
    Write-Host "Server command: $serverExe -config=DayZSolo.serverDZ.cfg -port=$gamePort -profiles=$profileDir -limitFPS=100 -doLogs"
    Write-Host "Client command: $clientExe -connect=127.0.0.1 -port=$gamePort -nosplash -doLogs"
    Write-Host ("Server installed: " + (Test-Path -LiteralPath $serverExe -PathType Leaf))
    Write-Host 'Dry run: no files changed and no game processes started.' -ForegroundColor Yellow
    return
}

if (-not (Test-Path -LiteralPath $serverExe -PathType Leaf)) {
    throw "DayZ Server is not installed. In Steam Library > Tools, install 'DayZ Server' (app 223350), then launch this script again. You can also double-click 'Install DayZ Server.cmd' here to open the Steam install dialog."
}
if (-not (Test-Path -LiteralPath $clientExe -PathType Leaf)) {
    throw "DayZ diagnostic client missing: $clientExe"
}
if (-not (Test-Path -LiteralPath $missionTemplate -PathType Container)) {
    throw "DayZ Server does not contain official Chernarus mission: $missionTemplate"
}
if (-not (Test-Path -LiteralPath $sourceConfig -PathType Leaf)) {
    throw "Server config template missing: $sourceConfig"
}

if (@(Get-Process -Name 'DayZServer_x64' -ErrorAction SilentlyContinue).Count -gt 0) {
    throw 'A DayZ dedicated server is already running. Close it before starting a local singleplayer server.'
}
if (-not $ServerOnly -and @(Get-Process -Name 'DayZDiag_x64' -ErrorAction SilentlyContinue).Count -gt 0) {
    throw 'DayZ diagnostic client is running. Close your current DayZ game before switching to saved mode.'
}

# Mission files and storage are isolated from Steam's default server mission.
# Never overwrite an existing save when relaunching.
if (-not (Test-Path -LiteralPath $soloMission -PathType Container)) {
    Copy-Item -LiteralPath $missionTemplate -Destination $soloMission -Recurse -Force
    Write-Host 'Created a separate Chernarus mission for save data.'
}
if (-not (Test-Path -LiteralPath $config -PathType Leaf)) {
    Copy-Item -LiteralPath $sourceConfig -Destination $config
}
New-Item -ItemType Directory -Path $profileDir -Force | Out-Null

if (@(Get-NetUDPEndpoint -LocalPort $gamePort -ErrorAction SilentlyContinue).Count -gt 0) {
    throw "UDP port $gamePort is already in use. Close the other server first."
}

$serverArgs = "-config=DayZSolo.serverDZ.cfg -port=$gamePort -profiles=$profileDir -limitFPS=100 -doLogs"
$serverProcess = Start-Process -FilePath $serverExe -WorkingDirectory $ServerDir -ArgumentList $serverArgs -PassThru
Write-Host "Started local DayZ Server (PID $($serverProcess.Id)). Waiting for UDP port $gamePort ..."

$ready = $false
for ($i = 0; $i -lt 45; $i++) {
    Start-Sleep -Seconds 2
    $serverProcess.Refresh()
    if ($serverProcess.HasExited) {
        throw "DayZ Server exited with code $($serverProcess.ExitCode). Check logs in $profileDir."
    }
    if (@(Get-NetUDPEndpoint -LocalPort $gamePort -ErrorAction SilentlyContinue).Count -gt 0) {
        $ready = $true
        break
    }
}
if (-not $ready) {
    throw "Server did not bind UDP port $gamePort within 90 seconds. It may still be loading; inspect $profileDir."
}

Write-Host 'Local server UDP socket is ready.' -ForegroundColor Green
if ($ServerOnly) {
    Write-Host 'Server-only mode; connect to 127.0.0.1:2302 from DayZ.'
    return
}

$clientArgs = "-connect=127.0.0.1 -port=$gamePort -nosplash -noPause -doLogs"
$clientProcess = Start-Process -FilePath $clientExe -WorkingDirectory $GameDir -ArgumentList $clientArgs -PassThru
Write-Host "Started local DayZ client (PID $($clientProcess.Id))."
Write-Host 'Your survivor and world data live in the server\mpmissions\dayzSolo.chernarusplus storage folder.'
Write-Host 'Close the game, then allow the server to save before closing the server process.'
