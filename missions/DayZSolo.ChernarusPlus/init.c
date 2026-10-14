// DayZ Solo Tools: local-only Chernarus survival + creator menu for DayZ 1.29.
// Inventory save structure was informed by Arkensor's CC BY-NC-SA 4.0 offline mode.

static const int SOLO_MENU_ID = 927461;

class SoloItem
{
    string Type;
    int Placement; // 1 = cargo, 2 = attachment
    float Quantity;
    float Health;
    float Wet;
    int Hotbar;
    ref array<ref SoloItem> Cargo;
    ref array<ref SoloItem> SavedAttachments;

    void SoloItem()
    {
        Hotbar = -1;
        Cargo = new array<ref SoloItem>;
        SavedAttachments = new array<ref SoloItem>;
    }
}

class SoloPlayerData
{
    int Version;
    string Model;
    vector Position;
    vector Orientation;
    float Health;
    float Blood;
    float Energy;
    float Water;
    bool GodMode;
    bool UnlimitedAmmo;
    ref SoloItem SavedInventory;
    ref SoloItem Hands;

    void SoloPlayerData()
    {
        Version = 1;
        SavedInventory = new SoloItem;
    }
}

class SoloBuildObject
{
    string Type;
    vector Position;
    vector Orientation;
}

class SoloBuildData
{
    ref array<ref SoloBuildObject> Objects;

    void SoloBuildData()
    {
        Objects = new array<ref SoloBuildObject>;
    }
}

class SoloKit
{
    string Name;
    ref TStringArray Items; // "ClassName" or "ClassName:count"

    void SoloKit(string name)
    {
        Name = name;
        Items = new TStringArray;
    }
}

class SoloCatalogEntry
{
    string Type;
    string TypeLower;
    string Label;
    string Search;
    int Category;
    ref TStringArray Slots;
}

// Every spawnable class is read from the game config at runtime, so the list
// always matches the installed DayZ version (including DLC items).
class SoloCatalog
{
    static const int CAT_ALL = 0;
    static const int CAT_KITS = 1;
    static const int CAT_FOR_GUN = 2;
    static const int CAT_WEAPONS = 3;
    static const int CAT_MAGS = 4;
    static const int CAT_AMMO = 5;
    static const int CAT_ATTACH = 6;
    static const int CAT_TOOLS = 7;
    static const int CAT_TOPS = 8;
    static const int CAT_PANTS = 9;
    static const int CAT_SHOES = 10;
    static const int CAT_HEAD = 11;
    static const int CAT_VESTS = 12;
    static const int CAT_BACKPACKS = 13;
    static const int CAT_FOOD = 14;
    static const int CAT_MEDICAL = 15;
    static const int CAT_BASE = 16;
    static const int CAT_VEHICLES = 17;
    static const int CAT_AI = 18;
    static const int CAT_WORLD = 19;
    static const int CAT_OTHER = 20;
    static const int CAT_COUNT = 21;

    static ref array<ref SoloCatalogEntry> Entries;
    static ref array<ref SoloKit> Kits;
    static ref TStringArray Names;

    static string CategoryName(int category)
    {
        if (!Names)
        {
            Names = new TStringArray;
            Names.Insert("All items (+ kits)");
            Names.Insert("Loadout kits");
            Names.Insert("Fits the gun in your hands");
            Names.Insert("Weapons");
            Names.Insert("Magazines");
            Names.Insert("Ammo");
            Names.Insert("Weapon attachments / optics");
            Names.Insert("Melee & tools");
            Names.Insert("Tops & jackets");
            Names.Insert("Pants");
            Names.Insert("Shoes & boots");
            Names.Insert("Hats, helmets, masks & glasses");
            Names.Insert("Vests, gloves, belts & armbands");
            Names.Insert("Backpacks");
            Names.Insert("Food & drink");
            Names.Insert("Medical");
            Names.Insert("Base building & storage");
            Names.Insert("Vehicles & car parts");
            Names.Insert("Zombies & animals");
            Names.Insert("World objects (buildings, props)");
            Names.Insert("Other");
        }
        if (category < 0 || category >= Names.Count())
            return "Other";
        return Names[category];
    }

    // items: comma-separated classnames, "Class:3" spawns three.
    static void AddKit(string name, string items)
    {
        SoloKit kit = new SoloKit(name);
        TStringArray parts = new TStringArray;
        items.Split(",", parts);
        foreach (string part : parts)
        {
            string item = part.Trim();
            if (item != "")
                kit.Items.Insert(item);
        }
        Kits.Insert(kit);
    }

    // Add your own kits here; any classname from the K menu list works.
    static void BuildKits()
    {
        Kits = new array<ref SoloKit>;
        AddKit("M4A1 assault rifle", "M4A1, Mag_STANAG_30Rnd:4, Ammo_556x45:2, ACOGOptic, M4_Suppressor");
        AddKit("AKM rifle", "AKM, Mag_AKM_30Rnd:4, Ammo_762x39:2, KashtanOptic");
        AddKit("SVD sniper + ghillie", "SVD, Mag_SVD_10Rnd:4, Ammo_762x54:2, PSO1Optic, GhillieSuit_Woodland, GhillieHood_Woodland");
        AddKit("VSS silent rifle", "VSS, Mag_VSS_10Rnd:4, Ammo_9x39:3");
        AddKit("Winchester 70 hunting rifle", "Winchester70, Ammo_308Win:3, HuntingOptic");
        AddKit("Mosin 91/30 + PU scope", "Mosin9130, Ammo_762x54:3, PUScopeOptic");
        AddKit("MP-133 shotgun", "Mp133Shotgun, Ammo_12gaPellets:4");
        AddKit("FNX45 pistol + holster", "FNX45, Mag_FNX45_15Rnd:3, Ammo_45ACP:2, ChestHolster");
        AddKit("Glock 19 pistol", "Glock19, Mag_Glock_15Rnd:3, Ammo_9x19:2");
        AddKit("Grenades", "M67Grenade:2, RGD5Grenade:2");
        AddKit("Outfit: military (USMC)", "USMCJacket_Woodland, USMCPants_Woodland, CombatBoots_Black, Mich2001Helmet, TacticalGloves_Green, PlateCarrierVest, AssaultBag_Ttsko, Balaclava3Holes_Black");
        AddKit("Outfit: Gorka", "GorkaEJacket_Summer, GorkaPants_Summer, MilitaryBoots_Black, BallisticHelmet_Green, SmershVest, AliceBag_Camo");
        AddKit("Outfit: civilian", "Hoodie_Blue, Jeans_Blue, AthleticShoes_Blue, BaseballCap_Black, TaloonBag_Blue");
        AddKit("Outfit: warm coat", "WoolCoat_Black, CargoPants_Blue, HikingBootsLow_Black, MountainBag_Green, HighCapacityVest_Black");
        AddKit("Outfit: NBC hazmat", "NBCJacketGray, NBCPantsGray, NBCBootsGray, NBCGlovesGray, NBCHoodGray, GasMask");
        AddKit("Medic supplies", "FirstAidKit, BandageDressing:4, Morphine, Epinephrine, SalineBagIV, StartKitIV, BloodTestKit, TetracyclineAntibiotics, PainkillerTablets, CharcoalTablets, VitaminBottle, DisinfectantSpray");
        AddKit("Food & water", "Canteen, WaterBottle, TacticalBaconCan:2, BakedBeansCan:2, PeachesCan, SardinesCan, CanOpener, SodaCan_Cola:2, Rice, PurificationTablets");
        AddKit("Survival tools", "HuntingKnife, Hatchet, Matchbox, Flashlight, Battery9V:2, Compass, Binoculars, ChernarusMap, Heatpack:2, Chemlight_Green, SewingKit, LeatherSewingKit, WeaponCleaningKit, DuctTape");
        AddKit("Melee weapons", "Machete, FirefighterAxe, CombatKnife");
        AddKit("Base builder", "Hatchet, Hammer, Shovel, Pliers, Hacksaw, SledgeHammer, NailBox:3, MetalWire, Rope, WoodenPlank:2, FenceKit, WatchtowerKit, CombinationLock");
        AddKit("Base storage (drops on ground)", "SeaChest, WoodenCrate:2, Barrel_Green, MediumTent");
        AddKit("Car: Offroad Hatchback + spares", "OffroadHatchback, CanisterGasoline, TireRepairKit, CarBattery, SparkPlug");
    }

    static bool HasAny(string lower, TStringArray words)
    {
        foreach (string word : words)
        {
            if (lower.Contains(word))
                return true;
        }
        return false;
    }

    static bool HasSlot(TStringArray slots, string slot)
    {
        return slots.Find(slot) != -1;
    }

    static void LowerAll(TStringArray values)
    {
        for (int i = 0; i < values.Count(); i++)
        {
            string value = values[i];
            value.ToLower();
            values[i] = value;
        }
    }

    static void ReadSlots(string path, TStringArray slots)
    {
        GetGame().ConfigGetTextArray(path + " inventorySlot", slots);
        if (slots.Count() == 0)
        {
            string single = "";
            GetGame().ConfigGetText(path + " inventorySlot", single);
            if (single != "")
                slots.Insert(single);
        }
        LowerAll(slots);
    }

    static int Categorize(string root, string name, string lower, TStringArray slots)
    {
        CGame game = GetGame();
        if (game.IsKindOf(name, "Weapon_Base"))
            return CAT_WEAPONS;
        if (game.IsKindOf(name, "Ammunition_Base"))
            return CAT_AMMO;
        if (root == CFG_MAGAZINESPATH)
        {
            if (lower.IndexOf("ammo_") == 0)
                return CAT_AMMO;
            return CAT_MAGS;
        }
        if (game.IsKindOf(name, "Transport"))
            return CAT_VEHICLES;
        if (game.IsKindOf(name, "DZ_LightAI"))
            return CAT_AI;
        if (game.IsKindOf(name, "House") || lower.IndexOf("land_") == 0 || lower.IndexOf("static_") == 0)
            return CAT_WORLD;

        if (game.IsKindOf(name, "Clothing_Base") || game.IsKindOf(name, "Clothing"))
        {
            if (HasSlot(slots, "body"))
                return CAT_TOPS;
            if (HasSlot(slots, "legs"))
                return CAT_PANTS;
            if (HasSlot(slots, "feet"))
                return CAT_SHOES;
            if (HasSlot(slots, "headgear") || HasSlot(slots, "mask") || HasSlot(slots, "eyewear"))
                return CAT_HEAD;
            if (HasSlot(slots, "back"))
                return CAT_BACKPACKS;
            if (HasSlot(slots, "vest") || HasSlot(slots, "hips") || HasSlot(slots, "gloves") || HasSlot(slots, "armband"))
                return CAT_VESTS;
        }

        foreach (string slot : slots)
        {
            if (slot.Contains("weapon") || slot.Contains("pistol"))
                return CAT_ATTACH;
        }

        TStringArray medical = {"bandage", "morphine", "epinephrine", "saline", "bloodbag", "bloodtest", "tablets", "antibiotic", "vitamin", "disinfect", "iodine", "splint", "defibrillator", "antidote", "firstaid", "startkitiv", "thermometer", "syringe", "tincture", "antichem", "antipox"};
        if (HasAny(lower, medical))
            return CAT_MEDICAL;

        if (game.IsKindOf(name, "Edible_Base") || game.IsKindOf(name, "Bottle_Base"))
            return CAT_FOOD;

        TStringArray carParts = {"carbattery", "truckbattery", "sparkplug", "glowplug", "radiator", "headlight", "hood", "trunk", "tirerepair", "canistergasoline", "wheel", "door"};
        if (game.IsKindOf(name, "CarWheel") || game.IsKindOf(name, "CarDoor") || HasAny(lower, carParts))
            return CAT_VEHICLES;

        TStringArray baseWords = {"nail", "plank", "combinationlock", "barbedwire", "camonet", "metalplate", "fencekit", "watchtowerkit", "shelterkit", "flagkit", "flag_", "tent", "barrel", "chest", "crate", "firewood", "woodenlog", "fireplace", "stash"};
        if (game.IsKindOf(name, "Container_Base") || game.IsKindOf(name, "TentBase") || game.IsKindOf(name, "KitBase") || game.IsKindOf(name, "BaseBuildingBase") || HasAny(lower, baseWords))
            return CAT_BASE;

        TStringArray toolWords = {"knife", "axe", "saw", "hammer", "shovel", "pliers", "wrench", "screwdriver", "pickaxe", "sickle", "machete", "crowbar", "lockpick", "flashlight", "headtorch", "compass", "binocular", "map", "matchbox", "lighter", "kit", "rope", "battery", "baseballbat", "sword", "pipe", "cleaver", "hoe", "pitchfork", "shears", "mace", "sledge", "chainsaw", "torch", "rangefinder", "radio", "megaphone", "fishing", "hook", "spear"};
        if (game.IsKindOf(name, "ToolBase") || HasSlot(slots, "melee") || HasSlot(slots, "shoulder") || HasAny(lower, toolWords))
            return CAT_TOOLS;

        return CAT_OTHER;
    }

    static void Build()
    {
        if (Entries)
            return;
        BuildKits();
        Entries = new array<ref SoloCatalogEntry>;
        array<ref SoloCatalogEntry> unsorted = new array<ref SoloCatalogEntry>;
        TStringArray sortKeys = new TStringArray;
        TStringArray roots = {CFG_WEAPONSPATH, CFG_MAGAZINESPATH, CFG_VEHICLESPATH};

        foreach (string root : roots)
        {
            int childCount = GetGame().ConfigGetChildrenCount(root);
            for (int i = 0; i < childCount; i++)
            {
                string name = "";
                GetGame().ConfigGetChildName(root, i, name);
                if (name == "")
                    continue;
                string path = root + " " + name;
                if (GetGame().ConfigGetInt(path + " scope") != 2)
                    continue;
                string lower = name;
                lower.ToLower();
                if (lower.Contains("_base") || lower.Contains("gp25") || lower.Contains("m203") || lower.Contains("debug"))
                    continue;

                SoloCatalogEntry entry = new SoloCatalogEntry;
                entry.Type = name;
                entry.TypeLower = lower;
                entry.Slots = new TStringArray;
                ReadSlots(path, entry.Slots);
                entry.Category = Categorize(root, name, lower, entry.Slots);

                string display = "";
                GetGame().ConfigGetText(path + " displayName", display);
                if (display == "" || display.IndexOf("$") == 0 || display.IndexOf("#") == 0)
                    entry.Label = name;
                else
                    entry.Label = display + "   [" + name + "]";
                string searchText = display + " " + name;
                searchText.ToLower();
                entry.Search = searchText;

                string sortKey = entry.Label;
                sortKey.ToLower();
                sortKeys.Insert(sortKey + "|" + unsorted.Count().ToString());
                unsorted.Insert(entry);
            }
        }

        sortKeys.Sort();
        foreach (string key : sortKeys)
        {
            int bar = key.LastIndexOf("|");
            int index = key.Substring(bar + 1, key.Length() - bar - 1).ToInt();
            Entries.Insert(unsorted[index]);
        }
        Print("[DayZSolo] Item catalog built: " + Entries.Count() + " spawnable classes");
    }

    // Fills mags/rounds that fit the weapon; returns them as a lower-case "|a|b|" lookup string.
    static string GunAmmoTypes(string gunType, TStringArray mags, TStringArray rounds)
    {
        string path = CFG_WEAPONSPATH + " " + gunType;
        GetGame().ConfigGetTextArray(path + " magazines", mags);
        GetGame().ConfigGetTextArray(path + " chamberableFrom", rounds);
        foreach (string magName : mags)
        {
            TStringArray magAmmo = new TStringArray;
            GetGame().ConfigGetTextArray(CFG_MAGAZINESPATH + " " + magName + " ammoItems", magAmmo);
            foreach (string ammoName : magAmmo)
            {
                if (rounds.Find(ammoName) == -1)
                    rounds.Insert(ammoName);
            }
        }
        string found = "|";
        foreach (string m : mags)
        {
            string ml = m;
            ml.ToLower();
            found += ml + "|";
        }
        foreach (string r : rounds)
        {
            string rl = r;
            rl.ToLower();
            found += rl + "|";
        }
        return found;
    }

    static void GunSlots(string gunType, TStringArray slots)
    {
        GetGame().ConfigGetTextArray(CFG_WEAPONSPATH + " " + gunType + " attachments", slots);
        LowerAll(slots);
    }

    static string JoinFirst(TStringArray values, int limit)
    {
        string text = "";
        for (int i = 0; i < values.Count() && i < limit; i++)
        {
            if (i > 0)
                text += ", ";
            text += values[i];
        }
        if (values.Count() > limit)
            text += " (+" + (values.Count() - limit).ToString() + " more)";
        if (text == "")
            text = "-";
        return text;
    }
}

class SoloTools
{
    static const int SPAWN_INVENTORY = 0;
    static const int SPAWN_HANDS = 1;
    static const int SPAWN_GROUND = 2;

    static bool GodMode;
    static bool UnlimitedAmmo;
    static bool KittedSpawn = true;
    static ref SoloBuildData Buildings;
    static ref map<string, string> BulletCache;

    static string SaveFolder()
    {
        return "$profile:DayZSolo";
    }

    static string SavePath()
    {
        return SaveFolder() + "\\solo_player.json";
    }

    static string BuildPath()
    {
        return SaveFolder() + "\\solo_buildings.json";
    }

    static SoloItem Capture(EntityAI item, PlayerBase player, int placement = 0)
    {
        SoloItem data = new SoloItem;
        data.Type = item.GetType();
        data.Placement = placement;

        ItemBase itemBase = ItemBase.Cast(item);
        if (itemBase)
        {
            data.Health = itemBase.GetHealth("", "");
            data.Wet = itemBase.GetWet();
            data.Quantity = itemBase.GetQuantity();
            Magazine mag = Magazine.Cast(item);
            if (mag)
                data.Quantity = mag.GetAmmoCount();
            data.Hotbar = player.FindQuickBarEntityIndex(itemBase);
        }

        for (int a = 0; a < item.GetInventory().AttachmentCount(); a++)
        {
            EntityAI attachment = item.GetInventory().GetAttachmentFromIndex(a);
            if (attachment)
                data.SavedAttachments.Insert(Capture(attachment, player, 2));
        }

        CargoBase cargo = item.GetInventory().GetCargo();
        if (cargo)
        {
            for (int c = 0; c < cargo.GetItemCount(); c++)
            {
                EntityAI contents = cargo.GetItem(c);
                if (contents)
                    data.Cargo.Insert(Capture(contents, player, 1));
            }
        }
        return data;
    }

    static void Populate(EntityAI parent, PlayerBase player, SoloItem data)
    {
        if (!parent || !data)
            return;
        foreach (SoloItem attachmentData : data.SavedAttachments)
            Restore(parent, player, attachmentData);
        foreach (SoloItem cargoData : data.Cargo)
            Restore(parent, player, cargoData);
    }

    static EntityAI Restore(EntityAI parent, PlayerBase player, SoloItem data)
    {
        if (!parent || !data || data.Type == "")
            return NULL;

        EntityAI created;
        if (data.Placement == 2)
            created = parent.GetInventory().CreateAttachment(data.Type);
        else
            created = parent.GetInventory().CreateInInventory(data.Type);

        if (!created)
        {
            Print("[DayZSolo] Save restore skipped: " + data.Type);
            return NULL;
        }

        ItemBase item = ItemBase.Cast(created);
        if (item)
        {
            if (data.Health > 0)
                item.SetHealth("", "", data.Health);
            item.SetWet(data.Wet);
            Magazine mag = Magazine.Cast(item);
            if (mag)
                mag.LocalSetAmmoCount(data.Quantity);
            else if (data.Quantity > 0)
                item.SetQuantity(data.Quantity);
            if (data.Hotbar >= 0)
                player.SetQuickBarEntityShortcut(item, data.Hotbar);
        }
        Populate(created, player, data);
        return created;
    }

    static bool LoadPlayer(PlayerBase player)
    {
        if (!FileExist(SavePath()))
            return false;
        SoloPlayerData data = new SoloPlayerData;
        JsonFileLoader<SoloPlayerData>.JsonLoadFile(SavePath(), data);
        if (data.Version != 1 || !data.SavedInventory)
        {
            Print("[DayZSolo] Saved character version invalid; spawning new survivor");
            return false;
        }

        player.SetPosition(data.Position);
        player.SetOrientation(data.Orientation);
        if (data.Health > 0)
            player.SetHealth("", "", data.Health);
        if (data.Blood > 0)
            player.SetHealth("", "Blood", data.Blood);
        player.GetStatEnergy().Set(data.Energy);
        player.GetStatWater().Set(data.Water);
        Populate(player, player, data.SavedInventory);

        if (data.Hands && data.Hands.Type != "")
        {
            SoloItem hand = data.Hands;
            hand.Placement = 1;
            EntityAI handItem = Restore(player, player, hand);
            if (handItem)
                player.LocalTakeEntityToHands(handItem);
        }

        GodMode = data.GodMode;
        UnlimitedAmmo = data.UnlimitedAmmo;
        player.SetAllowDamage(!GodMode);
        Print("[DayZSolo] Loaded saved player and inventory");
        return true;
    }

    static void SavePlayer(PlayerBase player)
    {
        if (!player)
            return;
        SoloPlayerData data = new SoloPlayerData;
        data.Model = player.GetType();
        data.Position = player.GetPosition();
        data.Orientation = player.GetOrientation();
        data.Health = player.GetHealth("", "");
        data.Blood = player.GetHealth("", "Blood");
        data.Energy = player.GetStatEnergy().Get();
        data.Water = player.GetStatWater().Get();
        data.GodMode = GodMode;
        data.UnlimitedAmmo = UnlimitedAmmo;
        data.SavedInventory = Capture(player, player);
        EntityAI hands = player.GetHumanInventory().GetEntityInHands();
        if (hands)
            data.Hands = Capture(hands, player);
        JsonFileLoader<SoloPlayerData>.JsonSaveFile(SavePath(), data);
        Print("[DayZSolo] Character save exists=" + FileExist(SavePath()) + " path=" + SavePath());
    }

    static void LoadBuildings()
    {
        Buildings = new SoloBuildData;
        if (!FileExist(BuildPath()))
            return;
        JsonFileLoader<SoloBuildData>.JsonLoadFile(BuildPath(), Buildings);
        if (!Buildings || !Buildings.Objects)
        {
            Buildings = new SoloBuildData;
            return;
        }
        foreach (SoloBuildObject data : Buildings.Objects)
        {
            Object obj = GetGame().CreateObject(data.Type, data.Position, false, false, true);
            if (obj)
            {
                obj.SetPosition(data.Position);
                obj.SetOrientation(data.Orientation);
            }
        }
        Print("[DayZSolo] Restored placed objects: " + Buildings.Objects.Count());
    }

    static void SaveBuildings()
    {
        if (Buildings)
            JsonFileLoader<SoloBuildData>.JsonSaveFile(BuildPath(), Buildings);
    }

    static void CreateBuilding(string type, PlayerBase player)
    {
        if (!player || type == "")
            return;
        vector position = player.GetPosition() + player.GetDirection() * 4;
        position[1] = GetGame().SurfaceY(position[0], position[2]);
        Object obj = GetGame().CreateObject(type, position, false, false, true);
        if (!obj)
        {
            player.MessageStatus("Cannot spawn world class: " + type);
            return;
        }
        obj.SetPosition(position);
        vector facing = player.GetOrientation();
        obj.SetOrientation(facing);
        if (!Buildings)
            Buildings = new SoloBuildData;
        SoloBuildObject entry = new SoloBuildObject;
        entry.Type = type;
        entry.Position = position;
        entry.Orientation = facing;
        Buildings.Objects.Insert(entry);
        SaveBuildings();
        player.MessageStatus("Placed " + type + " (saved)");
    }

    static bool IsKnownClass(string type)
    {
        if (type == "")
            return false;
        CGame game = GetGame();
        return game.ConfigIsExisting(CFG_VEHICLESPATH + " " + type) || game.ConfigIsExisting(CFG_WEAPONSPATH + " " + type) || game.ConfigIsExisting(CFG_MAGAZINESPATH + " " + type);
    }

    // Cars, AI and buildings can never go into the inventory.
    static bool IsWorldOnly(string type)
    {
        CGame game = GetGame();
        return game.IsKindOf(type, "Transport") || game.IsKindOf(type, "DZ_LightAI") || game.IsKindOf(type, "House");
    }

    static EntityAI SpawnNear(string type, PlayerBase player)
    {
        bool isAI = GetGame().IsKindOf(type, "DZ_LightAI");
        float distance = 1.5;
        if (GetGame().IsKindOf(type, "Transport"))
            distance = 7;
        else if (isAI)
            distance = 12;
        vector position = player.GetPosition() + player.GetDirection() * distance;
        position[1] = GetGame().SurfaceY(position[0], position[2]);
        Object obj = GetGame().CreateObject(type, position, false, isAI, true);
        if (obj && !isAI)
            obj.PlaceOnSurface();
        return EntityAI.Cast(obj);
    }

    // Spawns one item. Falls back to the ground when the inventory/hands are full.
    // kitted = vanilla "spawn with attachments" (guns get mag + parts, cars get wheels/fluids).
    static EntityAI SpawnGear(string type, PlayerBase player, bool kitted = false, int mode = 0)
    {
        if (!player || type == "")
            return NULL;
        if (!IsKnownClass(type))
        {
            player.MessageStatus("Unknown item classname: " + type);
            return NULL;
        }

        bool worldOnly = IsWorldOnly(type);
        EntityAI item;
        if (!worldOnly && mode == SPAWN_HANDS && !player.GetHumanInventory().GetEntityInHands())
            item = player.GetHumanInventory().CreateInHands(type);
        if (!item && !worldOnly && mode != SPAWN_GROUND)
            item = player.GetInventory().CreateInInventory(type);
        if (!item)
            item = SpawnNear(type, player);
        if (!item)
        {
            player.MessageStatus("Could not spawn " + type);
            return NULL;
        }

        Magazine mag = Magazine.Cast(item);
        if (mag)
            mag.LocalSetAmmoMax();
        else if (kitted)
            item.OnDebugSpawn();
        return item;
    }

    static int SpawnKit(SoloKit kit, PlayerBase player)
    {
        int made = 0;
        foreach (string line : kit.Items)
        {
            string kitType = line;
            int kitCount = 1;
            int colon = line.IndexOf(":");
            if (colon > 0)
            {
                kitType = line.Substring(0, colon);
                kitCount = line.Substring(colon + 1, line.Length() - colon - 1).ToInt();
            }
            for (int k = 0; k < kitCount; k++)
            {
                if (SpawnGear(kitType, player, KittedSpawn, SPAWN_INVENTORY))
                    made++;
            }
        }
        return made;
    }

    static string BulletFor(Weapon_Base gun)
    {
        if (!BulletCache)
            BulletCache = new map<string, string>;
        string type = gun.GetType();
        string bullet = "";
        if (BulletCache.Find(type, bullet))
            return bullet;
        bullet = "";
        TStringArray rounds = new TStringArray;
        GetGame().ConfigGetTextArray(CFG_WEAPONSPATH + " " + type + " chamberableFrom", rounds);
        if (rounds.Count() > 0)
            GetGame().ConfigGetText(CFG_MAGAZINESPATH + " " + rounds[0] + " ammo", bullet);
        BulletCache.Insert(type, bullet);
        return bullet;
    }

    // Tops up detachable magazines on every muzzle and internal tube/box magazines
    // (shotguns, bolt-action rifles), and keeps the weapon in pristine condition.
    static void RefillWeapon(Weapon_Base gun)
    {
        if (!gun)
            return;
        for (int mi = 0; mi < gun.GetMuzzleCount(); mi++)
        {
            Magazine mag = gun.GetMagazine(mi);
            if (mag && mag.GetAmmoCount() < mag.GetAmmoMax())
                mag.LocalSetAmmoMax();

            int internalMax = gun.GetInternalMagazineMaxCartridgeCount(mi);
            if (internalMax > 0)
            {
                int missing = internalMax - gun.GetInternalMagazineCartridgeCount(mi);
                if (missing > 0)
                {
                    string bullet = BulletFor(gun);
                    if (bullet != "")
                    {
                        for (int b = 0; b < missing; b++)
                            gun.PushCartridgeToInternalMagazine(mi, 0, bullet);
                    }
                }
            }
        }
        if (gun.GetHealth("", "") < gun.GetMaxHealth("", ""))
            gun.SetHealth("", "", gun.GetMaxHealth("", ""));
    }

    static int RefillAll(PlayerBase player)
    {
        if (!player)
            return 0;
        array<EntityAI> items = new array<EntityAI>;
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
        EntityAI inHands = player.GetHumanInventory().GetEntityInHands();
        if (inHands && items.Find(inHands) == -1)
            items.Insert(inHands);

        int refilled = 0;
        foreach (EntityAI entity : items)
        {
            Magazine mag = Magazine.Cast(entity);
            if (mag)
            {
                mag.LocalSetAmmoMax();
                refilled++;
                continue;
            }
            Weapon_Base gun = Weapon_Base.Cast(entity);
            if (gun)
            {
                RefillWeapon(gun);
                refilled++;
            }
        }
        return refilled;
    }

    static void Tick(PlayerBase player)
    {
        if (!player)
            return;
        if (GodMode)
        {
            player.SetHealth("", "", player.GetMaxHealth("", ""));
            player.SetHealth("", "Blood", player.GetMaxHealth("", "Blood"));
            player.SetHealth("", "Shock", player.GetMaxHealth("", "Shock"));
            player.GetStatEnergy().Set(1000);
            player.GetStatWater().Set(1000);
        }
        if (UnlimitedAmmo)
            RefillWeapon(Weapon_Base.Cast(player.GetHumanInventory().GetEntityInHands()));
    }
}

// Logs who closed a menu when it closes right after opening (see script_*.log).
class SoloMenuGuard
{
    static int ShowTime;

    static void Shown()
    {
        ShowTime = GetGame().GetTime();
    }

    static void Hidden(string menu)
    {
        if (GetGame().GetTime() - ShowTime < 500)
        {
            Print("[DayZSolo] " + menu + " closed right after opening; call stack follows:");
            DumpStack();
        }
    }
}

// Routes list / search box events to the menu.
class SoloMenuEvents: ScriptedWidgetEventHandler
{
    protected SoloMenu m_Menu;

    void SoloMenuEvents(SoloMenu menu)
    {
        m_Menu = menu;
    }

    override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn)
    {
        if (m_Menu)
            m_Menu.OnRowSelected(row);
        return true;
    }

    override bool OnDoubleClick(Widget w, int x, int y, int button)
    {
        if (m_Menu)
            m_Menu.SpawnSelected(SoloTools.SPAWN_INVENTORY);
        return true;
    }

    override bool OnChange(Widget w, int x, int y, bool finished)
    {
        if (m_Menu)
            m_Menu.RefreshList();
        return true;
    }
}

class SoloMenu: UIScriptedMenu
{
    static const int MAX_ROWS = 1500;
    static bool IsOpen;
    static ref SoloMenu Instance; // the UI manager only holds a weak pointer

    protected EditBoxWidget m_Search;
    protected EditBoxWidget m_Amount;
    protected TextListboxWidget m_List;
    protected TextWidget m_Info;
    protected TextWidget m_CategoryText;
    protected TextWidget m_CountText;
    protected TextWidget m_Details;
    protected ItemPreviewWidget m_Preview;
    protected ButtonWidget m_GodButton;
    protected ButtonWidget m_AmmoButton;
    protected ButtonWidget m_KittedButton;
    protected ref array<ButtonWidget> m_CategoryButtons;
    protected ref SoloMenuEvents m_Events;
    protected ref TStringArray m_RowTypes;
    protected ref array<int> m_RowKits;
    protected ref TStringArray m_GunSlots;
    protected string m_GunTypes;
    protected string m_GunName;
    protected int m_Category;
    protected EntityAI m_PreviewItem;

    void SoloMenu()
    {
        SetID(SOLO_MENU_ID);
        m_RowTypes = new TStringArray;
        m_RowKits = new array<int>;
        m_GunSlots = new TStringArray;
        m_CategoryButtons = new array<ButtonWidget>;
    }

    void ~SoloMenu()
    {
        if (m_PreviewItem)
            GetGame().ObjectDelete(m_PreviewItem);
    }

    override int GetID()
    {
        return SOLO_MENU_ID;
    }

    override Widget Init()
    {
        Print("[DayZSolo] SoloMenu Init called");
        layoutRoot = GetGame().GetWorkspace().CreateWidgets("$CurrentDir:missions\\DayZSolo.ChernarusPlus\\ui\\SoloMenu.layout");
        if (!layoutRoot)
        {
            Print("[DayZSolo] ERROR: SoloMenu layout not found");
            return NULL;
        }
        m_Search = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("search_input"));
        m_Amount = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("amount_input"));
        m_List = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("item_list"));
        m_Info = TextWidget.Cast(layoutRoot.FindAnyWidget("status_text"));
        m_CategoryText = TextWidget.Cast(layoutRoot.FindAnyWidget("category_text"));
        m_CountText = TextWidget.Cast(layoutRoot.FindAnyWidget("count_text"));
        m_Details = TextWidget.Cast(layoutRoot.FindAnyWidget("details_text"));
        m_GodButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("god_button"));
        m_AmmoButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ammo_button"));
        m_KittedButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("kitted_button"));
        for (int i = 0; i < SoloCatalog.CAT_COUNT; i++)
            m_CategoryButtons.Insert(ButtonWidget.Cast(layoutRoot.FindAnyWidget("cat_" + i.ToString())));

        m_Events = new SoloMenuEvents(this);
        if (m_List)
            m_List.SetHandler(m_Events);
        if (m_Search)
            m_Search.SetHandler(m_Events);

        SoloCatalog.Build();
        SelectCategory(SoloCatalog.CAT_ALL);
        Print("[DayZSolo] SoloMenu widgets loaded");
        ShowState("Pick a category or type in SEARCH, select a row, then spawn. Double-click = to inventory.");
        return layoutRoot;
    }

    override void OnShow()
    {
        super.OnShow();
        IsOpen = true;
        SoloMenuGuard.Shown();
        Print("[DayZSolo] SoloMenu OnShow");
    }

    override void OnHide()
    {
        super.OnHide();
        IsOpen = false;
        DeletePreview();
        Print("[DayZSolo] SoloMenu OnHide");
        SoloMenuGuard.Hidden("SoloMenu");
    }

    override void LockControls()
    {
        super.LockControls();
        GetGame().GetInput().ChangeGameFocus(1);
        GetGame().GetUIManager().ShowUICursor(true);
    }

    override void UnlockControls()
    {
        super.UnlockControls();
        GetGame().GetInput().ChangeGameFocus(-1);
        GetGame().GetUIManager().ShowUICursor(false);
    }

    static string OnOff(bool value)
    {
        if (value)
            return "ON";
        return "OFF";
    }

    void ShowState(string message)
    {
        if (m_Info)
            m_Info.SetText(message);
        if (m_GodButton)
            m_GodButton.SetText("GOD MODE: " + OnOff(SoloTools.GodMode));
        if (m_AmmoButton)
            m_AmmoButton.SetText("UNLIMITED AMMO: " + OnOff(SoloTools.UnlimitedAmmo));
        if (m_KittedButton)
            m_KittedButton.SetText("KITTED: " + OnOff(SoloTools.KittedSpawn));
    }

    void SetDetails(string text)
    {
        if (m_Details)
            m_Details.SetText(text);
    }

    void SelectCategory(int category)
    {
        if (category == SoloCatalog.CAT_FOR_GUN)
        {
            PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
            Weapon_Base gun;
            if (player)
                gun = Weapon_Base.Cast(player.GetHumanInventory().GetEntityInHands());
            if (!gun)
            {
                ShowState("Hold a firearm in your hands first, then press FITS MY GUN.");
                return;
            }
            TStringArray mags = new TStringArray;
            TStringArray rounds = new TStringArray;
            m_GunTypes = SoloCatalog.GunAmmoTypes(gun.GetType(), mags, rounds);
            m_GunSlots.Clear();
            SoloCatalog.GunSlots(gun.GetType(), m_GunSlots);
            m_GunName = gun.GetType();
        }

        m_Category = category;
        for (int c = 0; c < m_CategoryButtons.Count(); c++)
        {
            ButtonWidget button = m_CategoryButtons[c];
            if (!button)
                continue;
            if (c == category)
                button.SetColor(ARGB(255, 110, 190, 255));
            else
                button.SetColor(ARGB(255, 255, 255, 255));
        }

        string title = SoloCatalog.CategoryName(category);
        if (category == SoloCatalog.CAT_FOR_GUN)
            title = "Fits your " + m_GunName + ": magazines, ammo, attachments";
        if (m_CategoryText)
            m_CategoryText.SetText(title);
        RefreshList();
    }

    static bool MatchesWords(string text, TStringArray words)
    {
        foreach (string word : words)
        {
            if (word != "" && !text.Contains(word))
                return false;
        }
        return true;
    }

    bool MatchesCategory(SoloCatalogEntry entry, bool searching)
    {
        if (m_Category == SoloCatalog.CAT_ALL)
            return searching || entry.Category != SoloCatalog.CAT_WORLD;
        if (m_Category == SoloCatalog.CAT_FOR_GUN)
        {
            if (m_GunTypes.Contains("|" + entry.TypeLower + "|"))
                return true;
            if (entry.Category != SoloCatalog.CAT_ATTACH)
                return false;
            foreach (string slot : entry.Slots)
            {
                if (m_GunSlots.Find(slot) != -1)
                    return true;
            }
            return false;
        }
        return entry.Category == m_Category;
    }

    void AddRow(string label, string type, int kit)
    {
        m_List.AddItem(label, NULL, 0);
        m_RowTypes.Insert(type);
        m_RowKits.Insert(kit);
    }

    void RefreshList()
    {
        if (!m_List || !SoloCatalog.Entries)
            return;
        DeletePreview();
        m_List.ClearItems();
        m_RowTypes.Clear();
        m_RowKits.Clear();

        string query = "";
        if (m_Search)
            query = m_Search.GetText();
        query = query.Trim();
        query.ToLower();
        TStringArray words = new TStringArray;
        query.Split(" ", words);
        bool searching = query != "";

        int total = 0;
        if (m_Category == SoloCatalog.CAT_KITS || m_Category == SoloCatalog.CAT_ALL)
        {
            for (int k = 0; k < SoloCatalog.Kits.Count(); k++)
            {
                SoloKit kit = SoloCatalog.Kits[k];
                string kitText = kit.Name;
                kitText.ToLower();
                if (!MatchesWords(kitText, words))
                    continue;
                total++;
                AddRow("[KIT] " + kit.Name, "", k);
            }
        }
        if (m_Category != SoloCatalog.CAT_KITS)
        {
            foreach (SoloCatalogEntry entry : SoloCatalog.Entries)
            {
                if (!MatchesCategory(entry, searching))
                    continue;
                if (!MatchesWords(entry.Search, words))
                    continue;
                total++;
                if (m_RowTypes.Count() < MAX_ROWS)
                    AddRow(entry.Label, entry.Type, -1);
            }
        }

        if (m_CountText)
        {
            if (total > m_RowTypes.Count())
                m_CountText.SetText("Showing " + m_RowTypes.Count().ToString() + " of " + total.ToString() + " - type in SEARCH to narrow it down");
            else
                m_CountText.SetText(total.ToString() + " entries");
        }
        SetDetails("Select a row to see details.");
    }

    void OnRowSelected(int row)
    {
        DeletePreview();
        if (row < 0 || row >= m_RowTypes.Count())
            return;
        int kitIndex = m_RowKits[row];
        if (kitIndex >= 0)
        {
            SoloKit kit = SoloCatalog.Kits[kitIndex];
            SetDetails("KIT: " + kit.Name + "\n" + SoloCatalog.JoinFirst(kit.Items, 16));
            return;
        }

        string type = m_RowTypes[row];
        string details = type;
        if (GetGame().IsKindOf(type, "Weapon_Base"))
        {
            TStringArray mags = new TStringArray;
            TStringArray rounds = new TStringArray;
            SoloCatalog.GunAmmoTypes(type, mags, rounds);
            details += "\nMags: " + SoloCatalog.JoinFirst(mags, 4);
            details += "\nAmmo: " + SoloCatalog.JoinFirst(rounds, 3);
        }
        SetDetails(details);
        if (!SoloTools.IsWorldOnly(type) && type.IndexOf("Land_") != 0)
            CreatePreview(type);
    }

    void CreatePreview(string type)
    {
        if (!m_Preview)
        {
            Widget previewPanel = layoutRoot.FindAnyWidget("preview_panel");
            if (previewPanel)
                m_Preview = ItemPreviewWidget.Cast(GetGame().GetWorkspace().CreateWidget(ItemPreviewWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, ARGB(255, 255, 255, 255), 10, previewPanel));
        }
        if (!m_Preview)
            return;
        m_PreviewItem = EntityAI.Cast(GetGame().CreateObjectEx(type, vector.Zero, ECE_LOCAL));
        if (!m_PreviewItem)
            return;
        m_Preview.SetItem(m_PreviewItem);
        m_Preview.SetModelPosition(Vector(0, 0, 0.5));
        m_Preview.SetModelOrientation(vector.Zero);
    }

    void DeletePreview()
    {
        if (m_Preview)
            m_Preview.SetItem(NULL);
        if (m_PreviewItem)
        {
            GetGame().ObjectDelete(m_PreviewItem);
            m_PreviewItem = NULL;
        }
    }

    int ReadAmount()
    {
        int amount = 1;
        if (m_Amount)
            amount = m_Amount.GetText().ToInt();
        if (amount < 1)
            amount = 1;
        if (amount > 50)
            amount = 50;
        return amount;
    }

    int SelectedRow()
    {
        if (!m_List)
            return -1;
        int row = m_List.GetSelectedRow();
        if (row < 0 || row >= m_RowTypes.Count())
            return -1;
        return row;
    }

    // Selected list row, or an exact classname typed into SEARCH.
    string SelectedType()
    {
        int row = SelectedRow();
        if (row >= 0)
            return m_RowTypes[row];
        if (m_Search)
        {
            string typed = m_Search.GetText();
            typed = typed.Trim();
            if (SoloTools.IsKnownClass(typed))
                return typed;
        }
        return "";
    }

    void SpawnSelected(int mode)
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;
        int amount = ReadAmount();

        int row = SelectedRow();
        if (row >= 0 && m_RowKits[row] >= 0)
        {
            SoloKit kit = SoloCatalog.Kits[m_RowKits[row]];
            int kitItems = 0;
            for (int n = 0; n < amount; n++)
                kitItems += SoloTools.SpawnKit(kit, player);
            ShowState("Kit spawned: " + kit.Name + " (" + kitItems.ToString() + " items, overflow goes on the ground)");
            return;
        }

        string type = SelectedType();
        if (type == "")
        {
            ShowState("Select a row in the list first (or type an exact classname in SEARCH).");
            return;
        }
        if (mode == SoloTools.SPAWN_HANDS)
            amount = 1;
        int made = 0;
        for (int s = 0; s < amount; s++)
        {
            if (SoloTools.SpawnGear(type, player, SoloTools.KittedSpawn, mode))
                made++;
        }
        ShowState("Spawned " + made.ToString() + "x " + type);
    }

    void PlaceSelected()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        string type = SelectedType();
        if (!player || type == "")
        {
            ShowState("Select a world object first (WORLD OBJECTS category).");
            return;
        }
        CGame game = GetGame();
        if (game.IsKindOf(type, "Inventory_Base") || game.IsKindOf(type, "Weapon_Base") || game.IsKindOf(type, "Magazine_Base") || game.IsKindOf(type, "Transport") || game.IsKindOf(type, "DZ_LightAI"))
        {
            ShowState("PLACE + SAVE is for world objects (buildings/props). Use ON GROUND for items, cars and AI.");
            return;
        }
        SoloTools.CreateBuilding(type, player);
        ShowState("World object placed 4m ahead and saved: " + type);
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (!w)
            return false;
        string name = w.GetName();
        if (name.IndexOf("cat_") == 0)
        {
            SelectCategory(name.Substring(4, name.Length() - 4).ToInt());
            return true;
        }

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return false;

        if (name == "close_button")
        {
            GetGame().GetUIManager().Back();
            return true;
        }
        if (name == "spawn_inv_button")
            SpawnSelected(SoloTools.SPAWN_INVENTORY);
        else if (name == "spawn_hands_button")
            SpawnSelected(SoloTools.SPAWN_HANDS);
        else if (name == "spawn_ground_button")
            SpawnSelected(SoloTools.SPAWN_GROUND);
        else if (name == "place_world_button")
            PlaceSelected();
        else if (name == "clear_search_button")
        {
            if (m_Search)
                m_Search.SetText("");
            RefreshList();
        }
        else if (name == "kitted_button")
        {
            SoloTools.KittedSpawn = !SoloTools.KittedSpawn;
            ShowState("Kitted spawn: guns come with a loaded mag + parts, cars with wheels/battery/fluids.");
        }
        else if (name == "god_button")
        {
            SoloTools.GodMode = !SoloTools.GodMode;
            player.SetAllowDamage(!SoloTools.GodMode);
            ShowState("God mode toggled");
        }
        else if (name == "ammo_button")
        {
            SoloTools.UnlimitedAmmo = !SoloTools.UnlimitedAmmo;
            if (SoloTools.UnlimitedAmmo)
                SoloTools.RefillAll(player);
            ShowState("Unlimited ammo: the gun in your hands never runs dry (mag or internal tube).");
        }
        else if (name == "refill_button")
        {
            int refilled = SoloTools.RefillAll(player);
            ShowState("Refilled " + refilled.ToString() + " magazines / ammo stacks / guns");
        }
        else if (name == "heal_button")
        {
            player.SetHealth("", "", player.GetMaxHealth("", ""));
            player.SetHealth("", "Blood", player.GetMaxHealth("", "Blood"));
            player.SetHealth("", "Shock", player.GetMaxHealth("", "Shock"));
            player.GetStatEnergy().Set(1000);
            player.GetStatWater().Set(1000);
            ShowState("Healed, fed and hydrated");
        }
        else if (name == "save_button")
        {
            SoloTools.SavePlayer(player);
            SoloTools.SaveBuildings();
            ShowState("Character and buildings saved");
        }
        else if (name == "teleport_button")
        {
            GetGame().GetUIManager().Back();
            GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SoloOpenTeleportMenu, 50, false);
        }
        else
            return false;
        return true;
    }
}

static const int SOLO_TELEPORT_MENU_ID = 927462;

// Named Chernarus locations (coordinates from Arkensor's offline mode) + map teleport.
class SoloTeleport
{
    static ref TStringArray Names;
    static ref TStringArray Positions;

    static void Add(string name, string position)
    {
        Names.Insert(name);
        Positions.Insert(position);
    }

    static void Build()
    {
        if (Names)
            return;
        Names = new TStringArray;
        Positions = new TStringArray;
        Add("Altar", "8090.49 0 9326.95");
        Add("Balota", "4393.72 0 2412.05");
        Add("Balota Airstrip", "5025.65 0 2363.44");
        Add("Bashnya", "4156.51 0 11747.29");
        Add("Bay Mutnaya", "5634.24 0 1947.33");
        Add("Bay Tikhaya", "1190.83 0 2194.76");
        Add("Bay Zelenaya", "11140.75 0 3090.17");
        Add("Belaya Polana", "14148.24 0 15004.23");
        Add("Berezhki", "15152.52 0 13815.69");
        Add("Berezino", "11856.29 0 8991.54");
        Add("Berezino Lumberyard", "12661.27 0 9590.91");
        Add("Berezino Shipyard", "13178.96 0 10213.62");
        Add("Black Forest", "9083.59 0 7555.09");
        Add("Black Lake", "13360.76 0 11902.36");
        Add("Black Mtn", "10246.34 0 11984.03");
        Add("Bogatyrka", "1504.85 0 8940.5");
        Add("Bor", "3317.02 0 4014.26");
        Add("Cap Golova", "8350.07 0 2443.64");
        Add("Cernaya Polana", "12199.96 0 13666.45");
        Add("Chernogorsk", "6572.9 0 2447.53");
        Add("Chernogorsk Factory", "6406.17 0 2710.03");
        Add("Chyornaya Bay", "7706.96 0 3087.25");
        Add("Crown Castle", "1430 0 9218.07");
        Add("Deep Lake", "1853.38 0 14860.82");
        Add("Devil's Castle", "6909.76 0 11371.53");
        Add("Dichina Military", "4622.19 0 8283.76");
        Add("Dobroe", "13096.32 0 14953.18");
        Add("Dolina", "11327.42 0 6645.57");
        Add("Drakon Island", "11185.96 0 2491.77");
        Add("Drozhino", "3446.32 0 4929.12");
        Add("Dubky", "6653.59 0 3674.47");
        Add("Dubrovka", "10397.6 0 9795.56");
        Add("Dubrovka Vyshnaya", "9952.95 0 10392.8");
        Add("Electrozavodsk", "10502.8 0 2336.88");
        Add("Electrozavodsk Power", "10358.14 0 2593.85");
        Add("Electrozavodsk Shipyard", "9961.48 0 1788.85");
        Add("Gnome Castle", "7410.93 0 9114.04");
        Add("Gorka", "9646.16 0 8810.25");
        Add("Green Mtn", "3744.3 0 6013.15");
        Add("Grishino", "6018.26 0 10256.4");
        Add("Grozovoy Pass", "3237.3 0 15293.46");
        Add("Guba", "14609.06 0 13283.4");
        Add("Guglovo", "8411.8 0 6687.87");
        Add("Gvozdno", "8706.38 0 11895.07");
        Add("Kabanino", "5298.35 0 8563.76");
        Add("Kamenka", "1892.27 0 2155.87");
        Add("Kamensk", "6633.17 0 14466.59");
        Add("Kamyshovo", "12170.79 0 3447.95");
        Add("Karmanovka", "12519.8 0 14679.5");
        Add("Khelm", "12286.8 0 10794.4");
        Add("Klen Mtn", "11491.72 0 11331.18");
        Add("Komarovo", "3619.86 0 2342.53");
        Add("Kozlova Mtn", "8826.96 0 2858.41");
        Add("Kozlovka", "4451.57 0 4577.18");
        Add("Krasnoe", "6486.37 0 14981.87");
        Add("Krasnostav", "11228.8 0 12223.2");
        Add("Krasnostav Airstrip", "12016.22 0 12490.07");
        Add("Krutoy Cap", "13624.71 0 3892.25");
        Add("Kumyrna", "8403.05 0 6030.65");
        Add("Lopatino", "2749.87 0 9991.8");
        Add("Mamino", "7981.69 0 13057.3");
        Add("Map Edge NE", "15321.19 0 15327.49");
        Add("Map Edge NW", "44.12 0 15330.89");
        Add("Map Edge SW", "19.82 0 1584.2");
        Add("Mogilevka", "7502.8 0 5237.32");
        Add("Msta", "11258.3 0 5511.28");
        Add("Myshkino", "2056.57 0 7425.29");
        Add("Myshkino Military", "1266.66 0 7225.99");
        Add("Myshkino Trainyard", "885.72 0 7696.21");
        Add("Nadezhdino", "5821.39 0 4764.33");
        Add("Nagornoe", "9333.93 0 14666.87");
        Add("Nizhnoye", "12895.4 0 8040.79");
        Add("Novaya Petrovka", "3471.91 0 12988.1");
        Add("Novaya Trainyard", "3526.6 0 12579.7");
        Add("Novodmitrovsk", "11595 0 14696.5");
        Add("Novoselky", "6052.29 0 3285.59");
        Add("Novy Sobor", "7159.26 0 7688.51");
        Add("NW Airfield ATC", "4618.3 0 10439.17");
        Add("NW Airfield Range", "5373.21 0 9979.8");
        Add("NW Airfield South", "4520.11 0 9421.26");
        Add("NW Airfield Tents", "4163.31 0 11014.24");
        Add("Olha", "13320.9 0 12893.05");
        Add("Oreshka Pass", "9877.39 0 6009.26");
        Add("Orlovets", "12136.28 0 7281.89");
        Add("Orlovets Factory", "11489.7 0 7592.98");
        Add("Otmel Island", "11642.89 0 3095.52");
        Add("Pavlovo", "1626.37 0 3846.56");
        Add("Pavlovo Military", "2101.77 0 3290.93");
        Add("Pogorevka", "4376.71 0 6413.21");
        Add("Polana", "10678.7 0 8090.18");
        Add("Polesovo", "5817 0 13490");
        Add("Prigorodki", "7984.89 0 3302.17");
        Add("Prud Lake", "6627.34 0 9288.07");
        Add("Pulkovo", "4902.36 0 5650.65");
        Add("Pusta", "9176.44 0 3858.71");
        Add("Pustoshka", "2992.21 0 7805.54");
        Add("Ratnoe", "6305.55 0 12708.4");
        Add("Rify", "13783.18 0 11233");
        Add("Rog Castle", "11255 0 4293.29");
        Add("Rogovo", "4731 0 6775.61");
        Add("Romashka", "8148.5 0 11015.2");
        Add("Severograd", "7893.4 0 12576.3");
        Add("Shakhovka", "9670.8 0 6495.85");
        Add("Silence Lake", "1203.95 0 6507.42");
        Add("Sinystok", "1447.49 0 11924.24");
        Add("Skalisty Castle", "13493.95 0 3254.96");
        Add("Skalisty Island", "13661.17 0 2960.38");
        Add("Skvsch Biathlon Arena", "488.57 0 11118.7");
        Add("Smirnovo", "11629.3 0 15023.1");
        Add("Solnichniy", "13469.1 0 6239");
        Add("Solnichniy Factory", "13069.59 0 7161.34");
        Add("Sosnovka", "2512.53 0 6371.41");
        Add("Sosnovy Pass", "2736.13 0 6757.38");
        Add("Staroye", "10164.18 0 5460.93");
        Add("Stary Sobor", "6072.64 0 7748.56");
        Add("Stary Yar", "4935.61 0 15068");
        Add("Storozh Island (Prison)", "2611.2 0 1306.15");
        Add("Svergino", "9533.2 0 13728.4");
        Add("Svetlojarsk", "13931.93 0 13231.39");
        Add("Three Crosses Gas", "312.94 0 9362.44");
        Add("Three Valleys", "12842.1 0 5500.33");
        Add("Tisy", "3413.75 0 14810.27");
        Add("Tisy Military", "1681.3 0 14116.6");
        Add("Topolka Dam", "10280.5 0 3507.5");
        Add("Topolniki", "2821.19 0 12366.5");
        Add("Troitskoe", "7559.44 0 13515.9");
        Add("Troitskoe Military", "7866.4 0 14676.1");
        Add("Tulga", "12808.7 0 4457.13");
        Add("Turovo", "13576.5 0 14128.6");
        Add("Vavilovo", "2211.83 0 11016.4");
        Add("Veresnik Mtn", "4439.42 0 8078.14");
        Add("Vybor", "3800 0 8844.25");
        Add("Vyshnoye", "6532.61 0 6090.53");
        Add("Vysotovo", "5616.26 0 2561.77");
        Add("Willow Lake", "13254.2 0 11607.8");
        Add("Zabolotye", "1156.26 0 10006.2");
        Add("Zaprudnoe", "4927.47 0 13027.2");
        Add("Zelenogorsk", "2713.4 0 5391.75");
        Add("Zelenogorsk Trainyard", "2490.65 0 5151.76");
        Add("Zub Castle", "6541.79 0 5591.69");
        Add("Zvir", "477.35 0 5202.98");
    }

    // Puts the player on the ground at x/z (the height is looked up) and saves.
    static bool TeleportTo(PlayerBase player, vector target)
    {
        if (!player)
            return false;
        if (target[0] < 0 || target[2] < 0 || target[0] > 16000 || target[2] > 16000)
        {
            player.MessageStatus("That spot is outside the map");
            return false;
        }
        if (player.GetCommand_Vehicle())
        {
            player.MessageStatus("Get out of the vehicle before teleporting");
            return false;
        }
        target[1] = GetGame().SurfaceY(target[0], target[2]) + 0.2;
        player.SetPosition(target);
        player.MessageStatus("Teleported to " + Math.Round(target[0]).ToString() + " / " + Math.Round(target[2]).ToString());
        SoloTools.SavePlayer(player);
        return true;
    }
}

class SoloTeleportEvents: ScriptedWidgetEventHandler
{
    protected SoloTeleportMenu m_Menu;

    void SoloTeleportEvents(SoloTeleportMenu menu)
    {
        m_Menu = menu;
    }

    override bool OnDoubleClick(Widget w, int x, int y, int button)
    {
        if (!m_Menu)
            return false;
        if (w && w.GetName() == "map")
            m_Menu.TeleportToScreen(x, y);
        else
            m_Menu.TeleportToSelected();
        return true;
    }

    override bool OnChange(Widget w, int x, int y, bool finished)
    {
        if (m_Menu)
            m_Menu.RefreshList();
        return true;
    }
}

class SoloTeleportMenu: UIScriptedMenu
{
    static bool IsOpen;
    static ref SoloTeleportMenu Instance; // the UI manager only holds a weak pointer

    protected MapWidget m_Map;
    protected TextListboxWidget m_List;
    protected EditBoxWidget m_Search;
    protected TextWidget m_Info;
    protected ref SoloTeleportEvents m_Events;
    protected ref array<int> m_RowIndex;

    void SoloTeleportMenu()
    {
        SetID(SOLO_TELEPORT_MENU_ID);
        m_RowIndex = new array<int>;
    }

    override int GetID()
    {
        return SOLO_TELEPORT_MENU_ID;
    }

    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets("$CurrentDir:missions\\DayZSolo.ChernarusPlus\\ui\\TeleportMenu.layout");
        if (!layoutRoot)
        {
            Print("[DayZSolo] ERROR: TeleportMenu layout not found");
            return NULL;
        }
        m_Map = MapWidget.Cast(layoutRoot.FindAnyWidget("map"));
        m_List = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("location_list"));
        m_Search = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("search_input"));
        m_Info = TextWidget.Cast(layoutRoot.FindAnyWidget("status_text"));
        m_Events = new SoloTeleportEvents(this);
        if (m_Map)
            m_Map.SetHandler(m_Events);
        if (m_List)
            m_List.SetHandler(m_Events);
        if (m_Search)
            m_Search.SetHandler(m_Events);

        SoloTeleport.Build();
        RefreshList();
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (m_Map && player)
        {
            m_Map.SetScale(0.3);
            m_Map.SetMapPos(player.GetPosition());
        }
        if (!m_Map)
            Print("[DayZSolo] WARNING: map widget missing; place list still works");
        ShowState("Double-click the map to teleport there, or pick a place on the right and press GO.");
        return layoutRoot;
    }

    override void OnShow()
    {
        super.OnShow();
        IsOpen = true;
        SoloMenuGuard.Shown();
        Print("[DayZSolo] TeleportMenu OnShow");
    }

    override void OnHide()
    {
        super.OnHide();
        IsOpen = false;
        Print("[DayZSolo] TeleportMenu OnHide");
        SoloMenuGuard.Hidden("TeleportMenu");
    }

    override void LockControls()
    {
        super.LockControls();
        GetGame().GetInput().ChangeGameFocus(1);
        GetGame().GetUIManager().ShowUICursor(true);
    }

    override void UnlockControls()
    {
        super.UnlockControls();
        GetGame().GetInput().ChangeGameFocus(-1);
        GetGame().GetUIManager().ShowUICursor(false);
    }

    void ShowState(string message)
    {
        if (m_Info)
            m_Info.SetText(message);
    }

    void RefreshList()
    {
        if (!m_List)
            return;
        m_List.ClearItems();
        m_RowIndex.Clear();
        string query = "";
        if (m_Search)
            query = m_Search.GetText();
        query = query.Trim();
        query.ToLower();
        for (int i = 0; i < SoloTeleport.Names.Count(); i++)
        {
            string placeName = SoloTeleport.Names[i];
            string lowerName = placeName;
            lowerName.ToLower();
            if (query != "" && !lowerName.Contains(query))
                continue;
            m_List.AddItem(placeName, NULL, 0);
            m_RowIndex.Insert(i);
        }
    }

    void Finish(bool teleported)
    {
        if (teleported)
            GetGame().GetUIManager().Back();
    }

    void TeleportToScreen(int x, int y)
    {
        if (!m_Map)
            return;
        vector world = m_Map.ScreenToMap(Vector(x, y, 0));
        Finish(SoloTeleport.TeleportTo(PlayerBase.Cast(GetGame().GetPlayer()), world));
    }

    void TeleportToMapCenter()
    {
        if (!m_Map)
            return;
        Finish(SoloTeleport.TeleportTo(PlayerBase.Cast(GetGame().GetPlayer()), m_Map.GetMapPos()));
    }

    void TeleportToSelected()
    {
        if (!m_List)
            return;
        int row = m_List.GetSelectedRow();
        if (row < 0 || row >= m_RowIndex.Count())
        {
            ShowState("Select a place in the list first.");
            return;
        }
        int index = m_RowIndex[row];
        vector target = SoloTeleport.Positions[index].ToVector();
        Finish(SoloTeleport.TeleportTo(PlayerBase.Cast(GetGame().GetPlayer()), target));
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (!w)
            return false;
        string name = w.GetName();
        if (name == "close_button")
            GetGame().GetUIManager().Back();
        else if (name == "go_button")
            TeleportToSelected();
        else if (name == "center_button")
            TeleportToMapCenter();
        else if (name == "items_button")
        {
            GetGame().GetUIManager().Back();
            GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SoloOpenToolsMenu, 50, false);
        }
        else if (name == "clear_search_button")
        {
            if (m_Search)
                m_Search.SetText("");
            RefreshList();
        }
        else
            return false;
        return true;
    }
}

void SoloOpenToolsMenu()
{
    if (!SoloMenu.IsOpen)
    {
        SoloMenu.Instance = new SoloMenu();
        GetGame().GetUIManager().ShowScriptedMenu(SoloMenu.Instance, NULL);
    }
}

void SoloOpenTeleportMenu()
{
    if (!SoloTeleportMenu.IsOpen)
    {
        SoloTeleportMenu.Instance = new SoloTeleportMenu();
        GetGame().GetUIManager().ShowScriptedMenu(SoloTeleportMenu.Instance, NULL);
    }
}

void main()
{
    MakeDirectory("$profile:DayZSolo");
    Hive ce = CreateHive();
    if (ce)
    {
        ce.InitOffline();
        Print("[DayZSolo] Local Central Economy initialized");
    }

    GetGame().GetWorld().SetDate(2026, 9, 20, 12, 0);
    vector spawn = "6500 0 2600";
    spawn[1] = GetGame().SurfaceY(spawn[0], spawn[2]) + 0.3;
    PlayerBase player = PlayerBase.Cast(GetGame().CreatePlayer(NULL, "SurvivorM_Mirek", spawn, 0, "NONE"));
    if (!player)
    {
        Print("[DayZSolo] ERROR: Could not create player");
        return;
    }

    if (!SoloTools.LoadPlayer(player))
    {
        EntityAI shirt = player.GetInventory().CreateInInventory("TShirt_Black");
        player.GetInventory().CreateInInventory("ShortJeans_Green");
        player.GetInventory().CreateInInventory("WorkingBoots_Brown");
        if (shirt)
        {
            shirt.GetInventory().CreateInInventory("BandageDressing");
            shirt.GetInventory().CreateInInventory("Apple");
        }
        Print("[DayZSolo] Fresh survivor created");
    }
    GetGame().SelectPlayer(NULL, player);
    SoloTools.LoadBuildings();
    Print("[DayZSolo] Singleplayer loaded. K = item menu, O = teleport map");
}

class SoloMission: MissionGameplay
{
    protected float m_SaveTime;
    protected float m_EffectTime;

    override void OnKeyPress(int key)
    {
        super.OnKeyPress(key);
        // F1-F12 are emotes and HOME/END/INSERT are DayZDiag debug keys; their actions close
        // any open menu. K and O are unbound by default. They only open (so typing them in a
        // search box is harmless); ESC or the CLOSE button closes. Opening is deferred so the
        // key press is fully processed first.
        if (SoloMenu.IsOpen || SoloTeleportMenu.IsOpen || GetGame().GetUIManager().GetMenu())
            return;
        if (key == KeyCode.KC_K)
        {
            Print("[DayZSolo] K pressed: opening item menu");
            GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SoloOpenToolsMenu, 150, false);
        }
        else if (key == KeyCode.KC_O)
        {
            Print("[DayZSolo] O pressed: opening teleport menu");
            GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SoloOpenTeleportMenu, 150, false);
        }
    }

    override UIScriptedMenu CreateScriptedMenu(int id)
    {
        if (id == SOLO_MENU_ID)
        {
            SoloMenu.Instance = new SoloMenu();
            return SoloMenu.Instance;
        }
        if (id == SOLO_TELEPORT_MENU_ID)
        {
            SoloTeleportMenu.Instance = new SoloTeleportMenu();
            return SoloTeleportMenu.Instance;
        }
        return super.CreateScriptedMenu(id);
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
        m_EffectTime += timeslice;
        if (m_EffectTime >= 0.1)
        {
            m_EffectTime = 0;
            SoloTools.Tick(PlayerBase.Cast(GetGame().GetPlayer()));
        }

        m_SaveTime += timeslice;
        if (m_SaveTime >= 20.0)
        {
            m_SaveTime = 0;
            SoloTools.SavePlayer(PlayerBase.Cast(GetGame().GetPlayer()));
        }
    }

    override void OnMissionFinish()
    {
        SoloTools.SavePlayer(PlayerBase.Cast(GetGame().GetPlayer()));
        SoloTools.SaveBuildings();
        if (GetHive())
            DestroyHive();
        super.OnMissionFinish();
    }
}

Mission CreateCustomMission(string path)
{
    return new SoloMission();
}
