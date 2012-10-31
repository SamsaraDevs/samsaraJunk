function int GiveClassWeapon(int class, int slot, int ammoMode)
{
    int weapon = ClassWeapons[class][slot][0];
    int ammo1  = ClassWeapons[class][slot][1];
    int ammo2  = ClassWeapons[class][slot][2];
    int a1count = CheckInventory(ammo1);
    int a2count = CheckInventory(ammo2);

    int hasAmmo = 0;
    int giveWep = 0;

    if (!StrLen(weapon)) { return 0; }

    if (!CheckInventory(weapon)) { giveWep = 1; }

    if (StrLen(ammo1)) { hasAmmo |= 1; }
    if (StrLen(ammo2)) { hasAmmo |= 2; }

    if (((hasAmmo & 1) && a1count < GetAmmoCapacity(ammo1))
     || ((hasAmmo & 2) && a2count < GetAmmoCapacity(ammo2)))
    {
        giveWep = 1;
    }

    if (giveWep)
    {
        GiveInventory(weapon, 1);

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
        }
    }

    return !giveWep;
}

function int HasClassWeapon(int class, int slot)
{
    int weapon = ClassWeapons[class][slot][0];
    if (!StrLen(weapon)) { return 0; }

    return CheckInventory(weapon);
}

function void ApplyLMS(void)
{
    int classNum = samsaraClassNum();
    int lmsLevel = middle(0, GetCVar("sv_lmslife"), LMSMODES-1);
    int i;

    for (i = 0; i < SLOTCOUNT-1; i++)
    {
        GiveClassWeapon(classNum, i, 2);
    }

    if (GetCVar("sv_lmsult")) { GiveClassWeapon(classNum, SLOTCOUNT-1, 2); }
    
    if (lmsLevel)
    {
        SetActorProperty(0, APROP_Health, 100 * lmsLevel);
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
