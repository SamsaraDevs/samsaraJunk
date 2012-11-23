function int GiveClassWeapon(int class, int slot, int ammoMode)
{
    return _giveclassweapon(class, slot, ammoMode, 0);
}

function int _giveclassweapon(int class, int slot, int ammoMode, int dropped)
{
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

    if (!StrLen(weapon)) { return 0; }

    if (!CheckInventory(weapon)) { giveWep = 1; }
    if (!CheckInventory(ClassWeapons[class][slot][S_CHECKITEM])) { giveWep = 1; }

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

        if (DEBUG) { Print(s:"givescript is ", d:giveScript); }

        if (giveScript > 0)
        {
            success = ACS_ExecuteWithResult(giveScript, class, slot, dropped);
        }
        else
        {
            success = 1;
            GiveInventory(weapon, 1);

            if (array_pickupswitch[PlayerNumber()] && !hasWep &&
                    (array_pickupswitch[PlayerNumber()] >= 2 || slot > ClassWeaponSlot())
                && !PlayerIsBot(PlayerNumber()))
            {
                SetWeapon(ClassWeapons[class][slot][S_WEP]);
            }
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

    return !success;
}

function int HasClassWeapon(int class, int slot)
{
    int weapon = ClassWeapons[class][slot][S_WEP];
    int checkitem = ClassWeapons[class][slot][S_CHECKITEM];

    int hasWep, hasItem;

    if (!StrLen(weapon)) { return 0; }

    hasWep  = CheckInventory(weapon);
    hasItem = StrLen(checkitem) && CheckInventory(checkitem);

    return hasWep || hasItem;
}

function void GiveClassUnique(int class, int which)
{
    int unique, ammo, amax;
    
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
    
    GiveInventory("Backpack", 1);

    for (i = 0; i < SLOTCOUNT-1; i++) { GiveClassWeapon(classNum, i, 1); }

    if (GetCVar("samsara_lmsunique"))
    {
        for (i = 0; i < UNIQUECOUNT; i++) { GiveClassUnique(classNum, i); }
    }

    if (StrLen(LMSItems[classNum])) { GiveInventory(LMSITEMS[classNum], 1); }
    if (GetCVar("samsara_lmsult")) { GiveClassWeapon(classNum, SLOTCOUNT-1, 2); }

    if (lmsLevel)
    {
        SetActorProperty(0, APROP_Health, GetActorProperty(0, APROP_Health) + (100 * (lmsLevel-1)));
        GiveInventory(LMSArmors[lmsLevel], 1);
    }
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


function int ClassWeaponSlot(void)
{
    int pclass = samsaraClassNum();
    int weapon, i;

    for (i = 0; i < SLOTCOUNT; i++)
    {
        weapon = ClassWeapons[pclass][i][S_WEP];

        if (CheckWeapon(weapon)) { return i; }
    }

    return -1;
}

function int ConvertClassWeapons(int classnum)
{
    int i, j;
    int ret;
    if (classnum < 0) { classnum = samsaraClassNum(); }

    for (i = 0; i < CLASSCOUNT; i++)
    {
        if (i == classnum)
        {
            if (DEBUG) { Print(s:"skipping class #", d:classnum); }
            continue;
        }

        for (j = 0; j < SLOTCOUNT; j++)
        {
            if (HasClassWeapon(i, j))
            {
                TakeInventory(ClassWeapons[i][j][S_WEP], 0x7FFFFFFF);
                GiveInventory(ClassWeapons[classnum][j][S_WEP], 1);
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
