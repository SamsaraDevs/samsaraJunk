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
    if (!strcmp(ClassWeapons[class][slot][S_CHECKITEM], "ForceCheck")
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
            if (!strcmp(j, "")) { continue; }

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

    if (strcmp(unique, "") &&
        ((!CheckInventory(unique) || ignoreinv) || 
          (uniqueMax <= 1 && (!strcmp(unammo, "") || unammoMax == 0)) ||
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
      case "AmmoShell":     return 4;
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

function int HandleBans(void)
{
    int classnum = samsaraClassNum();
    if (classnum == -1) { return 0; }

    int classbanned = GetCVar(BanCVars[classnum]);

    if (classbanned)
    {
        KickFromGame(PlayerNumber(), BanStrings[classnum]);
        return 1;
    }

    return 0;
}

function int HandleBuffCVars(int respawning)
{
    int classnum = samsaraClassNum();
    if (classnum == -1) { return 0; }

    int dmgcvar = DamageCVars[classnum];
    int defcvar = DefenseCVars[classnum];

    // Since the CVars can be negative, we add to the cvar readings a value we know
    //   points to the "zero" index in the DamageModes/DefenseModes arrays.
    //   That way, -10 points to index 0 if the zero point is at index 10.
    
    int damagelevel  = min(max(GetCVar(dmgcvar) + DAMAGEZEROINDEX, 0), DAMAGEMODES-1);
    int defenselevel = min(max(GetCVar(defcvar) + DEFENSEZEROINDEX, 0), DEFENSEMODES-1);

    int i, j;

    for (i = 0; i < DAMAGEMODES; i++)
    {
        j = CVarDamageItems[i];
        if (!StrLen(j)) { continue; }

        if (damagelevel == i)
        {
            if (!CheckInventory(j)) { GiveInventory(j, 1); }
        }
        else
        {
            TakeInventory(j, 0x7FFFFFFF);
        }
    }

    for (i = 0; i < DEFENSEMODES; i++)
    {
        j = CVarDefenseItems[i];
        if (!StrLen(j)) { continue; }

        if (defenselevel == i)
        {
            if (!CheckInventory(j)) { GiveInventory(j, 1); }
        }
        else
        {
            TakeInventory(j, 0x7FFFFFFF);
        }
    }

    return 1;
}
