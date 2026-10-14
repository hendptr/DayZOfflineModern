# DayZ Solo Offline (DayZ 1.29)

Local **Chernarus singleplayer** mission for a legitimate DayZ installation, using the bundled `DayZDiag_x64.exe`. No dedicated server or Steam Workshop mod is required. Includes an in-game creator menu and local character/world-object saves, on top of Central Economy loot, infected and wildlife.

## Start

Double-click **`Play DayZ Offline.cmd`**, or run `./Play-Offline.ps1` in PowerShell. For convenience, the launcher is also available in the installed game directory as `C:\Program Files (x86)\Steam\steamapps\common\DayZ\Play DayZ Solo Offline.cmd`. It uses your DayZ 1.29 diagnostic client and the custom mission in this folder. Use `./Play-Offline.ps1 -DryRun` to inspect its launch configuration without opening the game.

Use `./Play-Offline.ps1 -GameDir 'D:\SteamLibrary\steamapps\common\DayZ'` if DayZ is elsewhere. Launch the script from the folder containing it; do not move only the `.cmd` file.

## Gameplay and data

This is a **diagnostic-client singleplayer mode**, so it may not behave exactly like the normal multiplayer game. It does not contain Arkensor's legacy toolbar or old script modules. The game launches without contacting a remote game server; Steam installation/authentication requirements may still apply.

By default, the player spawns near Chernogorsk at noon with basic clothes, a bandage, and food. Edit `missions/DayZSolo.ChernarusPlus/init.c` to customize the spawn, date, or starter gear.

## Creator tools (K) and teleport (O)

While in the game, click the game window to focus it, then press **K** to open the item browser (ESC or CLOSE closes it). F1–F12 (emotes) and HOME/END/INSERT (DayZDiag debug camera and tools) close menus, so the tools use K and O, which are unbound by default. If you rebind K or O in DayZ, change the keys in `SoloMission.OnKeyPress` in `init.c`. The first open takes a second or two while it reads every spawnable class from the game's config; after that it is instant. Because the list comes from the installed game, it always matches your DayZ version, so you never need to remember classnames.

1. **Pick a category** on the left: Weapons, Magazines, Ammo, Attachments, Melee & Tools, Tops, Pants, Shoes, Hats/Masks, Vests/Gloves, Backpacks, Food & Drink, Medical, Base/Storage, Vehicles, Zombies/Animals, World Objects, Other — or **All Items**.
2. **Search** (optional): type part of the name, e.g. `m4`, `stanag`, `556`, `ghillie`, `plate carrier`. Every word must match. CLEAR resets it.
3. **Click a row** to see a 3D preview. For a gun, it also shows which magazines and ammo it takes.
4. **Spawn:** **TO INVENTORY** (double-click a row does the same), **TO HANDS**, or **ON GROUND**. **AMOUNT** spawns several (max 50). Whatever doesn't fit in your inventory is dropped at your feet. Cars spawn 7 m ahead and AI 12 m ahead.

Special categories:

* **LOADOUT KITS:** a weapon with its magazines, ammo and optic in one click, plus complete outfits (military, Gorka, civilian, warm, NBC), medic supplies, food & water, survival tools, base-building tools, storage, and a ready-to-drive car. You can add your own kits in `SoloCatalog.BuildKits()` in `init.c`.
* **FITS MY GUN:** hold any firearm and press it. The list then shows only the magazines, ammo and attachments that fit that weapon.
* **KITTED: ON** (default): guns spawn with a loaded magazine and their parts, and cars spawn with wheels, doors, battery, plugs and fluids. Turn it off to spawn bare items.
* **PLACE + SAVE:** for World Objects (buildings and props). It places the object 4 m ahead and recreates it on every launch.

Cheats:

* **UNLIMITED AMMO:** the weapon in your hands is topped up 10× per second. This covers detachable magazines on every muzzle and internal magazines (shotguns, bolt-action rifles). The gun is kept at pristine condition, so it doesn't jam. You still need to load a magazine or round once.
* **REFILL ALL MAGS:** fills every magazine, ammo stack and gun you carry.
* **GOD MODE**, **HEAL / FOOD / WATER**, and **SAVE SESSION NOW**. Button labels show ON/OFF.

Teleport (press **O**, or the **TELEPORT (MAP)** button in the item menu):

* **Map:** drag to move, use the mouse wheel to zoom, and **double-click** any spot to teleport there. You land on the ground and your position is saved immediately.
* **GO TO MAP CENTER +:** teleports to the red **+** in the middle of the map. Use it if double-clicking doesn't respond.
* **Places list:** 140+ named Chernarus locations (towns, airfields, military bases, castles). Search by name, then double-click a place or press **GO TO PLACE**.
* Teleporting is blocked while you are in a vehicle.

## Persistence and reset

**Character saves happen every 20 seconds**, when you press Save Session, and on normal mission shutdown. The file is `%LOCALAPPDATA%\DayZ\DayZSolo\solo_player.json`; placed world objects live alongside it in `solo_buildings.json`. DayZ's Central Economy maintains loot/world state under the installed mission's `storage_-1` folder. On the next launch, the player resumes at the saved coordinates, with inventory, health, food/water and god/ammo toggles restored. The basic JSON saver is designed for normal item attachments and cargo, but may not restore every special state of all DayZ items or vehicles.

Close the game, then double-click **`Reset Session.cmd`** to select either **Character** (fresh survivor, keep your placed structures and world loot) or **All** (reset character, placed objects and world loot). The reset script requires you to type `RESET`, **backs up the selected save files** under `DayZOfflineModern/save-backups`, and then removes their active copies. It refuses to reset while DayZ is running. To restore a previous session, close DayZ first and copy the corresponding files or `storage_-1` from the named backup folder.

## Saved survival mode (optional)

To play with **persistent character, inventory, bases and world loot**, use the separate official **DayZ Server** tool as a private local server. Install it from Steam Library > Tools (or double-click **`Install DayZ Server.cmd`**). Then close the quick offline mission and double-click **`Play Saved DayZ.cmd`**. It creates a dedicated `dayzSolo.chernarusplus` mission copy within the installed server, starts the official server at UDP `2302`, waits for it to bind the port, and opens DayZ on `127.0.0.1:2302`.

This server uses the official Steam-hosted server build and is designed for local play. It may require Steam authentication even when connecting to localhost. Its default networking can still be accessible to devices on your LAN, depending on Windows Firewall; do not open or forward the DayZ server ports on your router for this solo setup. Save data lives in `C:\Program Files (x86)\Steam\steamapps\common\DayZServer\mpmissions\dayzSolo.chernarusplus\storage_15`. Back up that folder after stopping the server before changing DayZ versions. Server and client executable versions must match.

The server installation must be completed through your **signed-in Steam account**. SteamCMD anonymous installation of DayZ Server (app `223350`) can be refused; do not enter your Steam password into this repository or any launcher script. The persistent mode was prepared for when the official Steam tool is installed.

## Logs and troubleshooting

The DayZ client writes `.log`, `.RPT` and script-related files to `%LOCALAPPDATA%\DayZ`. Look for `[DayZSolo]` and any script compilation errors. A black screen or crash may indicate compatibility issues with the current DayZ build or GPU setup. The game is a native diagnostic tool, so compatibility after future DayZ updates is not guaranteed. Exit the game normally before changing mission scripts.

## Attribution and licenses

Central Economy world configuration data in `missions/DayZSolo.ChernarusPlus` is taken from [BohemiaInteractive/DayZ-Central-Economy](https://github.com/BohemiaInteractive/DayZ-Central-Economy), as of **2026-08-13**, under the [Arma and DayZ Public License Share Alike (ADPL-SA)](https://www.bohemia.net/community/licenses/arma-and-dayz-public-license-share-alike-adpl-sa). Used only with DayZ, for noncommercial play, with attribution. The mission entry point and launcher scripts are new and were written for this project.

For historical reference, the older [Arkensor DayZ Community Offline Mode](https://github.com/Arkensor/DayZCommunityOfflineMode) is checked out in `upstream-reference`, but is **not** loaded by this launcher.
