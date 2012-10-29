function int GiveClassWeapon(int class, int slot, int ammoMode)
{
    int weapon = ClassWeapons[class][slot][0];
    int ammo1  = ClassWeapons[class][slot][1];
    int ammo2  = ClassWeapons[class][slot][2];
    int a1count = CheckInventory(ammo1);
    int a2count = CheckInventory(ammo2);

    int hasAmmo = 0;
    int giveWep = 0;

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

        if (ammoMode == 1)
        {
            TakeInventory(ammo1, CheckInventory(ammo1) - a1count);
            TakeInventory(ammo2, CheckInventory(ammo2) - a2count);
        }
        if (ammoMode == 2)
        {
            GiveInventory(ammo1, GetAmmoCapacity(ammo1));
            GiveInventory(ammo2, GetAmmoCapacity(ammo2));
        }
    }

    return !giveWep;
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

function int itemToSlot(int i)
{
    i--;
    if (i < 0 || i > 6) { return -1; }
    return ItoSArray[i];
}

function int SamsaraClientVars(void)
{
    int ballgag         = !!GetCVar("cl_ballgag");
    int classicAnims    = !!GetCVar("cl_vanilladoom");
    int wolfmove        = !!GetCVar("cl_wolfmove");

    return (ballgag << 2) + (classicAnims << 1) + wolfmove;
}

