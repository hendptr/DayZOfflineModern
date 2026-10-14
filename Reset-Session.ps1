param(
    [ValidateSet('Character','All')]
    [string]$Mode = 'Character',
    [switch]$ConfirmReset,
    [string]$GameDir = 'C:\Program Files (x86)\Steam\steamapps\common\DayZ'
)

$ErrorActionPreference = 'Stop'
$missionDir = Join-Path $GameDir 'Missions\DayZSolo.ChernarusPlus'
$profileDir = Join-Path $env:LOCALAPPDATA 'DayZ\DayZSolo'
$backups = Join-Path $PSScriptRoot 'save-backups'

if (@(Get-Process -Name DayZDiag_x64,DayZ_x64,DayZServer_x64 -ErrorAction SilentlyContinue).Count -gt 0) {
    throw 'Close DayZ and the DayZ server before resetting the session, to avoid overwriting the reset with a new save.'
}
$targets = @((Join-Path $profileDir 'solo_player.json'))
if ($Mode -eq 'All') {
    $targets += (Join-Path $profileDir 'solo_buildings.json')
    $targets += (Join-Path $missionDir 'storage_-1')
}
$existing = @($targets | Where-Object {Test-Path -LiteralPath $_})
if ($existing.Count -eq 0) {
    Write-Host 'No matching session data to reset.'
    return
}

Write-Host "Reset mode: $Mode" -ForegroundColor Yellow
foreach ($target in $existing) {Write-Host "  $target"}
if (-not $ConfirmReset) {
    $reply = Read-Host 'Type RESET to back up these files and start fresh'
    if ($reply -cne 'RESET') {
        Write-Host 'Cancelled. Save data unchanged.'
        return
    }
}

$backupDir = Join-Path $backups (Get-Date -Format 'yyyyMMdd_HHmmss_fff')
New-Item -ItemType Directory -Path $backupDir -Force | Out-Null
foreach ($target in $existing) {
    $resolved = (Resolve-Path -LiteralPath $target).Path
    if (-not ($resolved.StartsWith($missionDir + '\',[StringComparison]::OrdinalIgnoreCase) -or $resolved.StartsWith($profileDir + '\',[StringComparison]::OrdinalIgnoreCase))) {
        throw "Refusing to reset a path outside the mission: $resolved"
    }
    Copy-Item -LiteralPath $resolved -Destination (Join-Path $backupDir (Split-Path -Leaf $resolved)) -Recurse -Force
    Remove-Item -LiteralPath $resolved -Recurse -Force
}
Write-Host "Session reset complete. Backup: $backupDir" -ForegroundColor Green
