function int GiveClassWeapon(int class, int slot, int ammoMode)
{
    return _giveclassweapon(class, slot, ammoMode, 0, 0);
}

function int _giveclassweapon(int class, int slot, int ammoMode, int dropped, int nopd)
{
    int pdSaws     = (IsPunchdrunk & 4) || (IsPunchdrunk & 1);
    if (pdSaws && slot == SLOT_CHAINSAW) { slot = SLOT_PUNCHDRUNKSAW; }

    int weapon = ClassWeapons[class][slot][S_WEP];
    int ammo1  = ClassWeapons[class][slot][S_AMMO1];
    int ammo2  = ClassWeapons[class][slot][S_AMMO2];
    int a1count = CheckInventory(ammo1);
    int a2count = CheckInventory(ammo2);

    int giveScript = ClassScripts[class][slot];
    int hasAmmo = 0;
    int giveWep = 0;
    int hasWep  = CheckInventory(weapon);
    int success;

    if (class == -1) { return 0; }

    if (!StrLen(weapon)) { return 0; }

    if (!CheckInventory(weapon)) { giveWep = 1; }
    if (ClassWeapons[class][slot][S_CHECKITEM] == "ForceCheck"
     || !CheckInventory(ClassWeapons[class][slot][S_CHECKITEM])) { giveWep = 1; }

    if (StrLen(ammo1)) { hasAmmo |= 1; }
    if (StrLen(ammo2)) { hasAmmo |= 2; }

    if (((hasAmmo & 1) && a1count < GetAmmoCapacity(ammo1))
     || ((hasAmmo & 2) && a2count < GetAmmoCapacity(ammo2)))
    {
        giveWep = 1;
    }

    if (giveWep)
    {
        if (StrLen(SlotItems[slot])) { GiveInventory(SlotItems[slot], 1); }

        if (giveScript > 0)
        {
            success = ACS_ExecuteWithResult(giveScript, class, slot, dropped);
        }
        else
        {
            success = 1;
            GiveInventory(weapon, 1);
        }

        if (success && array_pickupswitch[PlayerNumber()] && !hasWep &&
                (array_pickupswitch[PlayerNumber()] >= 2 || slot > ClassWeaponSlot())
            || PlayerIsBot(PlayerNumber()))
            
        {
            SetWeapon(ClassWeapons[class][slot][S_WEP]);
        }

        switch (ammoMode)
        {
          case 0:
            if (hasAmmo & 1) { TakeInventory(ammo1, CheckInventory(ammo1) - a1count); }
            if (hasAmmo & 2) { TakeInventory(ammo2, CheckInventory(ammo2) - a2count); }
            break;

          case 1:
            if (hasAmmo & 1) { GiveInventory(ammo1, GetAmmoCapacity(ammo1)); }
            if (hasAmmo & 2) { GiveInventory(ammo2, GetAmmoCapacity(ammo2)); }
            break;
          
          case 2:
            if (hasAmmo & 1) { TakeInventory(ammo1, (CheckInventory(ammo1) - a1count) / 2); }
            if (hasAmmo & 2) { TakeInventory(ammo2, (CheckInventory(ammo2) - a2count) / 2); }
            break;

          case 3:
            break;
        }
    }

    return !!success;
}

function int HasClassWeapon(int class, int slot)
{
    if (class == -1) { return 0; }

    int weapon = ClassWeapons[class][slot][S_WEP];
    int checkitem = ClassWeapons[class][slot][S_CHECKITEM];
    int failitem = ClassWeapons[class][slot][S_CHECKFAILITEM];

    int hasWep, hasItem, hasFail;

    if (!StrLen(weapon)) { return 0; }

    hasWep  = CheckInventory(weapon);
    hasItem = StrLen(checkitem) && CheckInventory(checkitem);
    hasFail = StrLen(failitem) && CheckInventory(failitem);

    return hasWep || hasItem || hasFail;
}

function void GiveClassUnique(int class, int which)
{
    int unique, ammo, amax;

    if (class == -1) { return; }
    
    switch (which)
    {
      case 0:
        unique = ClassUniques[class][U_UNIQUE1];
        ammo   = ClassUniques[class][U_AMMO1];
        amax   = UniqueMaxes[class][U_AMMO1];
        break;
    
      case 1:
        unique = ClassUniques[class][U_UNIQUE2];
        ammo   = ClassUniques[class][U_AMMO2];
        amax   = UniqueMaxes[class][U_AMMO2];
        break;
    }

    if (StrLen(unique)) { GiveInventory(unique, 1); }
    if (StrLen(ammo)) { GiveInventory(ammo, amax - CheckInventory(ammo)); }
}

function void ApplyLMS(void)
{
    int classNum = samsaraClassNum();
    int lmsLevel = middle(0, GetCVar("samsara_lmslife"), LMSMODES-1);
    int i;

    if (classNum == -1) { return; }
    
	SetActorProperty(0, APROP_Health, getMaxHealth());
    GiveInventory("Backpack", 1);

    for (i = 0; i < SLOT_BFG9000; i++) { GiveClassWeapon(classNum, i, 1); }


    if (StrLen(LMSItems[classNum])) { GiveInventory(LMSItems[classNum], 1); }
    if (GetCVar("samsara_lmsult")) { GiveClassWeapon(classNum, SLOT_BFG9000, 1); }

    i = (GetCVar("samsara_lmslife") + 1) * PlayerCount();

    GiveInventory("Clip",       GetAmmoCapacity("Clip")         - CheckInventory("Clip"));
    GiveInventory("Shell",      GetAmmoCapacity("Shell")        - CheckInventory("Shell"));
    GiveInventory("RocketAmmo", GetAmmoCapacity("RocketAmmo")   - CheckInventory("RocketAmmo"));
    GiveInventory("Cell",       GetAmmoCapacity("Cell")         - CheckInventory("Cell"));

    TakeInventory("LavaNails",       0x7FFFFFFF);
    TakeInventory("MultiRocketAmmo", 0x7FFFFFFF);
    GiveInventory("LavaNails",       ftoi(2.5 * i));
    GiveInventory("MultiRocketAmmo", ftoi(0.5 * i));

    if (lmsLevel > 0)
    {
        SetActorProperty(0, APROP_Health, GetActorProperty(0, APROP_Health) + (100 * (lmsLevel-1)));
        ACS_ExecuteWithResult(SAMSARA_DECORATE, 20, ARMOR_BLUE, -100 * lmsLevel);
    }
	//ACS_ExecuteAlways(678,0,0,0,0);
	//HandlePunchDrunk(respawning);
} 

function int samsaraClassNum(void)
{
    int i;

    for (i = 0; i < CLASSCOUNT; i++)
    {
        if (CheckInventory(ClassItems[i])) { return i; }
    }

    return -1;
}

function int slotToItem(int i)
{
    i--;
    if (i < 0 || i >= SLOTCOUNT) { return -1; }
    return StoIArray[i];
}

function int itemToSlot(int i)
{
    i--;
    if (i < 0 || i > 6) { return -1; }
    return ItoSArray[i];
}

function int SamsaraClientVars(void)
{
    int switchOnPickup  = !!GetCVar("switchonpickup");
    int weaponBar       = !!GetCVar("samsara_cl_weaponhud");
    int ballgag         = !!GetCVar("samsara_cl_ballgag");
    int classicAnims    = !!GetCVar("samsara_cl_vanilladoom");
    int wolfmove        = !!GetCVar("samsara_cl_wolfmove");

    return (switchOnPickup << 4) + (weaponBar << 3) + (ballgag << 2) + (classicAnims << 1) + wolfmove;
}

function int GiveUnique(int cnum, int unum)
{
    return _giveunique(cnum, unum, 0, 0);
}

int TempUniques[UNIQUECOUNT];

function int _giveunique(int cnum, int unum, int ignoreinv, int nopd)
{
    int success; 
    int i, j, tmpcount;

    int punchdrunk = IsPunchdrunk & 1;
    int pdUniques  = (IsPunchdrunk & 2) || punchdrunk;

    if (cnum == -1) { return -1; }

    if (pdUniques && !nopd)
    {
        GiveInventory(PunchdrunkItems[cnum][1], 1);
        return 1;
    }

    if (unum == -1)
    {
        tmpcount = 0;
        for (i = 0; i < UNIQUECOUNT; i++)
        {
            j = ClassUniques[cnum][i];
            if (j == "") { continue; }

            TempUniques[tmpcount++] = i;
        }

        if (tmpcount == 0) { return -1; }

        unum = TempUniques[random(0, tmpcount-1)];
    }

    unum *= 2;
    int uanum = unum + 1;

    int unique = ClassUniques[cnum][unum];
    int unammo = ClassUniques[cnum][uanum];

    int uniqueMax = UniqueMaxes[cnum][unum];
    int unammoMax = UniqueMaxes[cnum][uanum];

    int unCount = UniqueCounts[cnum][unum];
    int unammoCount = UniqueCounts[cnum][uanum];

    unCount = cond(unCount == -1, 1, unCount);
    unammoCount = cond(unammoCount == -1, unammoMax, unammoCount);

    if (unammoMax == 0 && unammo) { unammoMax = GetAmmoCapacity(unammo); }

    // If you can get a unique and:
    //  - You don't have the unique at all
    //   or
    //  - You don't get multiple duplicates of the unique, and it lacks ammo
    //   or
    //  - It does have ammo, but you're not full
    //   or
    //  - You can have multiple duplicates of the unique, and you're not full

    if (unique != "" && ((!CheckInventory(unique) || ignoreinv) || 
                         (uniqueMax <= 1 && (unammo == "" || unammoMax == 0)) ||
                         (unammoMax != 0 && (CheckInventory(unammo) != unammoMax || ignoreinv)) ||
                         (uniqueMax > 1 && (CheckInventory(unique) != uniqueMax || ignoreinv))
                        )
       )
    {
        GiveInventory(unique, unCount);
        GiveInventory(unammo, unammoCount);
        success = 1;
    }

    return success;
}

function void TakeUnique(int cnum, int unum)
{
    unum *= 2;
    int uanum = unum + 1;

    if (cnum == -1) { return; }

    int unique = ClassUniques[cnum][unum];
    if (unique != "") { TakeInventory(unique, 0x7FFFFFFF); }
}

function int HasUnique(int cnum, int unum)
{
    if (cnum == -1) { return 0; }

    int unique = ClassUniques[cnum][unum*2];
    return (unique != "") && CheckInventory(unique);
}


function int ClassWeaponSlot(void)
{
    int pclass = samsaraClassNum();
    int weapon, i;

    if (pclass == -1) { return -1; }

    for (i = 0; i < SLOTCOUNT; i++)
    {
        weapon = ClassWeapons[pclass][i][S_WEP];

        if (CheckWeapon(weapon)) { return i; }
    }

    return -1;
}

function int ConvertClassWeapons(int classnum)
{
    int i, j, k;
    int ret;

    if (classnum < 0) { classnum = samsaraClassNum(); }

    for (i = 0; i < CLASSCOUNT; i++)
    {
        if (i == classnum) { continue; }

        for (j = 0; j < SLOTCOUNT; j++)
        {
            if (HasClassWeapon(i, j))
            {
                TakeInventory(ClassWeapons[i][j][S_WEP], 0x7FFFFFFF);
                if (classnum != -1) { GiveClassWeapon(classnum, j, 1); }
                ret += 1;
            }
        }

        for (j = 0; j < UNIQUECOUNT; j++)
        {
            if (HasUnique(i, j))
            {
                k = j;
                TakeUnique(i, j);

                while (!_giveunique(classnum, k, 1, 0) && k >= 0) { k--; }
            }
        }
    }


    return ret;
}

function int ammoCount(int ammoname)
{
    switch (ammoname)
    {
      case "Clip":          return 10;
      case "Shell":         return 4;
      case "Shell":     return 4;
      case "RocketAmmo":    return 1;
      case "Cell":          return 20;
    }

    return GetAmmoCapacity(ammoname); // not the best of defaults but ya gotta have SOMETHING
}

function int CheckQuad(void)
{
    return CheckInventory("QuakeQuadTimer") - QUAD_THRESHOLD;
}

function int GiveQuad(int toAdd)
{
    int quadcount = QUAD_THRESHOLD - CheckInventory("QuakeQuadTimer");
    GiveInventory("QuakeQuadTimer", quadcount);
    GiveInventory("QuakeQuadTimer", toAdd);

    if (GetCVar("samsara_permault"))
    {
        GiveInventory("DoNotQuad", 1);   // nasty hack
    }

    quadcount = max(0, CheckInventory("QuakeQuadTimer") - QUAD_THRESHOLD);

    return quadcount;
}

function int HandleUniqueSpawn(int respawning)
{
    int cs = GetCVar("samsara_uniquestart");
    int classnum = samsaraClassNum();
    int i;

    switch (cs)
    {
      case 1:
        if (respawning && isCoop()) { return 0; }
        // Fallthrough

      case 3:
        GiveUnique(classnum, -1);
        break;

      case 2:
        if (respawning && isCoop()) { return 0; }
        // Fallthrough

      case 4:
        for (i = 0; i < UNIQUECOUNT; i++) { GiveUnique(classnum, i); }
        break;
    }

    return 1;
}

function int HandleChainsawSpawn(int respawning)
{
    int cs = GetCVar("samsara_chainsawstart");
    int classnum = samsaraClassNum();
    int ammomode = 3;

    if (cs == 0 || (respawning && isCoop())) { return 0; }

    if (cs == 2) { ammomode = 1; }

    GiveClassWeapon(classnum, SLOT_CHAINSAW, ammomode);
    return 1;
}

function int HandlePunchDrunk(int respawning)
{
    int cs = GetCVar("samsara_punchdrunk");
    int classnum = samsaraClassNum();
    int i;

    //delay(10);

    if (cs <= 0) { return 0; }

    for (i = 0; i < SLOTCOUNT; i++)
    {
        if (ClassWeapons[classnum][i][S_WEP] == ClassWeapons[classnum][SLOT_FIST][S_WEP]
         || ClassWeapons[classnum][i][S_WEP] == ClassWeapons[classnum][SLOT_PUNCHDRUNKSAW][S_WEP]) { continue; }
        if (i == SLOT_PISTOL)
        {
            if (isCoop()) { continue; }
            else
            {
                GiveInventory(ClassWeapons[classnum][i][S_WEP], 1);
            }
        }

        TakeInventory(ClassWeapons[classnum][i][S_WEP], 0x7FFFFFFF);
        TakeInventory(ClassWeapons[classnum][i][S_AMMO1], 0x7FFFFFFF);
        TakeInventory(ClassWeapons[classnum][i][S_AMMO2], 0x7FFFFFFF);
    }

    GiveClassWeapon(classnum, 0, 1);
    GiveInventory(PunchDrunkItems[classnum][0], 1);
    return 1;
}

function void CheckMapArmors(void)
{
    int i;

    for (i = 0; i < ARMORCOUNT; i++)
    {
        MapArmors[i] = ThingCountName(ArmorPickups[i], 0) > 0;
    }
}

function void SetArmorMode(void)
{
    if (ArmorMode < 0)
    {
        ArmorMode = middle(0, GetCVar("samsara_armormode"), ARMORMODES-1);
    }
}

function int SamsaraArmorType(void)
{
    int i, j;
    int pln = PlayerNumber();

    for (i = 0; i < ARMORMODES; i++)
    {
        for (j = 0; j < ARMORCOUNT; j++)
        {
            if (GetArmorType(ArmorItems[i][j][0], pln) > 0
             || GetArmorType(ArmorItems[i][j][2], pln) > 0)
            {
                return i;
            }
        }
    }

    return -1;
}

function int HandleInstagib(int respawning)
{
    int cs = GetCVar("instagib");
    int classnum = samsaraClassNum();
    int i;

    if (cs <= 0) { return 0; }

    for (i = 0; i < SLOTCOUNT; i++)
    {
        TakeInventory(ClassWeapons[classnum][i][S_WEP], 0x7FFFFFFF);
        TakeInventory(ClassWeapons[classnum][i][S_AMMO1], 0x7FFFFFFF);
        TakeInventory(ClassWeapons[classnum][i][S_AMMO2], 0x7FFFFFFF);
    }

    GiveInventory("InstagibModeOn",1);
    if (CheckInventory("DoomguyClass") == 1) { GiveInventory(" Railgun ",1); }
    if (CheckInventory("ChexClass") == 1) { GiveInventory("Gigazorcher 2100",1); }
    if (CheckInventory("CorvusClass") == 1) { GiveInventory("Grim Ballista",1); }
    if (CheckInventory("WolfenClass") == 1) { GiveInventory("Mauser Rifle",1); }
    if (CheckInventory("HexenClass") == 1) { GiveInventory("Bloodscourge",1); }
    if (CheckInventory("DukeClass") == 1) { GiveInventory("Golden Desert Eagle",1); }
    if (CheckInventory("MarathonClass") == 1) { GiveInventory("SPNKR-25 Auto Cannon",1); }
    if (CheckInventory("QuakeClass") == 1) { GiveInventory("Rocket Powered Impaler",1); }
    return 1;
}

function int HandleDoomguyDamage(int respawning)
{
if (CheckInventory("DoomguyClass") == 1)
{
    if (GetCvar("samsara_doomguydamage") != -10) { TakeInventory("PowerDamageBuff-10",1); }
    if (GetCvar("samsara_doomguydamage") != -9)  { TakeInventory("PowerDamageBuff-9",1); }
    if (GetCvar("samsara_doomguydamage") != -8)  { TakeInventory("PowerDamageBuff-8",1); }
    if (GetCvar("samsara_doomguydamage") != -7)  { TakeInventory("PowerDamageBuff-7",1); }
    if (GetCvar("samsara_doomguydamage") != -6)  { TakeInventory("PowerDamageBuff-6",1); }
    if (GetCvar("samsara_doomguydamage") != -5)  { TakeInventory("PowerDamageBuff-5",1); }
    if (GetCvar("samsara_doomguydamage") != -4)  { TakeInventory("PowerDamageBuff-4",1); }
    if (GetCvar("samsara_doomguydamage") != -3)  { TakeInventory("PowerDamageBuff-3",1); }
    if (GetCvar("samsara_doomguydamage") != -2)  { TakeInventory("PowerDamageBuff-2",1); }
    if (GetCvar("samsara_doomguydamage") != -1)  { TakeInventory("PowerDamageBuff-1",1); }
    if (GetCvar("samsara_doomguydamage") != 1)   { TakeInventory("PowerDamageBuff1",1); }
    if (GetCvar("samsara_doomguydamage") != 2)   { TakeInventory("PowerDamageBuff2",1); }
    if (GetCvar("samsara_doomguydamage") != 3)   { TakeInventory("PowerDamageBuff3",1); }
    if (GetCvar("samsara_doomguydamage") != 4)   { TakeInventory("PowerDamageBuff4",1); }
    if (GetCvar("samsara_doomguydamage") != 5)   { TakeInventory("PowerDamageBuff5",1); }
    if (GetCvar("samsara_doomguydamage") != 6)   { TakeInventory("PowerDamageBuff6",1); }
    if (GetCvar("samsara_doomguydamage") != 7)   { TakeInventory("PowerDamageBuff7",1); }
    if (GetCvar("samsara_doomguydamage") != 8)   { TakeInventory("PowerDamageBuff8",1); }
    if (GetCvar("samsara_doomguydamage") != 9)   { TakeInventory("PowerDamageBuff9",1); }
    if (GetCvar("samsara_doomguydamage") != 10)  { TakeInventory("PowerDamageBuff10",1); }
    if (GetCvar("samsara_doomguydamage") != 11)  { TakeInventory("PowerDamageBuff11",1); }
    if (GetCvar("samsara_doomguydamage") != 12)  { TakeInventory("PowerDamageBuff12",1); }
    if (GetCvar("samsara_doomguydamage") != 13)  { TakeInventory("PowerDamageBuff13",1); }
    if (GetCvar("samsara_doomguydamage") != 14)  { TakeInventory("PowerDamageBuff14",1); }
    if (GetCvar("samsara_doomguydamage") != 15)  { TakeInventory("PowerDamageBuff15",1); }
    if (GetCvar("samsara_doomguydamage") != 16)  { TakeInventory("PowerDamageBuff16",1); }
    if (GetCvar("samsara_doomguydamage") != 17)  { TakeInventory("PowerDamageBuff17",1); }
    if (GetCvar("samsara_doomguydamage") != 18)  { TakeInventory("PowerDamageBuff18",1); }
    if (GetCvar("samsara_doomguydamage") != 19)  { TakeInventory("PowerDamageBuff19",1); }
    if (GetCvar("samsara_doomguydamage") != 20)  { TakeInventory("PowerDamageBuff20",1); }
    if (GetCvar("samsara_doomguydamage") != 21)  { TakeInventory("PowerDamageBuff21",1); }
    if (GetCvar("samsara_doomguydamage") != 22)  { TakeInventory("PowerDamageBuff22",1); }
    if (GetCvar("samsara_doomguydamage") != 23)  { TakeInventory("PowerDamageBuff23",1); }
    if (GetCvar("samsara_doomguydamage") != 24)  { TakeInventory("PowerDamageBuff24",1); }
    if (GetCvar("samsara_doomguydamage") != 25)  { TakeInventory("PowerDamageBuff25",1); }
    if (GetCvar("samsara_doomguydamage") != 26)  { TakeInventory("PowerDamageBuff26",1); }
    if (GetCvar("samsara_doomguydamage") != 27)  { TakeInventory("PowerDamageBuff27",1); }
    if (GetCvar("samsara_doomguydamage") != 28)  { TakeInventory("PowerDamageBuff28",1); }
    if (GetCvar("samsara_doomguydamage") != 29)  { TakeInventory("PowerDamageBuff29",1); }
    if (GetCvar("samsara_doomguydamage") != 30)  { TakeInventory("PowerDamageBuff30",1); }
	
    if (GetCvar("samsara_doomguydamage") == -10) { GiveInventory("PowerDamageBuff-10",1); }
    if (GetCvar("samsara_doomguydamage") == -9)  { GiveInventory("PowerDamageBuff-9",1); }
    if (GetCvar("samsara_doomguydamage") == -8)  { GiveInventory("PowerDamageBuff-8",1); }
    if (GetCvar("samsara_doomguydamage") == -7)  { GiveInventory("PowerDamageBuff-7",1); }
    if (GetCvar("samsara_doomguydamage") == -6)  { GiveInventory("PowerDamageBuff-6",1); }
    if (GetCvar("samsara_doomguydamage") == -5)  { GiveInventory("PowerDamageBuff-5",1); }
    if (GetCvar("samsara_doomguydamage") == -4)  { GiveInventory("PowerDamageBuff-4",1); }
    if (GetCvar("samsara_doomguydamage") == -3)  { GiveInventory("PowerDamageBuff-3",1); }
    if (GetCvar("samsara_doomguydamage") == -2)  { GiveInventory("PowerDamageBuff-2",1); }
    if (GetCvar("samsara_doomguydamage") == -1)  { GiveInventory("PowerDamageBuff-1",1); }
    if (GetCvar("samsara_doomguydamage") == 1)   { GiveInventory("PowerDamageBuff1",1); }
    if (GetCvar("samsara_doomguydamage") == 2)   { GiveInventory("PowerDamageBuff2",1); }
    if (GetCvar("samsara_doomguydamage") == 3)   { GiveInventory("PowerDamageBuff3",1); }
    if (GetCvar("samsara_doomguydamage") == 4)   { GiveInventory("PowerDamageBuff4",1); }
    if (GetCvar("samsara_doomguydamage") == 5)   { GiveInventory("PowerDamageBuff5",1); }
    if (GetCvar("samsara_doomguydamage") == 6)   { GiveInventory("PowerDamageBuff6",1); }
    if (GetCvar("samsara_doomguydamage") == 7)   { GiveInventory("PowerDamageBuff7",1); }
    if (GetCvar("samsara_doomguydamage") == 8)   { GiveInventory("PowerDamageBuff8",1); }
    if (GetCvar("samsara_doomguydamage") == 9)   { GiveInventory("PowerDamageBuff9",1); }
    if (GetCvar("samsara_doomguydamage") == 10)  { GiveInventory("PowerDamageBuff10",1); }
    if (GetCvar("samsara_doomguydamage") == 11)  { GiveInventory("PowerDamageBuff11",1); }
    if (GetCvar("samsara_doomguydamage") == 12)  { GiveInventory("PowerDamageBuff12",1); }
    if (GetCvar("samsara_doomguydamage") == 13)  { GiveInventory("PowerDamageBuff13",1); }
    if (GetCvar("samsara_doomguydamage") == 14)  { GiveInventory("PowerDamageBuff14",1); }
    if (GetCvar("samsara_doomguydamage") == 15)  { GiveInventory("PowerDamageBuff15",1); }
    if (GetCvar("samsara_doomguydamage") == 16)  { GiveInventory("PowerDamageBuff16",1); }
    if (GetCvar("samsara_doomguydamage") == 17)  { GiveInventory("PowerDamageBuff17",1); }
    if (GetCvar("samsara_doomguydamage") == 18)  { GiveInventory("PowerDamageBuff18",1); }
    if (GetCvar("samsara_doomguydamage") == 19)  { GiveInventory("PowerDamageBuff19",1); }
    if (GetCvar("samsara_doomguydamage") == 20)  { GiveInventory("PowerDamageBuff20",1); }
    if (GetCvar("samsara_doomguydamage") == 21)  { GiveInventory("PowerDamageBuff21",1); }
    if (GetCvar("samsara_doomguydamage") == 22)  { GiveInventory("PowerDamageBuff22",1); }
    if (GetCvar("samsara_doomguydamage") == 23)  { GiveInventory("PowerDamageBuff23",1); }
    if (GetCvar("samsara_doomguydamage") == 24)  { GiveInventory("PowerDamageBuff24",1); }
    if (GetCvar("samsara_doomguydamage") == 25)  { GiveInventory("PowerDamageBuff25",1); }
    if (GetCvar("samsara_doomguydamage") == 26)  { GiveInventory("PowerDamageBuff26",1); }
    if (GetCvar("samsara_doomguydamage") == 27)  { GiveInventory("PowerDamageBuff27",1); }
    if (GetCvar("samsara_doomguydamage") == 28)  { GiveInventory("PowerDamageBuff28",1); }
    if (GetCvar("samsara_doomguydamage") == 29)  { GiveInventory("PowerDamageBuff29",1); }
    if (GetCvar("samsara_doomguydamage") == 30)  { GiveInventory("PowerDamageBuff30",1); }
}
return 1;
}

function int HandleDoomguyDefense(int respawning)
{
if (CheckInventory("DoomguyClass") == 1)
{
    if (GetCvar("samsara_doomguydefense") != -10) { TakeInventory("PowerDefenseBuff-10",1); }
    if (GetCvar("samsara_doomguydefense") != -9)  { TakeInventory("PowerDefenseBuff-9",1); }
    if (GetCvar("samsara_doomguydefense") != -8)  { TakeInventory("PowerDefenseBuff-8",1); }
    if (GetCvar("samsara_doomguydefense") != -7)  { TakeInventory("PowerDefenseBuff-7",1); }
    if (GetCvar("samsara_doomguydefense") != -6)  { TakeInventory("PowerDefenseBuff-6",1); }
    if (GetCvar("samsara_doomguydefense") != -5)  { TakeInventory("PowerDefenseBuff-5",1); }
    if (GetCvar("samsara_doomguydefense") != -4)  { TakeInventory("PowerDefenseBuff-4",1); }
    if (GetCvar("samsara_doomguydefense") != -3)  { TakeInventory("PowerDefenseBuff-3",1); }
    if (GetCvar("samsara_doomguydefense") != -2)  { TakeInventory("PowerDefenseBuff-2",1); }
    if (GetCvar("samsara_doomguydefense") != -1)  { TakeInventory("PowerDefenseBuff-1",1); }
    if (GetCvar("samsara_doomguydefense") != 1)   { TakeInventory("PowerDefenseBuff1",1); }
    if (GetCvar("samsara_doomguydefense") != 2)   { TakeInventory("PowerDefenseBuff2",1); }
    if (GetCvar("samsara_doomguydefense") != 3)   { TakeInventory("PowerDefenseBuff3",1); }
    if (GetCvar("samsara_doomguydefense") != 4)   { TakeInventory("PowerDefenseBuff4",1); }
    if (GetCvar("samsara_doomguydefense") != 5)   { TakeInventory("PowerDefenseBuff5",1); }
    if (GetCvar("samsara_doomguydefense") != 6)   { TakeInventory("PowerDefenseBuff6",1); }
    if (GetCvar("samsara_doomguydefense") != 7)   { TakeInventory("PowerDefenseBuff7",1); }
    if (GetCvar("samsara_doomguydefense") != 8)   { TakeInventory("PowerDefenseBuff8",1); }
    if (GetCvar("samsara_doomguydefense") != 9)   { TakeInventory("PowerDefenseBuff9",1); }
    if (GetCvar("samsara_doomguydefense") != 10)  { TakeInventory("PowerDefenseBuff10",1); }
    if (GetCvar("samsara_doomguydefense") != 11)  { TakeInventory("PowerDefenseBuff11",1); }
    if (GetCvar("samsara_doomguydefense") != 12)  { TakeInventory("PowerDefenseBuff12",1); }
    if (GetCvar("samsara_doomguydefense") != 13)  { TakeInventory("PowerDefenseBuff13",1); }
    if (GetCvar("samsara_doomguydefense") != 14)  { TakeInventory("PowerDefenseBuff14",1); }
    if (GetCvar("samsara_doomguydefense") != 15)  { TakeInventory("PowerDefenseBuff15",1); }
    if (GetCvar("samsara_doomguydefense") != 16)  { TakeInventory("PowerDefenseBuff16",1); }
    if (GetCvar("samsara_doomguydefense") != 17)  { TakeInventory("PowerDefenseBuff17",1); }
    if (GetCvar("samsara_doomguydefense") != 18)  { TakeInventory("PowerDefenseBuff18",1); }
    if (GetCvar("samsara_doomguydefense") != 19)  { TakeInventory("PowerDefenseBuff19",1); }
    if (GetCvar("samsara_doomguydefense") != 20)  { TakeInventory("PowerDefenseBuff20",1); }
	
    if (GetCvar("samsara_doomguydefense") == -10) { GiveInventory("PowerDefenseBuff-10",1); }
    if (GetCvar("samsara_doomguydefense") == -9)  { GiveInventory("PowerDefenseBuff-9",1); }
    if (GetCvar("samsara_doomguydefense") == -8)  { GiveInventory("PowerDefenseBuff-8",1); }
    if (GetCvar("samsara_doomguydefense") == -7)  { GiveInventory("PowerDefenseBuff-7",1); }
    if (GetCvar("samsara_doomguydefense") == -6)  { GiveInventory("PowerDefenseBuff-6",1); }
    if (GetCvar("samsara_doomguydefense") == -5)  { GiveInventory("PowerDefenseBuff-5",1); }
    if (GetCvar("samsara_doomguydefense") == -4)  { GiveInventory("PowerDefenseBuff-4",1); }
    if (GetCvar("samsara_doomguydefense") == -3)  { GiveInventory("PowerDefenseBuff-3",1); }
    if (GetCvar("samsara_doomguydefense") == -2)  { GiveInventory("PowerDefenseBuff-2",1); }
    if (GetCvar("samsara_doomguydefense") == -1)  { GiveInventory("PowerDefenseBuff-1",1); }
    if (GetCvar("samsara_doomguydefense") == 1)   { GiveInventory("PowerDefenseBuff1",1); }
    if (GetCvar("samsara_doomguydefense") == 2)   { GiveInventory("PowerDefenseBuff2",1); }
    if (GetCvar("samsara_doomguydefense") == 3)   { GiveInventory("PowerDefenseBuff3",1); }
    if (GetCvar("samsara_doomguydefense") == 4)   { GiveInventory("PowerDefenseBuff4",1); }
    if (GetCvar("samsara_doomguydefense") == 5)   { GiveInventory("PowerDefenseBuff5",1); }
    if (GetCvar("samsara_doomguydefense") == 6)   { GiveInventory("PowerDefenseBuff6",1); }
    if (GetCvar("samsara_doomguydefense") == 7)   { GiveInventory("PowerDefenseBuff7",1); }
    if (GetCvar("samsara_doomguydefense") == 8)   { GiveInventory("PowerDefenseBuff8",1); }
    if (GetCvar("samsara_doomguydefense") == 9)   { GiveInventory("PowerDefenseBuff9",1); }
    if (GetCvar("samsara_doomguydefense") == 10)  { GiveInventory("PowerDefenseBuff10",1); }
    if (GetCvar("samsara_doomguydefense") == 11)  { GiveInventory("PowerDefenseBuff11",1); }
    if (GetCvar("samsara_doomguydefense") == 12)  { GiveInventory("PowerDefenseBuff12",1); }
    if (GetCvar("samsara_doomguydefense") == 13)  { GiveInventory("PowerDefenseBuff13",1); }
    if (GetCvar("samsara_doomguydefense") == 14)  { GiveInventory("PowerDefenseBuff14",1); }
    if (GetCvar("samsara_doomguydefense") == 15)  { GiveInventory("PowerDefenseBuff15",1); }
    if (GetCvar("samsara_doomguydefense") == 16)  { GiveInventory("PowerDefenseBuff16",1); }
    if (GetCvar("samsara_doomguydefense") == 17)  { GiveInventory("PowerDefenseBuff17",1); }
    if (GetCvar("samsara_doomguydefense") == 18)  { GiveInventory("PowerDefenseBuff18",1); }
    if (GetCvar("samsara_doomguydefense") == 19)  { GiveInventory("PowerDefenseBuff19",1); }
    if (GetCvar("samsara_doomguydefense") == 20)  { GiveInventory("PowerDefenseBuff20",1); }
}
return 1;
}

function int HandleChexDamage(int respawning)
{
if (CheckInventory("ChexClass") == 1)
{
    if (GetCvar("samsara_chexdamage") != -10) { TakeInventory("PowerDamageBuff-10",1); }
    if (GetCvar("samsara_chexdamage") != -9)  { TakeInventory("PowerDamageBuff-9",1); }
    if (GetCvar("samsara_chexdamage") != -8)  { TakeInventory("PowerDamageBuff-8",1); }
    if (GetCvar("samsara_chexdamage") != -7)  { TakeInventory("PowerDamageBuff-7",1); }
    if (GetCvar("samsara_chexdamage") != -6)  { TakeInventory("PowerDamageBuff-6",1); }
    if (GetCvar("samsara_chexdamage") != -5)  { TakeInventory("PowerDamageBuff-5",1); }
    if (GetCvar("samsara_chexdamage") != -4)  { TakeInventory("PowerDamageBuff-4",1); }
    if (GetCvar("samsara_chexdamage") != -3)  { TakeInventory("PowerDamageBuff-3",1); }
    if (GetCvar("samsara_chexdamage") != -2)  { TakeInventory("PowerDamageBuff-2",1); }
    if (GetCvar("samsara_chexdamage") != -1)  { TakeInventory("PowerDamageBuff-1",1); }
    if (GetCvar("samsara_chexdamage") != 1)   { TakeInventory("PowerDamageBuff1",1); }
    if (GetCvar("samsara_chexdamage") != 2)   { TakeInventory("PowerDamageBuff2",1); }
    if (GetCvar("samsara_chexdamage") != 3)   { TakeInventory("PowerDamageBuff3",1); }
    if (GetCvar("samsara_chexdamage") != 4)   { TakeInventory("PowerDamageBuff4",1); }
    if (GetCvar("samsara_chexdamage") != 5)   { TakeInventory("PowerDamageBuff5",1); }
    if (GetCvar("samsara_chexdamage") != 6)   { TakeInventory("PowerDamageBuff6",1); }
    if (GetCvar("samsara_chexdamage") != 7)   { TakeInventory("PowerDamageBuff7",1); }
    if (GetCvar("samsara_chexdamage") != 8)   { TakeInventory("PowerDamageBuff8",1); }
    if (GetCvar("samsara_chexdamage") != 9)   { TakeInventory("PowerDamageBuff9",1); }
    if (GetCvar("samsara_chexdamage") != 10)  { TakeInventory("PowerDamageBuff10",1); }
    if (GetCvar("samsara_chexdamage") != 11)  { TakeInventory("PowerDamageBuff11",1); }
    if (GetCvar("samsara_chexdamage") != 12)  { TakeInventory("PowerDamageBuff12",1); }
    if (GetCvar("samsara_chexdamage") != 13)  { TakeInventory("PowerDamageBuff13",1); }
    if (GetCvar("samsara_chexdamage") != 14)  { TakeInventory("PowerDamageBuff14",1); }
    if (GetCvar("samsara_chexdamage") != 15)  { TakeInventory("PowerDamageBuff15",1); }
    if (GetCvar("samsara_chexdamage") != 16)  { TakeInventory("PowerDamageBuff16",1); }
    if (GetCvar("samsara_chexdamage") != 17)  { TakeInventory("PowerDamageBuff17",1); }
    if (GetCvar("samsara_chexdamage") != 18)  { TakeInventory("PowerDamageBuff18",1); }
    if (GetCvar("samsara_chexdamage") != 19)  { TakeInventory("PowerDamageBuff19",1); }
    if (GetCvar("samsara_chexdamage") != 20)  { TakeInventory("PowerDamageBuff20",1); }
    if (GetCvar("samsara_chexdamage") != 21)  { TakeInventory("PowerDamageBuff21",1); }
    if (GetCvar("samsara_chexdamage") != 22)  { TakeInventory("PowerDamageBuff22",1); }
    if (GetCvar("samsara_chexdamage") != 23)  { TakeInventory("PowerDamageBuff23",1); }
    if (GetCvar("samsara_chexdamage") != 24)  { TakeInventory("PowerDamageBuff24",1); }
    if (GetCvar("samsara_chexdamage") != 25)  { TakeInventory("PowerDamageBuff25",1); }
    if (GetCvar("samsara_chexdamage") != 26)  { TakeInventory("PowerDamageBuff26",1); }
    if (GetCvar("samsara_chexdamage") != 27)  { TakeInventory("PowerDamageBuff27",1); }
    if (GetCvar("samsara_chexdamage") != 28)  { TakeInventory("PowerDamageBuff28",1); }
    if (GetCvar("samsara_chexdamage") != 29)  { TakeInventory("PowerDamageBuff29",1); }
    if (GetCvar("samsara_chexdamage") != 30)  { TakeInventory("PowerDamageBuff30",1); }
	
    if (GetCvar("samsara_chexdamage") == -10) { GiveInventory("PowerDamageBuff-10",1); }
    if (GetCvar("samsara_chexdamage") == -9)  { GiveInventory("PowerDamageBuff-9",1); }
    if (GetCvar("samsara_chexdamage") == -8)  { GiveInventory("PowerDamageBuff-8",1); }
    if (GetCvar("samsara_chexdamage") == -7)  { GiveInventory("PowerDamageBuff-7",1); }
    if (GetCvar("samsara_chexdamage") == -6)  { GiveInventory("PowerDamageBuff-6",1); }
    if (GetCvar("samsara_chexdamage") == -5)  { GiveInventory("PowerDamageBuff-5",1); }
    if (GetCvar("samsara_chexdamage") == -4)  { GiveInventory("PowerDamageBuff-4",1); }
    if (GetCvar("samsara_chexdamage") == -3)  { GiveInventory("PowerDamageBuff-3",1); }
    if (GetCvar("samsara_chexdamage") == -2)  { GiveInventory("PowerDamageBuff-2",1); }
    if (GetCvar("samsara_chexdamage") == -1)  { GiveInventory("PowerDamageBuff-1",1); }
    if (GetCvar("samsara_chexdamage") == 1)   { GiveInventory("PowerDamageBuff1",1); }
    if (GetCvar("samsara_chexdamage") == 2)   { GiveInventory("PowerDamageBuff2",1); }
    if (GetCvar("samsara_chexdamage") == 3)   { GiveInventory("PowerDamageBuff3",1); }
    if (GetCvar("samsara_chexdamage") == 4)   { GiveInventory("PowerDamageBuff4",1); }
    if (GetCvar("samsara_chexdamage") == 5)   { GiveInventory("PowerDamageBuff5",1); }
    if (GetCvar("samsara_chexdamage") == 6)   { GiveInventory("PowerDamageBuff6",1); }
    if (GetCvar("samsara_chexdamage") == 7)   { GiveInventory("PowerDamageBuff7",1); }
    if (GetCvar("samsara_chexdamage") == 8)   { GiveInventory("PowerDamageBuff8",1); }
    if (GetCvar("samsara_chexdamage") == 9)   { GiveInventory("PowerDamageBuff9",1); }
    if (GetCvar("samsara_chexdamage") == 10)  { GiveInventory("PowerDamageBuff10",1); }
    if (GetCvar("samsara_chexdamage") == 11)  { GiveInventory("PowerDamageBuff11",1); }
    if (GetCvar("samsara_chexdamage") == 12)  { GiveInventory("PowerDamageBuff12",1); }
    if (GetCvar("samsara_chexdamage") == 13)  { GiveInventory("PowerDamageBuff13",1); }
    if (GetCvar("samsara_chexdamage") == 14)  { GiveInventory("PowerDamageBuff14",1); }
    if (GetCvar("samsara_chexdamage") == 15)  { GiveInventory("PowerDamageBuff15",1); }
    if (GetCvar("samsara_chexdamage") == 16)  { GiveInventory("PowerDamageBuff16",1); }
    if (GetCvar("samsara_chexdamage") == 17)  { GiveInventory("PowerDamageBuff17",1); }
    if (GetCvar("samsara_chexdamage") == 18)  { GiveInventory("PowerDamageBuff18",1); }
    if (GetCvar("samsara_chexdamage") == 19)  { GiveInventory("PowerDamageBuff19",1); }
    if (GetCvar("samsara_chexdamage") == 20)  { GiveInventory("PowerDamageBuff20",1); }
    if (GetCvar("samsara_chexdamage") == 21)  { GiveInventory("PowerDamageBuff21",1); }
    if (GetCvar("samsara_chexdamage") == 22)  { GiveInventory("PowerDamageBuff22",1); }
    if (GetCvar("samsara_chexdamage") == 23)  { GiveInventory("PowerDamageBuff23",1); }
    if (GetCvar("samsara_chexdamage") == 24)  { GiveInventory("PowerDamageBuff24",1); }
    if (GetCvar("samsara_chexdamage") == 25)  { GiveInventory("PowerDamageBuff25",1); }
    if (GetCvar("samsara_chexdamage") == 26)  { GiveInventory("PowerDamageBuff26",1); }
    if (GetCvar("samsara_chexdamage") == 27)  { GiveInventory("PowerDamageBuff27",1); }
    if (GetCvar("samsara_chexdamage") == 28)  { GiveInventory("PowerDamageBuff28",1); }
    if (GetCvar("samsara_chexdamage") == 29)  { GiveInventory("PowerDamageBuff29",1); }
    if (GetCvar("samsara_chexdamage") == 30)  { GiveInventory("PowerDamageBuff30",1); }
}
return 1;
}

function int HandleChexDefense(int respawning)
{
if (CheckInventory("ChexClass") == 1)
{
    if (GetCvar("samsara_chexdefense") != -10) { TakeInventory("PowerDefenseBuff-10",1); }
    if (GetCvar("samsara_chexdefense") != -9)  { TakeInventory("PowerDefenseBuff-9",1); }
    if (GetCvar("samsara_chexdefense") != -8)  { TakeInventory("PowerDefenseBuff-8",1); }
    if (GetCvar("samsara_chexdefense") != -7)  { TakeInventory("PowerDefenseBuff-7",1); }
    if (GetCvar("samsara_chexdefense") != -6)  { TakeInventory("PowerDefenseBuff-6",1); }
    if (GetCvar("samsara_chexdefense") != -5)  { TakeInventory("PowerDefenseBuff-5",1); }
    if (GetCvar("samsara_chexdefense") != -4)  { TakeInventory("PowerDefenseBuff-4",1); }
    if (GetCvar("samsara_chexdefense") != -3)  { TakeInventory("PowerDefenseBuff-3",1); }
    if (GetCvar("samsara_chexdefense") != -2)  { TakeInventory("PowerDefenseBuff-2",1); }
    if (GetCvar("samsara_chexdefense") != -1)  { TakeInventory("PowerDefenseBuff-1",1); }
    if (GetCvar("samsara_chexdefense") != 1)   { TakeInventory("PowerDefenseBuff1",1); }
    if (GetCvar("samsara_chexdefense") != 2)   { TakeInventory("PowerDefenseBuff2",1); }
    if (GetCvar("samsara_chexdefense") != 3)   { TakeInventory("PowerDefenseBuff3",1); }
    if (GetCvar("samsara_chexdefense") != 4)   { TakeInventory("PowerDefenseBuff4",1); }
    if (GetCvar("samsara_chexdefense") != 5)   { TakeInventory("PowerDefenseBuff5",1); }
    if (GetCvar("samsara_chexdefense") != 6)   { TakeInventory("PowerDefenseBuff6",1); }
    if (GetCvar("samsara_chexdefense") != 7)   { TakeInventory("PowerDefenseBuff7",1); }
    if (GetCvar("samsara_chexdefense") != 8)   { TakeInventory("PowerDefenseBuff8",1); }
    if (GetCvar("samsara_chexdefense") != 9)   { TakeInventory("PowerDefenseBuff9",1); }
    if (GetCvar("samsara_chexdefense") != 10)  { TakeInventory("PowerDefenseBuff10",1); }
    if (GetCvar("samsara_chexdefense") != 11)  { TakeInventory("PowerDefenseBuff11",1); }
    if (GetCvar("samsara_chexdefense") != 12)  { TakeInventory("PowerDefenseBuff12",1); }
    if (GetCvar("samsara_chexdefense") != 13)  { TakeInventory("PowerDefenseBuff13",1); }
    if (GetCvar("samsara_chexdefense") != 14)  { TakeInventory("PowerDefenseBuff14",1); }
    if (GetCvar("samsara_chexdefense") != 15)  { TakeInventory("PowerDefenseBuff15",1); }
    if (GetCvar("samsara_chexdefense") != 16)  { TakeInventory("PowerDefenseBuff16",1); }
    if (GetCvar("samsara_chexdefense") != 17)  { TakeInventory("PowerDefenseBuff17",1); }
    if (GetCvar("samsara_chexdefense") != 18)  { TakeInventory("PowerDefenseBuff18",1); }
    if (GetCvar("samsara_chexdefense") != 19)  { TakeInventory("PowerDefenseBuff19",1); }
    if (GetCvar("samsara_chexdefense") != 20)  { TakeInventory("PowerDefenseBuff20",1); }
	
    if (GetCvar("samsara_chexdefense") == -10) { GiveInventory("PowerDefenseBuff-10",1); }
    if (GetCvar("samsara_chexdefense") == -9)  { GiveInventory("PowerDefenseBuff-9",1); }
    if (GetCvar("samsara_chexdefense") == -8)  { GiveInventory("PowerDefenseBuff-8",1); }
    if (GetCvar("samsara_chexdefense") == -7)  { GiveInventory("PowerDefenseBuff-7",1); }
    if (GetCvar("samsara_chexdefense") == -6)  { GiveInventory("PowerDefenseBuff-6",1); }
    if (GetCvar("samsara_chexdefense") == -5)  { GiveInventory("PowerDefenseBuff-5",1); }
    if (GetCvar("samsara_chexdefense") == -4)  { GiveInventory("PowerDefenseBuff-4",1); }
    if (GetCvar("samsara_chexdefense") == -3)  { GiveInventory("PowerDefenseBuff-3",1); }
    if (GetCvar("samsara_chexdefense") == -2)  { GiveInventory("PowerDefenseBuff-2",1); }
    if (GetCvar("samsara_chexdefense") == -1)  { GiveInventory("PowerDefenseBuff-1",1); }
    if (GetCvar("samsara_chexdefense") == 1)   { GiveInventory("PowerDefenseBuff1",1); }
    if (GetCvar("samsara_chexdefense") == 2)   { GiveInventory("PowerDefenseBuff2",1); }
    if (GetCvar("samsara_chexdefense") == 3)   { GiveInventory("PowerDefenseBuff3",1); }
    if (GetCvar("samsara_chexdefense") == 4)   { GiveInventory("PowerDefenseBuff4",1); }
    if (GetCvar("samsara_chexdefense") == 5)   { GiveInventory("PowerDefenseBuff5",1); }
    if (GetCvar("samsara_chexdefense") == 6)   { GiveInventory("PowerDefenseBuff6",1); }
    if (GetCvar("samsara_chexdefense") == 7)   { GiveInventory("PowerDefenseBuff7",1); }
    if (GetCvar("samsara_chexdefense") == 8)   { GiveInventory("PowerDefenseBuff8",1); }
    if (GetCvar("samsara_chexdefense") == 9)   { GiveInventory("PowerDefenseBuff9",1); }
    if (GetCvar("samsara_chexdefense") == 10)  { GiveInventory("PowerDefenseBuff10",1); }
    if (GetCvar("samsara_chexdefense") == 11)  { GiveInventory("PowerDefenseBuff11",1); }
    if (GetCvar("samsara_chexdefense") == 12)  { GiveInventory("PowerDefenseBuff12",1); }
    if (GetCvar("samsara_chexdefense") == 13)  { GiveInventory("PowerDefenseBuff13",1); }
    if (GetCvar("samsara_chexdefense") == 14)  { GiveInventory("PowerDefenseBuff14",1); }
    if (GetCvar("samsara_chexdefense") == 15)  { GiveInventory("PowerDefenseBuff15",1); }
    if (GetCvar("samsara_chexdefense") == 16)  { GiveInventory("PowerDefenseBuff16",1); }
    if (GetCvar("samsara_chexdefense") == 17)  { GiveInventory("PowerDefenseBuff17",1); }
    if (GetCvar("samsara_chexdefense") == 18)  { GiveInventory("PowerDefenseBuff18",1); }
    if (GetCvar("samsara_chexdefense") == 19)  { GiveInventory("PowerDefenseBuff19",1); }
    if (GetCvar("samsara_chexdefense") == 20)  { GiveInventory("PowerDefenseBuff20",1); }
}
return 1;
}

function int HandleCorvusDamage(int respawning)
{
if (CheckInventory("CorvusClass") == 1)
{
    if (GetCvar("samsara_corvusdamage") != -10) { TakeInventory("PowerDamageBuff-10",1); }
    if (GetCvar("samsara_corvusdamage") != -9)  { TakeInventory("PowerDamageBuff-9",1); }
    if (GetCvar("samsara_corvusdamage") != -8)  { TakeInventory("PowerDamageBuff-8",1); }
    if (GetCvar("samsara_corvusdamage") != -7)  { TakeInventory("PowerDamageBuff-7",1); }
    if (GetCvar("samsara_corvusdamage") != -6)  { TakeInventory("PowerDamageBuff-6",1); }
    if (GetCvar("samsara_corvusdamage") != -5)  { TakeInventory("PowerDamageBuff-5",1); }
    if (GetCvar("samsara_corvusdamage") != -4)  { TakeInventory("PowerDamageBuff-4",1); }
    if (GetCvar("samsara_corvusdamage") != -3)  { TakeInventory("PowerDamageBuff-3",1); }
    if (GetCvar("samsara_corvusdamage") != -2)  { TakeInventory("PowerDamageBuff-2",1); }
    if (GetCvar("samsara_corvusdamage") != -1)  { TakeInventory("PowerDamageBuff-1",1); }
    if (GetCvar("samsara_corvusdamage") != 1)   { TakeInventory("PowerDamageBuff1",1); }
    if (GetCvar("samsara_corvusdamage") != 2)   { TakeInventory("PowerDamageBuff2",1); }
    if (GetCvar("samsara_corvusdamage") != 3)   { TakeInventory("PowerDamageBuff3",1); }
    if (GetCvar("samsara_corvusdamage") != 4)   { TakeInventory("PowerDamageBuff4",1); }
    if (GetCvar("samsara_corvusdamage") != 5)   { TakeInventory("PowerDamageBuff5",1); }
    if (GetCvar("samsara_corvusdamage") != 6)   { TakeInventory("PowerDamageBuff6",1); }
    if (GetCvar("samsara_corvusdamage") != 7)   { TakeInventory("PowerDamageBuff7",1); }
    if (GetCvar("samsara_corvusdamage") != 8)   { TakeInventory("PowerDamageBuff8",1); }
    if (GetCvar("samsara_corvusdamage") != 9)   { TakeInventory("PowerDamageBuff9",1); }
    if (GetCvar("samsara_corvusdamage") != 10)  { TakeInventory("PowerDamageBuff10",1); }
    if (GetCvar("samsara_corvusdamage") != 11)  { TakeInventory("PowerDamageBuff11",1); }
    if (GetCvar("samsara_corvusdamage") != 12)  { TakeInventory("PowerDamageBuff12",1); }
    if (GetCvar("samsara_corvusdamage") != 13)  { TakeInventory("PowerDamageBuff13",1); }
    if (GetCvar("samsara_corvusdamage") != 14)  { TakeInventory("PowerDamageBuff14",1); }
    if (GetCvar("samsara_corvusdamage") != 15)  { TakeInventory("PowerDamageBuff15",1); }
    if (GetCvar("samsara_corvusdamage") != 16)  { TakeInventory("PowerDamageBuff16",1); }
    if (GetCvar("samsara_corvusdamage") != 17)  { TakeInventory("PowerDamageBuff17",1); }
    if (GetCvar("samsara_corvusdamage") != 18)  { TakeInventory("PowerDamageBuff18",1); }
    if (GetCvar("samsara_corvusdamage") != 19)  { TakeInventory("PowerDamageBuff19",1); }
    if (GetCvar("samsara_corvusdamage") != 20)  { TakeInventory("PowerDamageBuff20",1); }
    if (GetCvar("samsara_corvusdamage") != 21)  { TakeInventory("PowerDamageBuff21",1); }
    if (GetCvar("samsara_corvusdamage") != 22)  { TakeInventory("PowerDamageBuff22",1); }
    if (GetCvar("samsara_corvusdamage") != 23)  { TakeInventory("PowerDamageBuff23",1); }
    if (GetCvar("samsara_corvusdamage") != 24)  { TakeInventory("PowerDamageBuff24",1); }
    if (GetCvar("samsara_corvusdamage") != 25)  { TakeInventory("PowerDamageBuff25",1); }
    if (GetCvar("samsara_corvusdamage") != 26)  { TakeInventory("PowerDamageBuff26",1); }
    if (GetCvar("samsara_corvusdamage") != 27)  { TakeInventory("PowerDamageBuff27",1); }
    if (GetCvar("samsara_corvusdamage") != 28)  { TakeInventory("PowerDamageBuff28",1); }
    if (GetCvar("samsara_corvusdamage") != 29)  { TakeInventory("PowerDamageBuff29",1); }
    if (GetCvar("samsara_corvusdamage") != 30)  { TakeInventory("PowerDamageBuff30",1); }
	
    if (GetCvar("samsara_corvusdamage") == -10) { GiveInventory("PowerDamageBuff-10",1); }
    if (GetCvar("samsara_corvusdamage") == -9)  { GiveInventory("PowerDamageBuff-9",1); }
    if (GetCvar("samsara_corvusdamage") == -8)  { GiveInventory("PowerDamageBuff-8",1); }
    if (GetCvar("samsara_corvusdamage") == -7)  { GiveInventory("PowerDamageBuff-7",1); }
    if (GetCvar("samsara_corvusdamage") == -6)  { GiveInventory("PowerDamageBuff-6",1); }
    if (GetCvar("samsara_corvusdamage") == -5)  { GiveInventory("PowerDamageBuff-5",1); }
    if (GetCvar("samsara_corvusdamage") == -4)  { GiveInventory("PowerDamageBuff-4",1); }
    if (GetCvar("samsara_corvusdamage") == -3)  { GiveInventory("PowerDamageBuff-3",1); }
    if (GetCvar("samsara_corvusdamage") == -2)  { GiveInventory("PowerDamageBuff-2",1); }
    if (GetCvar("samsara_corvusdamage") == -1)  { GiveInventory("PowerDamageBuff-1",1); }
    if (GetCvar("samsara_corvusdamage") == 1)   { GiveInventory("PowerDamageBuff1",1); }
    if (GetCvar("samsara_corvusdamage") == 2)   { GiveInventory("PowerDamageBuff2",1); }
    if (GetCvar("samsara_corvusdamage") == 3)   { GiveInventory("PowerDamageBuff3",1); }
    if (GetCvar("samsara_corvusdamage") == 4)   { GiveInventory("PowerDamageBuff4",1); }
    if (GetCvar("samsara_corvusdamage") == 5)   { GiveInventory("PowerDamageBuff5",1); }
    if (GetCvar("samsara_corvusdamage") == 6)   { GiveInventory("PowerDamageBuff6",1); }
    if (GetCvar("samsara_corvusdamage") == 7)   { GiveInventory("PowerDamageBuff7",1); }
    if (GetCvar("samsara_corvusdamage") == 8)   { GiveInventory("PowerDamageBuff8",1); }
    if (GetCvar("samsara_corvusdamage") == 9)   { GiveInventory("PowerDamageBuff9",1); }
    if (GetCvar("samsara_corvusdamage") == 10)  { GiveInventory("PowerDamageBuff10",1); }
    if (GetCvar("samsara_corvusdamage") == 11)  { GiveInventory("PowerDamageBuff11",1); }
    if (GetCvar("samsara_corvusdamage") == 12)  { GiveInventory("PowerDamageBuff12",1); }
    if (GetCvar("samsara_corvusdamage") == 13)  { GiveInventory("PowerDamageBuff13",1); }
    if (GetCvar("samsara_corvusdamage") == 14)  { GiveInventory("PowerDamageBuff14",1); }
    if (GetCvar("samsara_corvusdamage") == 15)  { GiveInventory("PowerDamageBuff15",1); }
    if (GetCvar("samsara_corvusdamage") == 16)  { GiveInventory("PowerDamageBuff16",1); }
    if (GetCvar("samsara_corvusdamage") == 17)  { GiveInventory("PowerDamageBuff17",1); }
    if (GetCvar("samsara_corvusdamage") == 18)  { GiveInventory("PowerDamageBuff18",1); }
    if (GetCvar("samsara_corvusdamage") == 19)  { GiveInventory("PowerDamageBuff19",1); }
    if (GetCvar("samsara_corvusdamage") == 20)  { GiveInventory("PowerDamageBuff20",1); }
    if (GetCvar("samsara_corvusdamage") == 21)  { GiveInventory("PowerDamageBuff21",1); }
    if (GetCvar("samsara_corvusdamage") == 22)  { GiveInventory("PowerDamageBuff22",1); }
    if (GetCvar("samsara_corvusdamage") == 23)  { GiveInventory("PowerDamageBuff23",1); }
    if (GetCvar("samsara_corvusdamage") == 24)  { GiveInventory("PowerDamageBuff24",1); }
    if (GetCvar("samsara_corvusdamage") == 25)  { GiveInventory("PowerDamageBuff25",1); }
    if (GetCvar("samsara_corvusdamage") == 26)  { GiveInventory("PowerDamageBuff26",1); }
    if (GetCvar("samsara_corvusdamage") == 27)  { GiveInventory("PowerDamageBuff27",1); }
    if (GetCvar("samsara_corvusdamage") == 28)  { GiveInventory("PowerDamageBuff28",1); }
    if (GetCvar("samsara_corvusdamage") == 29)  { GiveInventory("PowerDamageBuff29",1); }
    if (GetCvar("samsara_corvusdamage") == 30)  { GiveInventory("PowerDamageBuff30",1); }
}
return 1;
}

function int HandleCorvusDefense(int respawning)
{
if (CheckInventory("CorvusClass") == 1)
{
    if (GetCvar("samsara_corvusdefense") != -10) { TakeInventory("PowerDefenseBuff-10",1); }
    if (GetCvar("samsara_corvusdefense") != -9)  { TakeInventory("PowerDefenseBuff-9",1); }
    if (GetCvar("samsara_corvusdefense") != -8)  { TakeInventory("PowerDefenseBuff-8",1); }
    if (GetCvar("samsara_corvusdefense") != -7)  { TakeInventory("PowerDefenseBuff-7",1); }
    if (GetCvar("samsara_corvusdefense") != -6)  { TakeInventory("PowerDefenseBuff-6",1); }
    if (GetCvar("samsara_corvusdefense") != -5)  { TakeInventory("PowerDefenseBuff-5",1); }
    if (GetCvar("samsara_corvusdefense") != -4)  { TakeInventory("PowerDefenseBuff-4",1); }
    if (GetCvar("samsara_corvusdefense") != -3)  { TakeInventory("PowerDefenseBuff-3",1); }
    if (GetCvar("samsara_corvusdefense") != -2)  { TakeInventory("PowerDefenseBuff-2",1); }
    if (GetCvar("samsara_corvusdefense") != -1)  { TakeInventory("PowerDefenseBuff-1",1); }
    if (GetCvar("samsara_corvusdefense") != 1)   { TakeInventory("PowerDefenseBuff1",1); }
    if (GetCvar("samsara_corvusdefense") != 2)   { TakeInventory("PowerDefenseBuff2",1); }
    if (GetCvar("samsara_corvusdefense") != 3)   { TakeInventory("PowerDefenseBuff3",1); }
    if (GetCvar("samsara_corvusdefense") != 4)   { TakeInventory("PowerDefenseBuff4",1); }
    if (GetCvar("samsara_corvusdefense") != 5)   { TakeInventory("PowerDefenseBuff5",1); }
    if (GetCvar("samsara_corvusdefense") != 6)   { TakeInventory("PowerDefenseBuff6",1); }
    if (GetCvar("samsara_corvusdefense") != 7)   { TakeInventory("PowerDefenseBuff7",1); }
    if (GetCvar("samsara_corvusdefense") != 8)   { TakeInventory("PowerDefenseBuff8",1); }
    if (GetCvar("samsara_corvusdefense") != 9)   { TakeInventory("PowerDefenseBuff9",1); }
    if (GetCvar("samsara_corvusdefense") != 10)  { TakeInventory("PowerDefenseBuff10",1); }
    if (GetCvar("samsara_corvusdefense") != 11)  { TakeInventory("PowerDefenseBuff11",1); }
    if (GetCvar("samsara_corvusdefense") != 12)  { TakeInventory("PowerDefenseBuff12",1); }
    if (GetCvar("samsara_corvusdefense") != 13)  { TakeInventory("PowerDefenseBuff13",1); }
    if (GetCvar("samsara_corvusdefense") != 14)  { TakeInventory("PowerDefenseBuff14",1); }
    if (GetCvar("samsara_corvusdefense") != 15)  { TakeInventory("PowerDefenseBuff15",1); }
    if (GetCvar("samsara_corvusdefense") != 16)  { TakeInventory("PowerDefenseBuff16",1); }
    if (GetCvar("samsara_corvusdefense") != 17)  { TakeInventory("PowerDefenseBuff17",1); }
    if (GetCvar("samsara_corvusdefense") != 18)  { TakeInventory("PowerDefenseBuff18",1); }
    if (GetCvar("samsara_corvusdefense") != 19)  { TakeInventory("PowerDefenseBuff19",1); }
    if (GetCvar("samsara_corvusdefense") != 20)  { TakeInventory("PowerDefenseBuff20",1); }
	
    if (GetCvar("samsara_corvusdefense") == -10) { GiveInventory("PowerDefenseBuff-10",1); }
    if (GetCvar("samsara_corvusdefense") == -9)  { GiveInventory("PowerDefenseBuff-9",1); }
    if (GetCvar("samsara_corvusdefense") == -8)  { GiveInventory("PowerDefenseBuff-8",1); }
    if (GetCvar("samsara_corvusdefense") == -7)  { GiveInventory("PowerDefenseBuff-7",1); }
    if (GetCvar("samsara_corvusdefense") == -6)  { GiveInventory("PowerDefenseBuff-6",1); }
    if (GetCvar("samsara_corvusdefense") == -5)  { GiveInventory("PowerDefenseBuff-5",1); }
    if (GetCvar("samsara_corvusdefense") == -4)  { GiveInventory("PowerDefenseBuff-4",1); }
    if (GetCvar("samsara_corvusdefense") == -3)  { GiveInventory("PowerDefenseBuff-3",1); }
    if (GetCvar("samsara_corvusdefense") == -2)  { GiveInventory("PowerDefenseBuff-2",1); }
    if (GetCvar("samsara_corvusdefense") == -1)  { GiveInventory("PowerDefenseBuff-1",1); }
    if (GetCvar("samsara_corvusdefense") == 1)   { GiveInventory("PowerDefenseBuff1",1); }
    if (GetCvar("samsara_corvusdefense") == 2)   { GiveInventory("PowerDefenseBuff2",1); }
    if (GetCvar("samsara_corvusdefense") == 3)   { GiveInventory("PowerDefenseBuff3",1); }
    if (GetCvar("samsara_corvusdefense") == 4)   { GiveInventory("PowerDefenseBuff4",1); }
    if (GetCvar("samsara_corvusdefense") == 5)   { GiveInventory("PowerDefenseBuff5",1); }
    if (GetCvar("samsara_corvusdefense") == 6)   { GiveInventory("PowerDefenseBuff6",1); }
    if (GetCvar("samsara_corvusdefense") == 7)   { GiveInventory("PowerDefenseBuff7",1); }
    if (GetCvar("samsara_corvusdefense") == 8)   { GiveInventory("PowerDefenseBuff8",1); }
    if (GetCvar("samsara_corvusdefense") == 9)   { GiveInventory("PowerDefenseBuff9",1); }
    if (GetCvar("samsara_corvusdefense") == 10)  { GiveInventory("PowerDefenseBuff10",1); }
    if (GetCvar("samsara_corvusdefense") == 11)  { GiveInventory("PowerDefenseBuff11",1); }
    if (GetCvar("samsara_corvusdefense") == 12)  { GiveInventory("PowerDefenseBuff12",1); }
    if (GetCvar("samsara_corvusdefense") == 13)  { GiveInventory("PowerDefenseBuff13",1); }
    if (GetCvar("samsara_corvusdefense") == 14)  { GiveInventory("PowerDefenseBuff14",1); }
    if (GetCvar("samsara_corvusdefense") == 15)  { GiveInventory("PowerDefenseBuff15",1); }
    if (GetCvar("samsara_corvusdefense") == 16)  { GiveInventory("PowerDefenseBuff16",1); }
    if (GetCvar("samsara_corvusdefense") == 17)  { GiveInventory("PowerDefenseBuff17",1); }
    if (GetCvar("samsara_corvusdefense") == 18)  { GiveInventory("PowerDefenseBuff18",1); }
    if (GetCvar("samsara_corvusdefense") == 19)  { GiveInventory("PowerDefenseBuff19",1); }
    if (GetCvar("samsara_corvusdefense") == 20)  { GiveInventory("PowerDefenseBuff20",1); }
}
return 1;
}

function int HandleBJDamage(int respawning)
{
if (CheckInventory("WolfenClass") == 1)
{
    if (GetCvar("samsara_bjdamage") != -10) { TakeInventory("PowerDamageBuff-10",1); }
    if (GetCvar("samsara_bjdamage") != -9)  { TakeInventory("PowerDamageBuff-9",1); }
    if (GetCvar("samsara_bjdamage") != -8)  { TakeInventory("PowerDamageBuff-8",1); }
    if (GetCvar("samsara_bjdamage") != -7)  { TakeInventory("PowerDamageBuff-7",1); }
    if (GetCvar("samsara_bjdamage") != -6)  { TakeInventory("PowerDamageBuff-6",1); }
    if (GetCvar("samsara_bjdamage") != -5)  { TakeInventory("PowerDamageBuff-5",1); }
    if (GetCvar("samsara_bjdamage") != -4)  { TakeInventory("PowerDamageBuff-4",1); }
    if (GetCvar("samsara_bjdamage") != -3)  { TakeInventory("PowerDamageBuff-3",1); }
    if (GetCvar("samsara_bjdamage") != -2)  { TakeInventory("PowerDamageBuff-2",1); }
    if (GetCvar("samsara_bjdamage") != -1)  { TakeInventory("PowerDamageBuff-1",1); }
    if (GetCvar("samsara_bjdamage") != 1)   { TakeInventory("PowerDamageBuff1",1); }
    if (GetCvar("samsara_bjdamage") != 2)   { TakeInventory("PowerDamageBuff2",1); }
    if (GetCvar("samsara_bjdamage") != 3)   { TakeInventory("PowerDamageBuff3",1); }
    if (GetCvar("samsara_bjdamage") != 4)   { TakeInventory("PowerDamageBuff4",1); }
    if (GetCvar("samsara_bjdamage") != 5)   { TakeInventory("PowerDamageBuff5",1); }
    if (GetCvar("samsara_bjdamage") != 6)   { TakeInventory("PowerDamageBuff6",1); }
    if (GetCvar("samsara_bjdamage") != 7)   { TakeInventory("PowerDamageBuff7",1); }
    if (GetCvar("samsara_bjdamage") != 8)   { TakeInventory("PowerDamageBuff8",1); }
    if (GetCvar("samsara_bjdamage") != 9)   { TakeInventory("PowerDamageBuff9",1); }
    if (GetCvar("samsara_bjdamage") != 10)  { TakeInventory("PowerDamageBuff10",1); }
    if (GetCvar("samsara_bjdamage") != 11)  { TakeInventory("PowerDamageBuff11",1); }
    if (GetCvar("samsara_bjdamage") != 12)  { TakeInventory("PowerDamageBuff12",1); }
    if (GetCvar("samsara_bjdamage") != 13)  { TakeInventory("PowerDamageBuff13",1); }
    if (GetCvar("samsara_bjdamage") != 14)  { TakeInventory("PowerDamageBuff14",1); }
    if (GetCvar("samsara_bjdamage") != 15)  { TakeInventory("PowerDamageBuff15",1); }
    if (GetCvar("samsara_bjdamage") != 16)  { TakeInventory("PowerDamageBuff16",1); }
    if (GetCvar("samsara_bjdamage") != 17)  { TakeInventory("PowerDamageBuff17",1); }
    if (GetCvar("samsara_bjdamage") != 18)  { TakeInventory("PowerDamageBuff18",1); }
    if (GetCvar("samsara_bjdamage") != 19)  { TakeInventory("PowerDamageBuff19",1); }
    if (GetCvar("samsara_bjdamage") != 20)  { TakeInventory("PowerDamageBuff20",1); }
    if (GetCvar("samsara_bjdamage") != 21)  { TakeInventory("PowerDamageBuff21",1); }
    if (GetCvar("samsara_bjdamage") != 22)  { TakeInventory("PowerDamageBuff22",1); }
    if (GetCvar("samsara_bjdamage") != 23)  { TakeInventory("PowerDamageBuff23",1); }
    if (GetCvar("samsara_bjdamage") != 24)  { TakeInventory("PowerDamageBuff24",1); }
    if (GetCvar("samsara_bjdamage") != 25)  { TakeInventory("PowerDamageBuff25",1); }
    if (GetCvar("samsara_bjdamage") != 26)  { TakeInventory("PowerDamageBuff26",1); }
    if (GetCvar("samsara_bjdamage") != 27)  { TakeInventory("PowerDamageBuff27",1); }
    if (GetCvar("samsara_bjdamage") != 28)  { TakeInventory("PowerDamageBuff28",1); }
    if (GetCvar("samsara_bjdamage") != 29)  { TakeInventory("PowerDamageBuff29",1); }
    if (GetCvar("samsara_bjdamage") != 30)  { TakeInventory("PowerDamageBuff30",1); }
	
    if (GetCvar("samsara_bjdamage") == -10) { GiveInventory("PowerDamageBuff-10",1); }
    if (GetCvar("samsara_bjdamage") == -9)  { GiveInventory("PowerDamageBuff-9",1); }
    if (GetCvar("samsara_bjdamage") == -8)  { GiveInventory("PowerDamageBuff-8",1); }
    if (GetCvar("samsara_bjdamage") == -7)  { GiveInventory("PowerDamageBuff-7",1); }
    if (GetCvar("samsara_bjdamage") == -6)  { GiveInventory("PowerDamageBuff-6",1); }
    if (GetCvar("samsara_bjdamage") == -5)  { GiveInventory("PowerDamageBuff-5",1); }
    if (GetCvar("samsara_bjdamage") == -4)  { GiveInventory("PowerDamageBuff-4",1); }
    if (GetCvar("samsara_bjdamage") == -3)  { GiveInventory("PowerDamageBuff-3",1); }
    if (GetCvar("samsara_bjdamage") == -2)  { GiveInventory("PowerDamageBuff-2",1); }
    if (GetCvar("samsara_bjdamage") == -1)  { GiveInventory("PowerDamageBuff-1",1); }
    if (GetCvar("samsara_bjdamage") == 1)   { GiveInventory("PowerDamageBuff1",1); }
    if (GetCvar("samsara_bjdamage") == 2)   { GiveInventory("PowerDamageBuff2",1); }
    if (GetCvar("samsara_bjdamage") == 3)   { GiveInventory("PowerDamageBuff3",1); }
    if (GetCvar("samsara_bjdamage") == 4)   { GiveInventory("PowerDamageBuff4",1); }
    if (GetCvar("samsara_bjdamage") == 5)   { GiveInventory("PowerDamageBuff5",1); }
    if (GetCvar("samsara_bjdamage") == 6)   { GiveInventory("PowerDamageBuff6",1); }
    if (GetCvar("samsara_bjdamage") == 7)   { GiveInventory("PowerDamageBuff7",1); }
    if (GetCvar("samsara_bjdamage") == 8)   { GiveInventory("PowerDamageBuff8",1); }
    if (GetCvar("samsara_bjdamage") == 9)   { GiveInventory("PowerDamageBuff9",1); }
    if (GetCvar("samsara_bjdamage") == 10)  { GiveInventory("PowerDamageBuff10",1); }
    if (GetCvar("samsara_bjdamage") == 11)  { GiveInventory("PowerDamageBuff11",1); }
    if (GetCvar("samsara_bjdamage") == 12)  { GiveInventory("PowerDamageBuff12",1); }
    if (GetCvar("samsara_bjdamage") == 13)  { GiveInventory("PowerDamageBuff13",1); }
    if (GetCvar("samsara_bjdamage") == 14)  { GiveInventory("PowerDamageBuff14",1); }
    if (GetCvar("samsara_bjdamage") == 15)  { GiveInventory("PowerDamageBuff15",1); }
    if (GetCvar("samsara_bjdamage") == 16)  { GiveInventory("PowerDamageBuff16",1); }
    if (GetCvar("samsara_bjdamage") == 17)  { GiveInventory("PowerDamageBuff17",1); }
    if (GetCvar("samsara_bjdamage") == 18)  { GiveInventory("PowerDamageBuff18",1); }
    if (GetCvar("samsara_bjdamage") == 19)  { GiveInventory("PowerDamageBuff19",1); }
    if (GetCvar("samsara_bjdamage") == 20)  { GiveInventory("PowerDamageBuff20",1); }
    if (GetCvar("samsara_bjdamage") == 21)  { GiveInventory("PowerDamageBuff21",1); }
    if (GetCvar("samsara_bjdamage") == 22)  { GiveInventory("PowerDamageBuff22",1); }
    if (GetCvar("samsara_bjdamage") == 23)  { GiveInventory("PowerDamageBuff23",1); }
    if (GetCvar("samsara_bjdamage") == 24)  { GiveInventory("PowerDamageBuff24",1); }
    if (GetCvar("samsara_bjdamage") == 25)  { GiveInventory("PowerDamageBuff25",1); }
    if (GetCvar("samsara_bjdamage") == 26)  { GiveInventory("PowerDamageBuff26",1); }
    if (GetCvar("samsara_bjdamage") == 27)  { GiveInventory("PowerDamageBuff27",1); }
    if (GetCvar("samsara_bjdamage") == 28)  { GiveInventory("PowerDamageBuff28",1); }
    if (GetCvar("samsara_bjdamage") == 29)  { GiveInventory("PowerDamageBuff29",1); }
    if (GetCvar("samsara_bjdamage") == 30)  { GiveInventory("PowerDamageBuff30",1); }
}
return 1;
}

function int HandleBJDefense(int respawning)
{
if (CheckInventory("WolfenClass") == 1)
{
    if (GetCvar("samsara_bjdefense") != -10) { TakeInventory("PowerDefenseBuff-10",1); }
    if (GetCvar("samsara_bjdefense") != -9)  { TakeInventory("PowerDefenseBuff-9",1); }
    if (GetCvar("samsara_bjdefense") != -8)  { TakeInventory("PowerDefenseBuff-8",1); }
    if (GetCvar("samsara_bjdefense") != -7)  { TakeInventory("PowerDefenseBuff-7",1); }
    if (GetCvar("samsara_bjdefense") != -6)  { TakeInventory("PowerDefenseBuff-6",1); }
    if (GetCvar("samsara_bjdefense") != -5)  { TakeInventory("PowerDefenseBuff-5",1); }
    if (GetCvar("samsara_bjdefense") != -4)  { TakeInventory("PowerDefenseBuff-4",1); }
    if (GetCvar("samsara_bjdefense") != -3)  { TakeInventory("PowerDefenseBuff-3",1); }
    if (GetCvar("samsara_bjdefense") != -2)  { TakeInventory("PowerDefenseBuff-2",1); }
    if (GetCvar("samsara_bjdefense") != -1)  { TakeInventory("PowerDefenseBuff-1",1); }
    if (GetCvar("samsara_bjdefense") != 1)   { TakeInventory("PowerDefenseBuff1",1); }
    if (GetCvar("samsara_bjdefense") != 2)   { TakeInventory("PowerDefenseBuff2",1); }
    if (GetCvar("samsara_bjdefense") != 3)   { TakeInventory("PowerDefenseBuff3",1); }
    if (GetCvar("samsara_bjdefense") != 4)   { TakeInventory("PowerDefenseBuff4",1); }
    if (GetCvar("samsara_bjdefense") != 5)   { TakeInventory("PowerDefenseBuff5",1); }
    if (GetCvar("samsara_bjdefense") != 6)   { TakeInventory("PowerDefenseBuff6",1); }
    if (GetCvar("samsara_bjdefense") != 7)   { TakeInventory("PowerDefenseBuff7",1); }
    if (GetCvar("samsara_bjdefense") != 8)   { TakeInventory("PowerDefenseBuff8",1); }
    if (GetCvar("samsara_bjdefense") != 9)   { TakeInventory("PowerDefenseBuff9",1); }
    if (GetCvar("samsara_bjdefense") != 10)  { TakeInventory("PowerDefenseBuff10",1); }
    if (GetCvar("samsara_bjdefense") != 11)  { TakeInventory("PowerDefenseBuff11",1); }
    if (GetCvar("samsara_bjdefense") != 12)  { TakeInventory("PowerDefenseBuff12",1); }
    if (GetCvar("samsara_bjdefense") != 13)  { TakeInventory("PowerDefenseBuff13",1); }
    if (GetCvar("samsara_bjdefense") != 14)  { TakeInventory("PowerDefenseBuff14",1); }
    if (GetCvar("samsara_bjdefense") != 15)  { TakeInventory("PowerDefenseBuff15",1); }
    if (GetCvar("samsara_bjdefense") != 16)  { TakeInventory("PowerDefenseBuff16",1); }
    if (GetCvar("samsara_bjdefense") != 17)  { TakeInventory("PowerDefenseBuff17",1); }
    if (GetCvar("samsara_bjdefense") != 18)  { TakeInventory("PowerDefenseBuff18",1); }
    if (GetCvar("samsara_bjdefense") != 19)  { TakeInventory("PowerDefenseBuff19",1); }
    if (GetCvar("samsara_bjdefense") != 20)  { TakeInventory("PowerDefenseBuff20",1); }
	
    if (GetCvar("samsara_bjdefense") == -10) { GiveInventory("PowerDefenseBuff-10",1); }
    if (GetCvar("samsara_bjdefense") == -9)  { GiveInventory("PowerDefenseBuff-9",1); }
    if (GetCvar("samsara_bjdefense") == -8)  { GiveInventory("PowerDefenseBuff-8",1); }
    if (GetCvar("samsara_bjdefense") == -7)  { GiveInventory("PowerDefenseBuff-7",1); }
    if (GetCvar("samsara_bjdefense") == -6)  { GiveInventory("PowerDefenseBuff-6",1); }
    if (GetCvar("samsara_bjdefense") == -5)  { GiveInventory("PowerDefenseBuff-5",1); }
    if (GetCvar("samsara_bjdefense") == -4)  { GiveInventory("PowerDefenseBuff-4",1); }
    if (GetCvar("samsara_bjdefense") == -3)  { GiveInventory("PowerDefenseBuff-3",1); }
    if (GetCvar("samsara_bjdefense") == -2)  { GiveInventory("PowerDefenseBuff-2",1); }
    if (GetCvar("samsara_bjdefense") == -1)  { GiveInventory("PowerDefenseBuff-1",1); }
    if (GetCvar("samsara_bjdefense") == 1)   { GiveInventory("PowerDefenseBuff1",1); }
    if (GetCvar("samsara_bjdefense") == 2)   { GiveInventory("PowerDefenseBuff2",1); }
    if (GetCvar("samsara_bjdefense") == 3)   { GiveInventory("PowerDefenseBuff3",1); }
    if (GetCvar("samsara_bjdefense") == 4)   { GiveInventory("PowerDefenseBuff4",1); }
    if (GetCvar("samsara_bjdefense") == 5)   { GiveInventory("PowerDefenseBuff5",1); }
    if (GetCvar("samsara_bjdefense") == 6)   { GiveInventory("PowerDefenseBuff6",1); }
    if (GetCvar("samsara_bjdefense") == 7)   { GiveInventory("PowerDefenseBuff7",1); }
    if (GetCvar("samsara_bjdefense") == 8)   { GiveInventory("PowerDefenseBuff8",1); }
    if (GetCvar("samsara_bjdefense") == 9)   { GiveInventory("PowerDefenseBuff9",1); }
    if (GetCvar("samsara_bjdefense") == 10)  { GiveInventory("PowerDefenseBuff10",1); }
    if (GetCvar("samsara_bjdefense") == 11)  { GiveInventory("PowerDefenseBuff11",1); }
    if (GetCvar("samsara_bjdefense") == 12)  { GiveInventory("PowerDefenseBuff12",1); }
    if (GetCvar("samsara_bjdefense") == 13)  { GiveInventory("PowerDefenseBuff13",1); }
    if (GetCvar("samsara_bjdefense") == 14)  { GiveInventory("PowerDefenseBuff14",1); }
    if (GetCvar("samsara_bjdefense") == 15)  { GiveInventory("PowerDefenseBuff15",1); }
    if (GetCvar("samsara_bjdefense") == 16)  { GiveInventory("PowerDefenseBuff16",1); }
    if (GetCvar("samsara_bjdefense") == 17)  { GiveInventory("PowerDefenseBuff17",1); }
    if (GetCvar("samsara_bjdefense") == 18)  { GiveInventory("PowerDefenseBuff18",1); }
    if (GetCvar("samsara_bjdefense") == 19)  { GiveInventory("PowerDefenseBuff19",1); }
    if (GetCvar("samsara_bjdefense") == 20)  { GiveInventory("PowerDefenseBuff20",1); }
}
return 1;
}

function int HandlePariasDamage(int respawning)
{
if (CheckInventory("HexenClass") == 1)
{
    if (GetCvar("samsara_Pariasdamage") != -10) { TakeInventory("PowerDamageBuff-10",1); }
    if (GetCvar("samsara_Pariasdamage") != -9)  { TakeInventory("PowerDamageBuff-9",1); }
    if (GetCvar("samsara_Pariasdamage") != -8)  { TakeInventory("PowerDamageBuff-8",1); }
    if (GetCvar("samsara_Pariasdamage") != -7)  { TakeInventory("PowerDamageBuff-7",1); }
    if (GetCvar("samsara_Pariasdamage") != -6)  { TakeInventory("PowerDamageBuff-6",1); }
    if (GetCvar("samsara_Pariasdamage") != -5)  { TakeInventory("PowerDamageBuff-5",1); }
    if (GetCvar("samsara_Pariasdamage") != -4)  { TakeInventory("PowerDamageBuff-4",1); }
    if (GetCvar("samsara_Pariasdamage") != -3)  { TakeInventory("PowerDamageBuff-3",1); }
    if (GetCvar("samsara_Pariasdamage") != -2)  { TakeInventory("PowerDamageBuff-2",1); }
    if (GetCvar("samsara_Pariasdamage") != -1)  { TakeInventory("PowerDamageBuff-1",1); }
    if (GetCvar("samsara_Pariasdamage") != 1)   { TakeInventory("PowerDamageBuff1",1); }
    if (GetCvar("samsara_Pariasdamage") != 2)   { TakeInventory("PowerDamageBuff2",1); }
    if (GetCvar("samsara_Pariasdamage") != 3)   { TakeInventory("PowerDamageBuff3",1); }
    if (GetCvar("samsara_Pariasdamage") != 4)   { TakeInventory("PowerDamageBuff4",1); }
    if (GetCvar("samsara_Pariasdamage") != 5)   { TakeInventory("PowerDamageBuff5",1); }
    if (GetCvar("samsara_Pariasdamage") != 6)   { TakeInventory("PowerDamageBuff6",1); }
    if (GetCvar("samsara_Pariasdamage") != 7)   { TakeInventory("PowerDamageBuff7",1); }
    if (GetCvar("samsara_Pariasdamage") != 8)   { TakeInventory("PowerDamageBuff8",1); }
    if (GetCvar("samsara_Pariasdamage") != 9)   { TakeInventory("PowerDamageBuff9",1); }
    if (GetCvar("samsara_Pariasdamage") != 10)  { TakeInventory("PowerDamageBuff10",1); }
    if (GetCvar("samsara_Pariasdamage") != 11)  { TakeInventory("PowerDamageBuff11",1); }
    if (GetCvar("samsara_Pariasdamage") != 12)  { TakeInventory("PowerDamageBuff12",1); }
    if (GetCvar("samsara_Pariasdamage") != 13)  { TakeInventory("PowerDamageBuff13",1); }
    if (GetCvar("samsara_Pariasdamage") != 14)  { TakeInventory("PowerDamageBuff14",1); }
    if (GetCvar("samsara_Pariasdamage") != 15)  { TakeInventory("PowerDamageBuff15",1); }
    if (GetCvar("samsara_Pariasdamage") != 16)  { TakeInventory("PowerDamageBuff16",1); }
    if (GetCvar("samsara_Pariasdamage") != 17)  { TakeInventory("PowerDamageBuff17",1); }
    if (GetCvar("samsara_Pariasdamage") != 18)  { TakeInventory("PowerDamageBuff18",1); }
    if (GetCvar("samsara_Pariasdamage") != 19)  { TakeInventory("PowerDamageBuff19",1); }
    if (GetCvar("samsara_Pariasdamage") != 20)  { TakeInventory("PowerDamageBuff20",1); }
    if (GetCvar("samsara_Pariasdamage") != 21)  { TakeInventory("PowerDamageBuff21",1); }
    if (GetCvar("samsara_Pariasdamage") != 22)  { TakeInventory("PowerDamageBuff22",1); }
    if (GetCvar("samsara_Pariasdamage") != 23)  { TakeInventory("PowerDamageBuff23",1); }
    if (GetCvar("samsara_Pariasdamage") != 24)  { TakeInventory("PowerDamageBuff24",1); }
    if (GetCvar("samsara_Pariasdamage") != 25)  { TakeInventory("PowerDamageBuff25",1); }
    if (GetCvar("samsara_Pariasdamage") != 26)  { TakeInventory("PowerDamageBuff26",1); }
    if (GetCvar("samsara_Pariasdamage") != 27)  { TakeInventory("PowerDamageBuff27",1); }
    if (GetCvar("samsara_Pariasdamage") != 28)  { TakeInventory("PowerDamageBuff28",1); }
    if (GetCvar("samsara_Pariasdamage") != 29)  { TakeInventory("PowerDamageBuff29",1); }
    if (GetCvar("samsara_Pariasdamage") != 30)  { TakeInventory("PowerDamageBuff30",1); }
	
    if (GetCvar("samsara_Pariasdamage") == -10) { GiveInventory("PowerDamageBuff-10",1); }
    if (GetCvar("samsara_Pariasdamage") == -9)  { GiveInventory("PowerDamageBuff-9",1); }
    if (GetCvar("samsara_Pariasdamage") == -8)  { GiveInventory("PowerDamageBuff-8",1); }
    if (GetCvar("samsara_Pariasdamage") == -7)  { GiveInventory("PowerDamageBuff-7",1); }
    if (GetCvar("samsara_Pariasdamage") == -6)  { GiveInventory("PowerDamageBuff-6",1); }
    if (GetCvar("samsara_Pariasdamage") == -5)  { GiveInventory("PowerDamageBuff-5",1); }
    if (GetCvar("samsara_Pariasdamage") == -4)  { GiveInventory("PowerDamageBuff-4",1); }
    if (GetCvar("samsara_Pariasdamage") == -3)  { GiveInventory("PowerDamageBuff-3",1); }
    if (GetCvar("samsara_Pariasdamage") == -2)  { GiveInventory("PowerDamageBuff-2",1); }
    if (GetCvar("samsara_Pariasdamage") == -1)  { GiveInventory("PowerDamageBuff-1",1); }
    if (GetCvar("samsara_Pariasdamage") == 1)   { GiveInventory("PowerDamageBuff1",1); }
    if (GetCvar("samsara_Pariasdamage") == 2)   { GiveInventory("PowerDamageBuff2",1); }
    if (GetCvar("samsara_Pariasdamage") == 3)   { GiveInventory("PowerDamageBuff3",1); }
    if (GetCvar("samsara_Pariasdamage") == 4)   { GiveInventory("PowerDamageBuff4",1); }
    if (GetCvar("samsara_Pariasdamage") == 5)   { GiveInventory("PowerDamageBuff5",1); }
    if (GetCvar("samsara_Pariasdamage") == 6)   { GiveInventory("PowerDamageBuff6",1); }
    if (GetCvar("samsara_Pariasdamage") == 7)   { GiveInventory("PowerDamageBuff7",1); }
    if (GetCvar("samsara_Pariasdamage") == 8)   { GiveInventory("PowerDamageBuff8",1); }
    if (GetCvar("samsara_Pariasdamage") == 9)   { GiveInventory("PowerDamageBuff9",1); }
    if (GetCvar("samsara_Pariasdamage") == 10)  { GiveInventory("PowerDamageBuff10",1); }
    if (GetCvar("samsara_Pariasdamage") == 11)  { GiveInventory("PowerDamageBuff11",1); }
    if (GetCvar("samsara_Pariasdamage") == 12)  { GiveInventory("PowerDamageBuff12",1); }
    if (GetCvar("samsara_Pariasdamage") == 13)  { GiveInventory("PowerDamageBuff13",1); }
    if (GetCvar("samsara_Pariasdamage") == 14)  { GiveInventory("PowerDamageBuff14",1); }
    if (GetCvar("samsara_Pariasdamage") == 15)  { GiveInventory("PowerDamageBuff15",1); }
    if (GetCvar("samsara_Pariasdamage") == 16)  { GiveInventory("PowerDamageBuff16",1); }
    if (GetCvar("samsara_Pariasdamage") == 17)  { GiveInventory("PowerDamageBuff17",1); }
    if (GetCvar("samsara_Pariasdamage") == 18)  { GiveInventory("PowerDamageBuff18",1); }
    if (GetCvar("samsara_Pariasdamage") == 19)  { GiveInventory("PowerDamageBuff19",1); }
    if (GetCvar("samsara_Pariasdamage") == 20)  { GiveInventory("PowerDamageBuff20",1); }
    if (GetCvar("samsara_Pariasdamage") == 21)  { GiveInventory("PowerDamageBuff21",1); }
    if (GetCvar("samsara_Pariasdamage") == 22)  { GiveInventory("PowerDamageBuff22",1); }
    if (GetCvar("samsara_Pariasdamage") == 23)  { GiveInventory("PowerDamageBuff23",1); }
    if (GetCvar("samsara_Pariasdamage") == 24)  { GiveInventory("PowerDamageBuff24",1); }
    if (GetCvar("samsara_Pariasdamage") == 25)  { GiveInventory("PowerDamageBuff25",1); }
    if (GetCvar("samsara_Pariasdamage") == 26)  { GiveInventory("PowerDamageBuff26",1); }
    if (GetCvar("samsara_Pariasdamage") == 27)  { GiveInventory("PowerDamageBuff27",1); }
    if (GetCvar("samsara_Pariasdamage") == 28)  { GiveInventory("PowerDamageBuff28",1); }
    if (GetCvar("samsara_Pariasdamage") == 29)  { GiveInventory("PowerDamageBuff29",1); }
    if (GetCvar("samsara_Pariasdamage") == 30)  { GiveInventory("PowerDamageBuff30",1); }
}
return 1;
}

function int HandlePariasDefense(int respawning)
{
if (CheckInventory("HexenClass") == 1)
{
    if (GetCvar("samsara_Pariasdefense") != -10) { TakeInventory("PowerDefenseBuff-10",1); }
    if (GetCvar("samsara_Pariasdefense") != -9)  { TakeInventory("PowerDefenseBuff-9",1); }
    if (GetCvar("samsara_Pariasdefense") != -8)  { TakeInventory("PowerDefenseBuff-8",1); }
    if (GetCvar("samsara_Pariasdefense") != -7)  { TakeInventory("PowerDefenseBuff-7",1); }
    if (GetCvar("samsara_Pariasdefense") != -6)  { TakeInventory("PowerDefenseBuff-6",1); }
    if (GetCvar("samsara_Pariasdefense") != -5)  { TakeInventory("PowerDefenseBuff-5",1); }
    if (GetCvar("samsara_Pariasdefense") != -4)  { TakeInventory("PowerDefenseBuff-4",1); }
    if (GetCvar("samsara_Pariasdefense") != -3)  { TakeInventory("PowerDefenseBuff-3",1); }
    if (GetCvar("samsara_Pariasdefense") != -2)  { TakeInventory("PowerDefenseBuff-2",1); }
    if (GetCvar("samsara_Pariasdefense") != -1)  { TakeInventory("PowerDefenseBuff-1",1); }
    if (GetCvar("samsara_Pariasdefense") != 1)   { TakeInventory("PowerDefenseBuff1",1); }
    if (GetCvar("samsara_Pariasdefense") != 2)   { TakeInventory("PowerDefenseBuff2",1); }
    if (GetCvar("samsara_Pariasdefense") != 3)   { TakeInventory("PowerDefenseBuff3",1); }
    if (GetCvar("samsara_Pariasdefense") != 4)   { TakeInventory("PowerDefenseBuff4",1); }
    if (GetCvar("samsara_Pariasdefense") != 5)   { TakeInventory("PowerDefenseBuff5",1); }
    if (GetCvar("samsara_Pariasdefense") != 6)   { TakeInventory("PowerDefenseBuff6",1); }
    if (GetCvar("samsara_Pariasdefense") != 7)   { TakeInventory("PowerDefenseBuff7",1); }
    if (GetCvar("samsara_Pariasdefense") != 8)   { TakeInventory("PowerDefenseBuff8",1); }
    if (GetCvar("samsara_Pariasdefense") != 9)   { TakeInventory("PowerDefenseBuff9",1); }
    if (GetCvar("samsara_Pariasdefense") != 10)  { TakeInventory("PowerDefenseBuff10",1); }
    if (GetCvar("samsara_Pariasdefense") != 11)  { TakeInventory("PowerDefenseBuff11",1); }
    if (GetCvar("samsara_Pariasdefense") != 12)  { TakeInventory("PowerDefenseBuff12",1); }
    if (GetCvar("samsara_Pariasdefense") != 13)  { TakeInventory("PowerDefenseBuff13",1); }
    if (GetCvar("samsara_Pariasdefense") != 14)  { TakeInventory("PowerDefenseBuff14",1); }
    if (GetCvar("samsara_Pariasdefense") != 15)  { TakeInventory("PowerDefenseBuff15",1); }
    if (GetCvar("samsara_Pariasdefense") != 16)  { TakeInventory("PowerDefenseBuff16",1); }
    if (GetCvar("samsara_Pariasdefense") != 17)  { TakeInventory("PowerDefenseBuff17",1); }
    if (GetCvar("samsara_Pariasdefense") != 18)  { TakeInventory("PowerDefenseBuff18",1); }
    if (GetCvar("samsara_Pariasdefense") != 19)  { TakeInventory("PowerDefenseBuff19",1); }
    if (GetCvar("samsara_Pariasdefense") != 20)  { TakeInventory("PowerDefenseBuff20",1); }
	
    if (GetCvar("samsara_Pariasdefense") == -10) { GiveInventory("PowerDefenseBuff-10",1); }
    if (GetCvar("samsara_Pariasdefense") == -9)  { GiveInventory("PowerDefenseBuff-9",1); }
    if (GetCvar("samsara_Pariasdefense") == -8)  { GiveInventory("PowerDefenseBuff-8",1); }
    if (GetCvar("samsara_Pariasdefense") == -7)  { GiveInventory("PowerDefenseBuff-7",1); }
    if (GetCvar("samsara_Pariasdefense") == -6)  { GiveInventory("PowerDefenseBuff-6",1); }
    if (GetCvar("samsara_Pariasdefense") == -5)  { GiveInventory("PowerDefenseBuff-5",1); }
    if (GetCvar("samsara_Pariasdefense") == -4)  { GiveInventory("PowerDefenseBuff-4",1); }
    if (GetCvar("samsara_Pariasdefense") == -3)  { GiveInventory("PowerDefenseBuff-3",1); }
    if (GetCvar("samsara_Pariasdefense") == -2)  { GiveInventory("PowerDefenseBuff-2",1); }
    if (GetCvar("samsara_Pariasdefense") == -1)  { GiveInventory("PowerDefenseBuff-1",1); }
    if (GetCvar("samsara_Pariasdefense") == 1)   { GiveInventory("PowerDefenseBuff1",1); }
    if (GetCvar("samsara_Pariasdefense") == 2)   { GiveInventory("PowerDefenseBuff2",1); }
    if (GetCvar("samsara_Pariasdefense") == 3)   { GiveInventory("PowerDefenseBuff3",1); }
    if (GetCvar("samsara_Pariasdefense") == 4)   { GiveInventory("PowerDefenseBuff4",1); }
    if (GetCvar("samsara_Pariasdefense") == 5)   { GiveInventory("PowerDefenseBuff5",1); }
    if (GetCvar("samsara_Pariasdefense") == 6)   { GiveInventory("PowerDefenseBuff6",1); }
    if (GetCvar("samsara_Pariasdefense") == 7)   { GiveInventory("PowerDefenseBuff7",1); }
    if (GetCvar("samsara_Pariasdefense") == 8)   { GiveInventory("PowerDefenseBuff8",1); }
    if (GetCvar("samsara_Pariasdefense") == 9)   { GiveInventory("PowerDefenseBuff9",1); }
    if (GetCvar("samsara_Pariasdefense") == 10)  { GiveInventory("PowerDefenseBuff10",1); }
    if (GetCvar("samsara_Pariasdefense") == 11)  { GiveInventory("PowerDefenseBuff11",1); }
    if (GetCvar("samsara_Pariasdefense") == 12)  { GiveInventory("PowerDefenseBuff12",1); }
    if (GetCvar("samsara_Pariasdefense") == 13)  { GiveInventory("PowerDefenseBuff13",1); }
    if (GetCvar("samsara_Pariasdefense") == 14)  { GiveInventory("PowerDefenseBuff14",1); }
    if (GetCvar("samsara_Pariasdefense") == 15)  { GiveInventory("PowerDefenseBuff15",1); }
    if (GetCvar("samsara_Pariasdefense") == 16)  { GiveInventory("PowerDefenseBuff16",1); }
    if (GetCvar("samsara_Pariasdefense") == 17)  { GiveInventory("PowerDefenseBuff17",1); }
    if (GetCvar("samsara_Pariasdefense") == 18)  { GiveInventory("PowerDefenseBuff18",1); }
    if (GetCvar("samsara_Pariasdefense") == 19)  { GiveInventory("PowerDefenseBuff19",1); }
    if (GetCvar("samsara_Pariasdefense") == 20)  { GiveInventory("PowerDefenseBuff20",1); }
}
return 1;
}

function int HandleDukeDamage(int respawning)
{
if (CheckInventory("DukeClass") == 1)
{
    if (GetCvar("samsara_Dukedamage") != -10) { TakeInventory("PowerDamageBuff-10",1); }
    if (GetCvar("samsara_Dukedamage") != -9)  { TakeInventory("PowerDamageBuff-9",1); }
    if (GetCvar("samsara_Dukedamage") != -8)  { TakeInventory("PowerDamageBuff-8",1); }
    if (GetCvar("samsara_Dukedamage") != -7)  { TakeInventory("PowerDamageBuff-7",1); }
    if (GetCvar("samsara_Dukedamage") != -6)  { TakeInventory("PowerDamageBuff-6",1); }
    if (GetCvar("samsara_Dukedamage") != -5)  { TakeInventory("PowerDamageBuff-5",1); }
    if (GetCvar("samsara_Dukedamage") != -4)  { TakeInventory("PowerDamageBuff-4",1); }
    if (GetCvar("samsara_Dukedamage") != -3)  { TakeInventory("PowerDamageBuff-3",1); }
    if (GetCvar("samsara_Dukedamage") != -2)  { TakeInventory("PowerDamageBuff-2",1); }
    if (GetCvar("samsara_Dukedamage") != -1)  { TakeInventory("PowerDamageBuff-1",1); }
    if (GetCvar("samsara_Dukedamage") != 1)   { TakeInventory("PowerDamageBuff1",1); }
    if (GetCvar("samsara_Dukedamage") != 2)   { TakeInventory("PowerDamageBuff2",1); }
    if (GetCvar("samsara_Dukedamage") != 3)   { TakeInventory("PowerDamageBuff3",1); }
    if (GetCvar("samsara_Dukedamage") != 4)   { TakeInventory("PowerDamageBuff4",1); }
    if (GetCvar("samsara_Dukedamage") != 5)   { TakeInventory("PowerDamageBuff5",1); }
    if (GetCvar("samsara_Dukedamage") != 6)   { TakeInventory("PowerDamageBuff6",1); }
    if (GetCvar("samsara_Dukedamage") != 7)   { TakeInventory("PowerDamageBuff7",1); }
    if (GetCvar("samsara_Dukedamage") != 8)   { TakeInventory("PowerDamageBuff8",1); }
    if (GetCvar("samsara_Dukedamage") != 9)   { TakeInventory("PowerDamageBuff9",1); }
    if (GetCvar("samsara_Dukedamage") != 10)  { TakeInventory("PowerDamageBuff10",1); }
    if (GetCvar("samsara_Dukedamage") != 11)  { TakeInventory("PowerDamageBuff11",1); }
    if (GetCvar("samsara_Dukedamage") != 12)  { TakeInventory("PowerDamageBuff12",1); }
    if (GetCvar("samsara_Dukedamage") != 13)  { TakeInventory("PowerDamageBuff13",1); }
    if (GetCvar("samsara_Dukedamage") != 14)  { TakeInventory("PowerDamageBuff14",1); }
    if (GetCvar("samsara_Dukedamage") != 15)  { TakeInventory("PowerDamageBuff15",1); }
    if (GetCvar("samsara_Dukedamage") != 16)  { TakeInventory("PowerDamageBuff16",1); }
    if (GetCvar("samsara_Dukedamage") != 17)  { TakeInventory("PowerDamageBuff17",1); }
    if (GetCvar("samsara_Dukedamage") != 18)  { TakeInventory("PowerDamageBuff18",1); }
    if (GetCvar("samsara_Dukedamage") != 19)  { TakeInventory("PowerDamageBuff19",1); }
    if (GetCvar("samsara_Dukedamage") != 20)  { TakeInventory("PowerDamageBuff20",1); }
    if (GetCvar("samsara_Dukedamage") != 21)  { TakeInventory("PowerDamageBuff21",1); }
    if (GetCvar("samsara_Dukedamage") != 22)  { TakeInventory("PowerDamageBuff22",1); }
    if (GetCvar("samsara_Dukedamage") != 23)  { TakeInventory("PowerDamageBuff23",1); }
    if (GetCvar("samsara_Dukedamage") != 24)  { TakeInventory("PowerDamageBuff24",1); }
    if (GetCvar("samsara_Dukedamage") != 25)  { TakeInventory("PowerDamageBuff25",1); }
    if (GetCvar("samsara_Dukedamage") != 26)  { TakeInventory("PowerDamageBuff26",1); }
    if (GetCvar("samsara_Dukedamage") != 27)  { TakeInventory("PowerDamageBuff27",1); }
    if (GetCvar("samsara_Dukedamage") != 28)  { TakeInventory("PowerDamageBuff28",1); }
    if (GetCvar("samsara_Dukedamage") != 29)  { TakeInventory("PowerDamageBuff29",1); }
    if (GetCvar("samsara_Dukedamage") != 30)  { TakeInventory("PowerDamageBuff30",1); }
	
    if (GetCvar("samsara_Dukedamage") == -10) { GiveInventory("PowerDamageBuff-10",1); }
    if (GetCvar("samsara_Dukedamage") == -9)  { GiveInventory("PowerDamageBuff-9",1); }
    if (GetCvar("samsara_Dukedamage") == -8)  { GiveInventory("PowerDamageBuff-8",1); }
    if (GetCvar("samsara_Dukedamage") == -7)  { GiveInventory("PowerDamageBuff-7",1); }
    if (GetCvar("samsara_Dukedamage") == -6)  { GiveInventory("PowerDamageBuff-6",1); }
    if (GetCvar("samsara_Dukedamage") == -5)  { GiveInventory("PowerDamageBuff-5",1); }
    if (GetCvar("samsara_Dukedamage") == -4)  { GiveInventory("PowerDamageBuff-4",1); }
    if (GetCvar("samsara_Dukedamage") == -3)  { GiveInventory("PowerDamageBuff-3",1); }
    if (GetCvar("samsara_Dukedamage") == -2)  { GiveInventory("PowerDamageBuff-2",1); }
    if (GetCvar("samsara_Dukedamage") == -1)  { GiveInventory("PowerDamageBuff-1",1); }
    if (GetCvar("samsara_Dukedamage") == 1)   { GiveInventory("PowerDamageBuff1",1); }
    if (GetCvar("samsara_Dukedamage") == 2)   { GiveInventory("PowerDamageBuff2",1); }
    if (GetCvar("samsara_Dukedamage") == 3)   { GiveInventory("PowerDamageBuff3",1); }
    if (GetCvar("samsara_Dukedamage") == 4)   { GiveInventory("PowerDamageBuff4",1); }
    if (GetCvar("samsara_Dukedamage") == 5)   { GiveInventory("PowerDamageBuff5",1); }
    if (GetCvar("samsara_Dukedamage") == 6)   { GiveInventory("PowerDamageBuff6",1); }
    if (GetCvar("samsara_Dukedamage") == 7)   { GiveInventory("PowerDamageBuff7",1); }
    if (GetCvar("samsara_Dukedamage") == 8)   { GiveInventory("PowerDamageBuff8",1); }
    if (GetCvar("samsara_Dukedamage") == 9)   { GiveInventory("PowerDamageBuff9",1); }
    if (GetCvar("samsara_Dukedamage") == 10)  { GiveInventory("PowerDamageBuff10",1); }
    if (GetCvar("samsara_Dukedamage") == 11)  { GiveInventory("PowerDamageBuff11",1); }
    if (GetCvar("samsara_Dukedamage") == 12)  { GiveInventory("PowerDamageBuff12",1); }
    if (GetCvar("samsara_Dukedamage") == 13)  { GiveInventory("PowerDamageBuff13",1); }
    if (GetCvar("samsara_Dukedamage") == 14)  { GiveInventory("PowerDamageBuff14",1); }
    if (GetCvar("samsara_Dukedamage") == 15)  { GiveInventory("PowerDamageBuff15",1); }
    if (GetCvar("samsara_Dukedamage") == 16)  { GiveInventory("PowerDamageBuff16",1); }
    if (GetCvar("samsara_Dukedamage") == 17)  { GiveInventory("PowerDamageBuff17",1); }
    if (GetCvar("samsara_Dukedamage") == 18)  { GiveInventory("PowerDamageBuff18",1); }
    if (GetCvar("samsara_Dukedamage") == 19)  { GiveInventory("PowerDamageBuff19",1); }
    if (GetCvar("samsara_Dukedamage") == 20)  { GiveInventory("PowerDamageBuff20",1); }
    if (GetCvar("samsara_Dukedamage") == 21)  { GiveInventory("PowerDamageBuff21",1); }
    if (GetCvar("samsara_Dukedamage") == 22)  { GiveInventory("PowerDamageBuff22",1); }
    if (GetCvar("samsara_Dukedamage") == 23)  { GiveInventory("PowerDamageBuff23",1); }
    if (GetCvar("samsara_Dukedamage") == 24)  { GiveInventory("PowerDamageBuff24",1); }
    if (GetCvar("samsara_Dukedamage") == 25)  { GiveInventory("PowerDamageBuff25",1); }
    if (GetCvar("samsara_Dukedamage") == 26)  { GiveInventory("PowerDamageBuff26",1); }
    if (GetCvar("samsara_Dukedamage") == 27)  { GiveInventory("PowerDamageBuff27",1); }
    if (GetCvar("samsara_Dukedamage") == 28)  { GiveInventory("PowerDamageBuff28",1); }
    if (GetCvar("samsara_Dukedamage") == 29)  { GiveInventory("PowerDamageBuff29",1); }
    if (GetCvar("samsara_Dukedamage") == 30)  { GiveInventory("PowerDamageBuff30",1); }
}
return 1;
}

function int HandleDukeDefense(int respawning)
{
if (CheckInventory("DukeClass") == 1)
{
    if (GetCvar("samsara_Dukedefense") != -10) { TakeInventory("PowerDefenseBuff-10",1); }
    if (GetCvar("samsara_Dukedefense") != -9)  { TakeInventory("PowerDefenseBuff-9",1); }
    if (GetCvar("samsara_Dukedefense") != -8)  { TakeInventory("PowerDefenseBuff-8",1); }
    if (GetCvar("samsara_Dukedefense") != -7)  { TakeInventory("PowerDefenseBuff-7",1); }
    if (GetCvar("samsara_Dukedefense") != -6)  { TakeInventory("PowerDefenseBuff-6",1); }
    if (GetCvar("samsara_Dukedefense") != -5)  { TakeInventory("PowerDefenseBuff-5",1); }
    if (GetCvar("samsara_Dukedefense") != -4)  { TakeInventory("PowerDefenseBuff-4",1); }
    if (GetCvar("samsara_Dukedefense") != -3)  { TakeInventory("PowerDefenseBuff-3",1); }
    if (GetCvar("samsara_Dukedefense") != -2)  { TakeInventory("PowerDefenseBuff-2",1); }
    if (GetCvar("samsara_Dukedefense") != -1)  { TakeInventory("PowerDefenseBuff-1",1); }
    if (GetCvar("samsara_Dukedefense") != 1)   { TakeInventory("PowerDefenseBuff1",1); }
    if (GetCvar("samsara_Dukedefense") != 2)   { TakeInventory("PowerDefenseBuff2",1); }
    if (GetCvar("samsara_Dukedefense") != 3)   { TakeInventory("PowerDefenseBuff3",1); }
    if (GetCvar("samsara_Dukedefense") != 4)   { TakeInventory("PowerDefenseBuff4",1); }
    if (GetCvar("samsara_Dukedefense") != 5)   { TakeInventory("PowerDefenseBuff5",1); }
    if (GetCvar("samsara_Dukedefense") != 6)   { TakeInventory("PowerDefenseBuff6",1); }
    if (GetCvar("samsara_Dukedefense") != 7)   { TakeInventory("PowerDefenseBuff7",1); }
    if (GetCvar("samsara_Dukedefense") != 8)   { TakeInventory("PowerDefenseBuff8",1); }
    if (GetCvar("samsara_Dukedefense") != 9)   { TakeInventory("PowerDefenseBuff9",1); }
    if (GetCvar("samsara_Dukedefense") != 10)  { TakeInventory("PowerDefenseBuff10",1); }
    if (GetCvar("samsara_Dukedefense") != 11)  { TakeInventory("PowerDefenseBuff11",1); }
    if (GetCvar("samsara_Dukedefense") != 12)  { TakeInventory("PowerDefenseBuff12",1); }
    if (GetCvar("samsara_Dukedefense") != 13)  { TakeInventory("PowerDefenseBuff13",1); }
    if (GetCvar("samsara_Dukedefense") != 14)  { TakeInventory("PowerDefenseBuff14",1); }
    if (GetCvar("samsara_Dukedefense") != 15)  { TakeInventory("PowerDefenseBuff15",1); }
    if (GetCvar("samsara_Dukedefense") != 16)  { TakeInventory("PowerDefenseBuff16",1); }
    if (GetCvar("samsara_Dukedefense") != 17)  { TakeInventory("PowerDefenseBuff17",1); }
    if (GetCvar("samsara_Dukedefense") != 18)  { TakeInventory("PowerDefenseBuff18",1); }
    if (GetCvar("samsara_Dukedefense") != 19)  { TakeInventory("PowerDefenseBuff19",1); }
    if (GetCvar("samsara_Dukedefense") != 20)  { TakeInventory("PowerDefenseBuff20",1); }
	
    if (GetCvar("samsara_Dukedefense") == -10) { GiveInventory("PowerDefenseBuff-10",1); }
    if (GetCvar("samsara_Dukedefense") == -9)  { GiveInventory("PowerDefenseBuff-9",1); }
    if (GetCvar("samsara_Dukedefense") == -8)  { GiveInventory("PowerDefenseBuff-8",1); }
    if (GetCvar("samsara_Dukedefense") == -7)  { GiveInventory("PowerDefenseBuff-7",1); }
    if (GetCvar("samsara_Dukedefense") == -6)  { GiveInventory("PowerDefenseBuff-6",1); }
    if (GetCvar("samsara_Dukedefense") == -5)  { GiveInventory("PowerDefenseBuff-5",1); }
    if (GetCvar("samsara_Dukedefense") == -4)  { GiveInventory("PowerDefenseBuff-4",1); }
    if (GetCvar("samsara_Dukedefense") == -3)  { GiveInventory("PowerDefenseBuff-3",1); }
    if (GetCvar("samsara_Dukedefense") == -2)  { GiveInventory("PowerDefenseBuff-2",1); }
    if (GetCvar("samsara_Dukedefense") == -1)  { GiveInventory("PowerDefenseBuff-1",1); }
    if (GetCvar("samsara_Dukedefense") == 1)   { GiveInventory("PowerDefenseBuff1",1); }
    if (GetCvar("samsara_Dukedefense") == 2)   { GiveInventory("PowerDefenseBuff2",1); }
    if (GetCvar("samsara_Dukedefense") == 3)   { GiveInventory("PowerDefenseBuff3",1); }
    if (GetCvar("samsara_Dukedefense") == 4)   { GiveInventory("PowerDefenseBuff4",1); }
    if (GetCvar("samsara_Dukedefense") == 5)   { GiveInventory("PowerDefenseBuff5",1); }
    if (GetCvar("samsara_Dukedefense") == 6)   { GiveInventory("PowerDefenseBuff6",1); }
    if (GetCvar("samsara_Dukedefense") == 7)   { GiveInventory("PowerDefenseBuff7",1); }
    if (GetCvar("samsara_Dukedefense") == 8)   { GiveInventory("PowerDefenseBuff8",1); }
    if (GetCvar("samsara_Dukedefense") == 9)   { GiveInventory("PowerDefenseBuff9",1); }
    if (GetCvar("samsara_Dukedefense") == 10)  { GiveInventory("PowerDefenseBuff10",1); }
    if (GetCvar("samsara_Dukedefense") == 11)  { GiveInventory("PowerDefenseBuff11",1); }
    if (GetCvar("samsara_Dukedefense") == 12)  { GiveInventory("PowerDefenseBuff12",1); }
    if (GetCvar("samsara_Dukedefense") == 13)  { GiveInventory("PowerDefenseBuff13",1); }
    if (GetCvar("samsara_Dukedefense") == 14)  { GiveInventory("PowerDefenseBuff14",1); }
    if (GetCvar("samsara_Dukedefense") == 15)  { GiveInventory("PowerDefenseBuff15",1); }
    if (GetCvar("samsara_Dukedefense") == 16)  { GiveInventory("PowerDefenseBuff16",1); }
    if (GetCvar("samsara_Dukedefense") == 17)  { GiveInventory("PowerDefenseBuff17",1); }
    if (GetCvar("samsara_Dukedefense") == 18)  { GiveInventory("PowerDefenseBuff18",1); }
    if (GetCvar("samsara_Dukedefense") == 19)  { GiveInventory("PowerDefenseBuff19",1); }
    if (GetCvar("samsara_Dukedefense") == 20)  { GiveInventory("PowerDefenseBuff20",1); }
}
return 1;
}

function int HandleSODamage(int respawning)
{
if (CheckInventory("MarathonClass") == 1)
{
    if (GetCvar("samsara_SOdamage") != -10) { TakeInventory("PowerDamageBuff-10",1); }
    if (GetCvar("samsara_SOdamage") != -9)  { TakeInventory("PowerDamageBuff-9",1); }
    if (GetCvar("samsara_SOdamage") != -8)  { TakeInventory("PowerDamageBuff-8",1); }
    if (GetCvar("samsara_SOdamage") != -7)  { TakeInventory("PowerDamageBuff-7",1); }
    if (GetCvar("samsara_SOdamage") != -6)  { TakeInventory("PowerDamageBuff-6",1); }
    if (GetCvar("samsara_SOdamage") != -5)  { TakeInventory("PowerDamageBuff-5",1); }
    if (GetCvar("samsara_SOdamage") != -4)  { TakeInventory("PowerDamageBuff-4",1); }
    if (GetCvar("samsara_SOdamage") != -3)  { TakeInventory("PowerDamageBuff-3",1); }
    if (GetCvar("samsara_SOdamage") != -2)  { TakeInventory("PowerDamageBuff-2",1); }
    if (GetCvar("samsara_SOdamage") != -1)  { TakeInventory("PowerDamageBuff-1",1); }
    if (GetCvar("samsara_SOdamage") != 1)   { TakeInventory("PowerDamageBuff1",1); }
    if (GetCvar("samsara_SOdamage") != 2)   { TakeInventory("PowerDamageBuff2",1); }
    if (GetCvar("samsara_SOdamage") != 3)   { TakeInventory("PowerDamageBuff3",1); }
    if (GetCvar("samsara_SOdamage") != 4)   { TakeInventory("PowerDamageBuff4",1); }
    if (GetCvar("samsara_SOdamage") != 5)   { TakeInventory("PowerDamageBuff5",1); }
    if (GetCvar("samsara_SOdamage") != 6)   { TakeInventory("PowerDamageBuff6",1); }
    if (GetCvar("samsara_SOdamage") != 7)   { TakeInventory("PowerDamageBuff7",1); }
    if (GetCvar("samsara_SOdamage") != 8)   { TakeInventory("PowerDamageBuff8",1); }
    if (GetCvar("samsara_SOdamage") != 9)   { TakeInventory("PowerDamageBuff9",1); }
    if (GetCvar("samsara_SOdamage") != 10)  { TakeInventory("PowerDamageBuff10",1); }
    if (GetCvar("samsara_SOdamage") != 11)  { TakeInventory("PowerDamageBuff11",1); }
    if (GetCvar("samsara_SOdamage") != 12)  { TakeInventory("PowerDamageBuff12",1); }
    if (GetCvar("samsara_SOdamage") != 13)  { TakeInventory("PowerDamageBuff13",1); }
    if (GetCvar("samsara_SOdamage") != 14)  { TakeInventory("PowerDamageBuff14",1); }
    if (GetCvar("samsara_SOdamage") != 15)  { TakeInventory("PowerDamageBuff15",1); }
    if (GetCvar("samsara_SOdamage") != 16)  { TakeInventory("PowerDamageBuff16",1); }
    if (GetCvar("samsara_SOdamage") != 17)  { TakeInventory("PowerDamageBuff17",1); }
    if (GetCvar("samsara_SOdamage") != 18)  { TakeInventory("PowerDamageBuff18",1); }
    if (GetCvar("samsara_SOdamage") != 19)  { TakeInventory("PowerDamageBuff19",1); }
    if (GetCvar("samsara_SOdamage") != 20)  { TakeInventory("PowerDamageBuff20",1); }
    if (GetCvar("samsara_SOdamage") != 21)  { TakeInventory("PowerDamageBuff21",1); }
    if (GetCvar("samsara_SOdamage") != 22)  { TakeInventory("PowerDamageBuff22",1); }
    if (GetCvar("samsara_SOdamage") != 23)  { TakeInventory("PowerDamageBuff23",1); }
    if (GetCvar("samsara_SOdamage") != 24)  { TakeInventory("PowerDamageBuff24",1); }
    if (GetCvar("samsara_SOdamage") != 25)  { TakeInventory("PowerDamageBuff25",1); }
    if (GetCvar("samsara_SOdamage") != 26)  { TakeInventory("PowerDamageBuff26",1); }
    if (GetCvar("samsara_SOdamage") != 27)  { TakeInventory("PowerDamageBuff27",1); }
    if (GetCvar("samsara_SOdamage") != 28)  { TakeInventory("PowerDamageBuff28",1); }
    if (GetCvar("samsara_SOdamage") != 29)  { TakeInventory("PowerDamageBuff29",1); }
    if (GetCvar("samsara_SOdamage") != 30)  { TakeInventory("PowerDamageBuff30",1); }
	
    if (GetCvar("samsara_SOdamage") == -10) { GiveInventory("PowerDamageBuff-10",1); }
    if (GetCvar("samsara_SOdamage") == -9)  { GiveInventory("PowerDamageBuff-9",1); }
    if (GetCvar("samsara_SOdamage") == -8)  { GiveInventory("PowerDamageBuff-8",1); }
    if (GetCvar("samsara_SOdamage") == -7)  { GiveInventory("PowerDamageBuff-7",1); }
    if (GetCvar("samsara_SOdamage") == -6)  { GiveInventory("PowerDamageBuff-6",1); }
    if (GetCvar("samsara_SOdamage") == -5)  { GiveInventory("PowerDamageBuff-5",1); }
    if (GetCvar("samsara_SOdamage") == -4)  { GiveInventory("PowerDamageBuff-4",1); }
    if (GetCvar("samsara_SOdamage") == -3)  { GiveInventory("PowerDamageBuff-3",1); }
    if (GetCvar("samsara_SOdamage") == -2)  { GiveInventory("PowerDamageBuff-2",1); }
    if (GetCvar("samsara_SOdamage") == -1)  { GiveInventory("PowerDamageBuff-1",1); }
    if (GetCvar("samsara_SOdamage") == 1)   { GiveInventory("PowerDamageBuff1",1); }
    if (GetCvar("samsara_SOdamage") == 2)   { GiveInventory("PowerDamageBuff2",1); }
    if (GetCvar("samsara_SOdamage") == 3)   { GiveInventory("PowerDamageBuff3",1); }
    if (GetCvar("samsara_SOdamage") == 4)   { GiveInventory("PowerDamageBuff4",1); }
    if (GetCvar("samsara_SOdamage") == 5)   { GiveInventory("PowerDamageBuff5",1); }
    if (GetCvar("samsara_SOdamage") == 6)   { GiveInventory("PowerDamageBuff6",1); }
    if (GetCvar("samsara_SOdamage") == 7)   { GiveInventory("PowerDamageBuff7",1); }
    if (GetCvar("samsara_SOdamage") == 8)   { GiveInventory("PowerDamageBuff8",1); }
    if (GetCvar("samsara_SOdamage") == 9)   { GiveInventory("PowerDamageBuff9",1); }
    if (GetCvar("samsara_SOdamage") == 10)  { GiveInventory("PowerDamageBuff10",1); }
    if (GetCvar("samsara_SOdamage") == 11)  { GiveInventory("PowerDamageBuff11",1); }
    if (GetCvar("samsara_SOdamage") == 12)  { GiveInventory("PowerDamageBuff12",1); }
    if (GetCvar("samsara_SOdamage") == 13)  { GiveInventory("PowerDamageBuff13",1); }
    if (GetCvar("samsara_SOdamage") == 14)  { GiveInventory("PowerDamageBuff14",1); }
    if (GetCvar("samsara_SOdamage") == 15)  { GiveInventory("PowerDamageBuff15",1); }
    if (GetCvar("samsara_SOdamage") == 16)  { GiveInventory("PowerDamageBuff16",1); }
    if (GetCvar("samsara_SOdamage") == 17)  { GiveInventory("PowerDamageBuff17",1); }
    if (GetCvar("samsara_SOdamage") == 18)  { GiveInventory("PowerDamageBuff18",1); }
    if (GetCvar("samsara_SOdamage") == 19)  { GiveInventory("PowerDamageBuff19",1); }
    if (GetCvar("samsara_SOdamage") == 20)  { GiveInventory("PowerDamageBuff20",1); }
    if (GetCvar("samsara_SOdamage") == 21)  { GiveInventory("PowerDamageBuff21",1); }
    if (GetCvar("samsara_SOdamage") == 22)  { GiveInventory("PowerDamageBuff22",1); }
    if (GetCvar("samsara_SOdamage") == 23)  { GiveInventory("PowerDamageBuff23",1); }
    if (GetCvar("samsara_SOdamage") == 24)  { GiveInventory("PowerDamageBuff24",1); }
    if (GetCvar("samsara_SOdamage") == 25)  { GiveInventory("PowerDamageBuff25",1); }
    if (GetCvar("samsara_SOdamage") == 26)  { GiveInventory("PowerDamageBuff26",1); }
    if (GetCvar("samsara_SOdamage") == 27)  { GiveInventory("PowerDamageBuff27",1); }
    if (GetCvar("samsara_SOdamage") == 28)  { GiveInventory("PowerDamageBuff28",1); }
    if (GetCvar("samsara_SOdamage") == 29)  { GiveInventory("PowerDamageBuff29",1); }
    if (GetCvar("samsara_SOdamage") == 30)  { GiveInventory("PowerDamageBuff30",1); }
}
return 1;
}

function int HandleSODefense(int respawning)
{
if (CheckInventory("MarathonClass") == 1)
{
    if (GetCvar("samsara_SOdefense") != -10) { TakeInventory("PowerDefenseBuff-10",1); }
    if (GetCvar("samsara_SOdefense") != -9)  { TakeInventory("PowerDefenseBuff-9",1); }
    if (GetCvar("samsara_SOdefense") != -8)  { TakeInventory("PowerDefenseBuff-8",1); }
    if (GetCvar("samsara_SOdefense") != -7)  { TakeInventory("PowerDefenseBuff-7",1); }
    if (GetCvar("samsara_SOdefense") != -6)  { TakeInventory("PowerDefenseBuff-6",1); }
    if (GetCvar("samsara_SOdefense") != -5)  { TakeInventory("PowerDefenseBuff-5",1); }
    if (GetCvar("samsara_SOdefense") != -4)  { TakeInventory("PowerDefenseBuff-4",1); }
    if (GetCvar("samsara_SOdefense") != -3)  { TakeInventory("PowerDefenseBuff-3",1); }
    if (GetCvar("samsara_SOdefense") != -2)  { TakeInventory("PowerDefenseBuff-2",1); }
    if (GetCvar("samsara_SOdefense") != -1)  { TakeInventory("PowerDefenseBuff-1",1); }
    if (GetCvar("samsara_SOdefense") != 1)   { TakeInventory("PowerDefenseBuff1",1); }
    if (GetCvar("samsara_SOdefense") != 2)   { TakeInventory("PowerDefenseBuff2",1); }
    if (GetCvar("samsara_SOdefense") != 3)   { TakeInventory("PowerDefenseBuff3",1); }
    if (GetCvar("samsara_SOdefense") != 4)   { TakeInventory("PowerDefenseBuff4",1); }
    if (GetCvar("samsara_SOdefense") != 5)   { TakeInventory("PowerDefenseBuff5",1); }
    if (GetCvar("samsara_SOdefense") != 6)   { TakeInventory("PowerDefenseBuff6",1); }
    if (GetCvar("samsara_SOdefense") != 7)   { TakeInventory("PowerDefenseBuff7",1); }
    if (GetCvar("samsara_SOdefense") != 8)   { TakeInventory("PowerDefenseBuff8",1); }
    if (GetCvar("samsara_SOdefense") != 9)   { TakeInventory("PowerDefenseBuff9",1); }
    if (GetCvar("samsara_SOdefense") != 10)  { TakeInventory("PowerDefenseBuff10",1); }
    if (GetCvar("samsara_SOdefense") != 11)  { TakeInventory("PowerDefenseBuff11",1); }
    if (GetCvar("samsara_SOdefense") != 12)  { TakeInventory("PowerDefenseBuff12",1); }
    if (GetCvar("samsara_SOdefense") != 13)  { TakeInventory("PowerDefenseBuff13",1); }
    if (GetCvar("samsara_SOdefense") != 14)  { TakeInventory("PowerDefenseBuff14",1); }
    if (GetCvar("samsara_SOdefense") != 15)  { TakeInventory("PowerDefenseBuff15",1); }
    if (GetCvar("samsara_SOdefense") != 16)  { TakeInventory("PowerDefenseBuff16",1); }
    if (GetCvar("samsara_SOdefense") != 17)  { TakeInventory("PowerDefenseBuff17",1); }
    if (GetCvar("samsara_SOdefense") != 18)  { TakeInventory("PowerDefenseBuff18",1); }
    if (GetCvar("samsara_SOdefense") != 19)  { TakeInventory("PowerDefenseBuff19",1); }
    if (GetCvar("samsara_SOdefense") != 20)  { TakeInventory("PowerDefenseBuff20",1); }
	
    if (GetCvar("samsara_SOdefense") == -10) { GiveInventory("PowerDefenseBuff-10",1); }
    if (GetCvar("samsara_SOdefense") == -9)  { GiveInventory("PowerDefenseBuff-9",1); }
    if (GetCvar("samsara_SOdefense") == -8)  { GiveInventory("PowerDefenseBuff-8",1); }
    if (GetCvar("samsara_SOdefense") == -7)  { GiveInventory("PowerDefenseBuff-7",1); }
    if (GetCvar("samsara_SOdefense") == -6)  { GiveInventory("PowerDefenseBuff-6",1); }
    if (GetCvar("samsara_SOdefense") == -5)  { GiveInventory("PowerDefenseBuff-5",1); }
    if (GetCvar("samsara_SOdefense") == -4)  { GiveInventory("PowerDefenseBuff-4",1); }
    if (GetCvar("samsara_SOdefense") == -3)  { GiveInventory("PowerDefenseBuff-3",1); }
    if (GetCvar("samsara_SOdefense") == -2)  { GiveInventory("PowerDefenseBuff-2",1); }
    if (GetCvar("samsara_SOdefense") == -1)  { GiveInventory("PowerDefenseBuff-1",1); }
    if (GetCvar("samsara_SOdefense") == 1)   { GiveInventory("PowerDefenseBuff1",1); }
    if (GetCvar("samsara_SOdefense") == 2)   { GiveInventory("PowerDefenseBuff2",1); }
    if (GetCvar("samsara_SOdefense") == 3)   { GiveInventory("PowerDefenseBuff3",1); }
    if (GetCvar("samsara_SOdefense") == 4)   { GiveInventory("PowerDefenseBuff4",1); }
    if (GetCvar("samsara_SOdefense") == 5)   { GiveInventory("PowerDefenseBuff5",1); }
    if (GetCvar("samsara_SOdefense") == 6)   { GiveInventory("PowerDefenseBuff6",1); }
    if (GetCvar("samsara_SOdefense") == 7)   { GiveInventory("PowerDefenseBuff7",1); }
    if (GetCvar("samsara_SOdefense") == 8)   { GiveInventory("PowerDefenseBuff8",1); }
    if (GetCvar("samsara_SOdefense") == 9)   { GiveInventory("PowerDefenseBuff9",1); }
    if (GetCvar("samsara_SOdefense") == 10)  { GiveInventory("PowerDefenseBuff10",1); }
    if (GetCvar("samsara_SOdefense") == 11)  { GiveInventory("PowerDefenseBuff11",1); }
    if (GetCvar("samsara_SOdefense") == 12)  { GiveInventory("PowerDefenseBuff12",1); }
    if (GetCvar("samsara_SOdefense") == 13)  { GiveInventory("PowerDefenseBuff13",1); }
    if (GetCvar("samsara_SOdefense") == 14)  { GiveInventory("PowerDefenseBuff14",1); }
    if (GetCvar("samsara_SOdefense") == 15)  { GiveInventory("PowerDefenseBuff15",1); }
    if (GetCvar("samsara_SOdefense") == 16)  { GiveInventory("PowerDefenseBuff16",1); }
    if (GetCvar("samsara_SOdefense") == 17)  { GiveInventory("PowerDefenseBuff17",1); }
    if (GetCvar("samsara_SOdefense") == 18)  { GiveInventory("PowerDefenseBuff18",1); }
    if (GetCvar("samsara_SOdefense") == 19)  { GiveInventory("PowerDefenseBuff19",1); }
    if (GetCvar("samsara_SOdefense") == 20)  { GiveInventory("PowerDefenseBuff20",1); }
}
return 1;
}

function int HandleRangerDamage(int respawning)
{
if (CheckInventory("QuakeClass") == 1)
{
    if (GetCvar("samsara_Rangerdamage") != -10) { TakeInventory("PowerDamageBuff-10",1); }
    if (GetCvar("samsara_Rangerdamage") != -9)  { TakeInventory("PowerDamageBuff-9",1); }
    if (GetCvar("samsara_Rangerdamage") != -8)  { TakeInventory("PowerDamageBuff-8",1); }
    if (GetCvar("samsara_Rangerdamage") != -7)  { TakeInventory("PowerDamageBuff-7",1); }
    if (GetCvar("samsara_Rangerdamage") != -6)  { TakeInventory("PowerDamageBuff-6",1); }
    if (GetCvar("samsara_Rangerdamage") != -5)  { TakeInventory("PowerDamageBuff-5",1); }
    if (GetCvar("samsara_Rangerdamage") != -4)  { TakeInventory("PowerDamageBuff-4",1); }
    if (GetCvar("samsara_Rangerdamage") != -3)  { TakeInventory("PowerDamageBuff-3",1); }
    if (GetCvar("samsara_Rangerdamage") != -2)  { TakeInventory("PowerDamageBuff-2",1); }
    if (GetCvar("samsara_Rangerdamage") != -1)  { TakeInventory("PowerDamageBuff-1",1); }
    if (GetCvar("samsara_Rangerdamage") != 1)   { TakeInventory("PowerDamageBuff1",1); }
    if (GetCvar("samsara_Rangerdamage") != 2)   { TakeInventory("PowerDamageBuff2",1); }
    if (GetCvar("samsara_Rangerdamage") != 3)   { TakeInventory("PowerDamageBuff3",1); }
    if (GetCvar("samsara_Rangerdamage") != 4)   { TakeInventory("PowerDamageBuff4",1); }
    if (GetCvar("samsara_Rangerdamage") != 5)   { TakeInventory("PowerDamageBuff5",1); }
    if (GetCvar("samsara_Rangerdamage") != 6)   { TakeInventory("PowerDamageBuff6",1); }
    if (GetCvar("samsara_Rangerdamage") != 7)   { TakeInventory("PowerDamageBuff7",1); }
    if (GetCvar("samsara_Rangerdamage") != 8)   { TakeInventory("PowerDamageBuff8",1); }
    if (GetCvar("samsara_Rangerdamage") != 9)   { TakeInventory("PowerDamageBuff9",1); }
    if (GetCvar("samsara_Rangerdamage") != 10)  { TakeInventory("PowerDamageBuff10",1); }
    if (GetCvar("samsara_Rangerdamage") != 11)  { TakeInventory("PowerDamageBuff11",1); }
    if (GetCvar("samsara_Rangerdamage") != 12)  { TakeInventory("PowerDamageBuff12",1); }
    if (GetCvar("samsara_Rangerdamage") != 13)  { TakeInventory("PowerDamageBuff13",1); }
    if (GetCvar("samsara_Rangerdamage") != 14)  { TakeInventory("PowerDamageBuff14",1); }
    if (GetCvar("samsara_Rangerdamage") != 15)  { TakeInventory("PowerDamageBuff15",1); }
    if (GetCvar("samsara_Rangerdamage") != 16)  { TakeInventory("PowerDamageBuff16",1); }
    if (GetCvar("samsara_Rangerdamage") != 17)  { TakeInventory("PowerDamageBuff17",1); }
    if (GetCvar("samsara_Rangerdamage") != 18)  { TakeInventory("PowerDamageBuff18",1); }
    if (GetCvar("samsara_Rangerdamage") != 19)  { TakeInventory("PowerDamageBuff19",1); }
    if (GetCvar("samsara_Rangerdamage") != 20)  { TakeInventory("PowerDamageBuff20",1); }
    if (GetCvar("samsara_Rangerdamage") != 21)  { TakeInventory("PowerDamageBuff21",1); }
    if (GetCvar("samsara_Rangerdamage") != 22)  { TakeInventory("PowerDamageBuff22",1); }
    if (GetCvar("samsara_Rangerdamage") != 23)  { TakeInventory("PowerDamageBuff23",1); }
    if (GetCvar("samsara_Rangerdamage") != 24)  { TakeInventory("PowerDamageBuff24",1); }
    if (GetCvar("samsara_Rangerdamage") != 25)  { TakeInventory("PowerDamageBuff25",1); }
    if (GetCvar("samsara_Rangerdamage") != 26)  { TakeInventory("PowerDamageBuff26",1); }
    if (GetCvar("samsara_Rangerdamage") != 27)  { TakeInventory("PowerDamageBuff27",1); }
    if (GetCvar("samsara_Rangerdamage") != 28)  { TakeInventory("PowerDamageBuff28",1); }
    if (GetCvar("samsara_Rangerdamage") != 29)  { TakeInventory("PowerDamageBuff29",1); }
    if (GetCvar("samsara_Rangerdamage") != 30)  { TakeInventory("PowerDamageBuff30",1); }
	
    if (GetCvar("samsara_Rangerdamage") == -10) { GiveInventory("PowerDamageBuff-10",1); }
    if (GetCvar("samsara_Rangerdamage") == -9)  { GiveInventory("PowerDamageBuff-9",1); }
    if (GetCvar("samsara_Rangerdamage") == -8)  { GiveInventory("PowerDamageBuff-8",1); }
    if (GetCvar("samsara_Rangerdamage") == -7)  { GiveInventory("PowerDamageBuff-7",1); }
    if (GetCvar("samsara_Rangerdamage") == -6)  { GiveInventory("PowerDamageBuff-6",1); }
    if (GetCvar("samsara_Rangerdamage") == -5)  { GiveInventory("PowerDamageBuff-5",1); }
    if (GetCvar("samsara_Rangerdamage") == -4)  { GiveInventory("PowerDamageBuff-4",1); }
    if (GetCvar("samsara_Rangerdamage") == -3)  { GiveInventory("PowerDamageBuff-3",1); }
    if (GetCvar("samsara_Rangerdamage") == -2)  { GiveInventory("PowerDamageBuff-2",1); }
    if (GetCvar("samsara_Rangerdamage") == -1)  { GiveInventory("PowerDamageBuff-1",1); }
    if (GetCvar("samsara_Rangerdamage") == 1)   { GiveInventory("PowerDamageBuff1",1); }
    if (GetCvar("samsara_Rangerdamage") == 2)   { GiveInventory("PowerDamageBuff2",1); }
    if (GetCvar("samsara_Rangerdamage") == 3)   { GiveInventory("PowerDamageBuff3",1); }
    if (GetCvar("samsara_Rangerdamage") == 4)   { GiveInventory("PowerDamageBuff4",1); }
    if (GetCvar("samsara_Rangerdamage") == 5)   { GiveInventory("PowerDamageBuff5",1); }
    if (GetCvar("samsara_Rangerdamage") == 6)   { GiveInventory("PowerDamageBuff6",1); }
    if (GetCvar("samsara_Rangerdamage") == 7)   { GiveInventory("PowerDamageBuff7",1); }
    if (GetCvar("samsara_Rangerdamage") == 8)   { GiveInventory("PowerDamageBuff8",1); }
    if (GetCvar("samsara_Rangerdamage") == 9)   { GiveInventory("PowerDamageBuff9",1); }
    if (GetCvar("samsara_Rangerdamage") == 10)  { GiveInventory("PowerDamageBuff10",1); }
    if (GetCvar("samsara_Rangerdamage") == 11)  { GiveInventory("PowerDamageBuff11",1); }
    if (GetCvar("samsara_Rangerdamage") == 12)  { GiveInventory("PowerDamageBuff12",1); }
    if (GetCvar("samsara_Rangerdamage") == 13)  { GiveInventory("PowerDamageBuff13",1); }
    if (GetCvar("samsara_Rangerdamage") == 14)  { GiveInventory("PowerDamageBuff14",1); }
    if (GetCvar("samsara_Rangerdamage") == 15)  { GiveInventory("PowerDamageBuff15",1); }
    if (GetCvar("samsara_Rangerdamage") == 16)  { GiveInventory("PowerDamageBuff16",1); }
    if (GetCvar("samsara_Rangerdamage") == 17)  { GiveInventory("PowerDamageBuff17",1); }
    if (GetCvar("samsara_Rangerdamage") == 18)  { GiveInventory("PowerDamageBuff18",1); }
    if (GetCvar("samsara_Rangerdamage") == 19)  { GiveInventory("PowerDamageBuff19",1); }
    if (GetCvar("samsara_Rangerdamage") == 20)  { GiveInventory("PowerDamageBuff20",1); }
    if (GetCvar("samsara_Rangerdamage") == 21)  { GiveInventory("PowerDamageBuff21",1); }
    if (GetCvar("samsara_Rangerdamage") == 22)  { GiveInventory("PowerDamageBuff22",1); }
    if (GetCvar("samsara_Rangerdamage") == 23)  { GiveInventory("PowerDamageBuff23",1); }
    if (GetCvar("samsara_Rangerdamage") == 24)  { GiveInventory("PowerDamageBuff24",1); }
    if (GetCvar("samsara_Rangerdamage") == 25)  { GiveInventory("PowerDamageBuff25",1); }
    if (GetCvar("samsara_Rangerdamage") == 26)  { GiveInventory("PowerDamageBuff26",1); }
    if (GetCvar("samsara_Rangerdamage") == 27)  { GiveInventory("PowerDamageBuff27",1); }
    if (GetCvar("samsara_Rangerdamage") == 28)  { GiveInventory("PowerDamageBuff28",1); }
    if (GetCvar("samsara_Rangerdamage") == 29)  { GiveInventory("PowerDamageBuff29",1); }
    if (GetCvar("samsara_Rangerdamage") == 30)  { GiveInventory("PowerDamageBuff30",1); }
}
return 1;
}

function int HandleRangerDefense(int respawning)
{
if (CheckInventory("QuakeClass") == 1)
{
    if (GetCvar("samsara_Rangerdefense") != -10) { TakeInventory("PowerDefenseBuff-10",1); }
    if (GetCvar("samsara_Rangerdefense") != -9)  { TakeInventory("PowerDefenseBuff-9",1); }
    if (GetCvar("samsara_Rangerdefense") != -8)  { TakeInventory("PowerDefenseBuff-8",1); }
    if (GetCvar("samsara_Rangerdefense") != -7)  { TakeInventory("PowerDefenseBuff-7",1); }
    if (GetCvar("samsara_Rangerdefense") != -6)  { TakeInventory("PowerDefenseBuff-6",1); }
    if (GetCvar("samsara_Rangerdefense") != -5)  { TakeInventory("PowerDefenseBuff-5",1); }
    if (GetCvar("samsara_Rangerdefense") != -4)  { TakeInventory("PowerDefenseBuff-4",1); }
    if (GetCvar("samsara_Rangerdefense") != -3)  { TakeInventory("PowerDefenseBuff-3",1); }
    if (GetCvar("samsara_Rangerdefense") != -2)  { TakeInventory("PowerDefenseBuff-2",1); }
    if (GetCvar("samsara_Rangerdefense") != -1)  { TakeInventory("PowerDefenseBuff-1",1); }
    if (GetCvar("samsara_Rangerdefense") != 1)   { TakeInventory("PowerDefenseBuff1",1); }
    if (GetCvar("samsara_Rangerdefense") != 2)   { TakeInventory("PowerDefenseBuff2",1); }
    if (GetCvar("samsara_Rangerdefense") != 3)   { TakeInventory("PowerDefenseBuff3",1); }
    if (GetCvar("samsara_Rangerdefense") != 4)   { TakeInventory("PowerDefenseBuff4",1); }
    if (GetCvar("samsara_Rangerdefense") != 5)   { TakeInventory("PowerDefenseBuff5",1); }
    if (GetCvar("samsara_Rangerdefense") != 6)   { TakeInventory("PowerDefenseBuff6",1); }
    if (GetCvar("samsara_Rangerdefense") != 7)   { TakeInventory("PowerDefenseBuff7",1); }
    if (GetCvar("samsara_Rangerdefense") != 8)   { TakeInventory("PowerDefenseBuff8",1); }
    if (GetCvar("samsara_Rangerdefense") != 9)   { TakeInventory("PowerDefenseBuff9",1); }
    if (GetCvar("samsara_Rangerdefense") != 10)  { TakeInventory("PowerDefenseBuff10",1); }
    if (GetCvar("samsara_Rangerdefense") != 11)  { TakeInventory("PowerDefenseBuff11",1); }
    if (GetCvar("samsara_Rangerdefense") != 12)  { TakeInventory("PowerDefenseBuff12",1); }
    if (GetCvar("samsara_Rangerdefense") != 13)  { TakeInventory("PowerDefenseBuff13",1); }
    if (GetCvar("samsara_Rangerdefense") != 14)  { TakeInventory("PowerDefenseBuff14",1); }
    if (GetCvar("samsara_Rangerdefense") != 15)  { TakeInventory("PowerDefenseBuff15",1); }
    if (GetCvar("samsara_Rangerdefense") != 16)  { TakeInventory("PowerDefenseBuff16",1); }
    if (GetCvar("samsara_Rangerdefense") != 17)  { TakeInventory("PowerDefenseBuff17",1); }
    if (GetCvar("samsara_Rangerdefense") != 18)  { TakeInventory("PowerDefenseBuff18",1); }
    if (GetCvar("samsara_Rangerdefense") != 19)  { TakeInventory("PowerDefenseBuff19",1); }
    if (GetCvar("samsara_Rangerdefense") != 20)  { TakeInventory("PowerDefenseBuff20",1); }
	
    if (GetCvar("samsara_Rangerdefense") == -10) { GiveInventory("PowerDefenseBuff-10",1); }
    if (GetCvar("samsara_Rangerdefense") == -9)  { GiveInventory("PowerDefenseBuff-9",1); }
    if (GetCvar("samsara_Rangerdefense") == -8)  { GiveInventory("PowerDefenseBuff-8",1); }
    if (GetCvar("samsara_Rangerdefense") == -7)  { GiveInventory("PowerDefenseBuff-7",1); }
    if (GetCvar("samsara_Rangerdefense") == -6)  { GiveInventory("PowerDefenseBuff-6",1); }
    if (GetCvar("samsara_Rangerdefense") == -5)  { GiveInventory("PowerDefenseBuff-5",1); }
    if (GetCvar("samsara_Rangerdefense") == -4)  { GiveInventory("PowerDefenseBuff-4",1); }
    if (GetCvar("samsara_Rangerdefense") == -3)  { GiveInventory("PowerDefenseBuff-3",1); }
    if (GetCvar("samsara_Rangerdefense") == -2)  { GiveInventory("PowerDefenseBuff-2",1); }
    if (GetCvar("samsara_Rangerdefense") == -1)  { GiveInventory("PowerDefenseBuff-1",1); }
    if (GetCvar("samsara_Rangerdefense") == 1)   { GiveInventory("PowerDefenseBuff1",1); }
    if (GetCvar("samsara_Rangerdefense") == 2)   { GiveInventory("PowerDefenseBuff2",1); }
    if (GetCvar("samsara_Rangerdefense") == 3)   { GiveInventory("PowerDefenseBuff3",1); }
    if (GetCvar("samsara_Rangerdefense") == 4)   { GiveInventory("PowerDefenseBuff4",1); }
    if (GetCvar("samsara_Rangerdefense") == 5)   { GiveInventory("PowerDefenseBuff5",1); }
    if (GetCvar("samsara_Rangerdefense") == 6)   { GiveInventory("PowerDefenseBuff6",1); }
    if (GetCvar("samsara_Rangerdefense") == 7)   { GiveInventory("PowerDefenseBuff7",1); }
    if (GetCvar("samsara_Rangerdefense") == 8)   { GiveInventory("PowerDefenseBuff8",1); }
    if (GetCvar("samsara_Rangerdefense") == 9)   { GiveInventory("PowerDefenseBuff9",1); }
    if (GetCvar("samsara_Rangerdefense") == 10)  { GiveInventory("PowerDefenseBuff10",1); }
    if (GetCvar("samsara_Rangerdefense") == 11)  { GiveInventory("PowerDefenseBuff11",1); }
    if (GetCvar("samsara_Rangerdefense") == 12)  { GiveInventory("PowerDefenseBuff12",1); }
    if (GetCvar("samsara_Rangerdefense") == 13)  { GiveInventory("PowerDefenseBuff13",1); }
    if (GetCvar("samsara_Rangerdefense") == 14)  { GiveInventory("PowerDefenseBuff14",1); }
    if (GetCvar("samsara_Rangerdefense") == 15)  { GiveInventory("PowerDefenseBuff15",1); }
    if (GetCvar("samsara_Rangerdefense") == 16)  { GiveInventory("PowerDefenseBuff16",1); }
    if (GetCvar("samsara_Rangerdefense") == 17)  { GiveInventory("PowerDefenseBuff17",1); }
    if (GetCvar("samsara_Rangerdefense") == 18)  { GiveInventory("PowerDefenseBuff18",1); }
    if (GetCvar("samsara_Rangerdefense") == 19)  { GiveInventory("PowerDefenseBuff19",1); }
    if (GetCvar("samsara_Rangerdefense") == 20)  { GiveInventory("PowerDefenseBuff20",1); }
}
return 1;
}