int msgColors[22] = 
{
    "\ca", "\cb", "\cc", "\cd", "\ce", "\cf", "\cg", "\ch", "\ci", "\cj", "\ck",
    "\cl", "\cm", "\cn", "\co", "\cp", "\cq", "\cr", "\cs", "\ct", "\cu", "\cv"
};

script SAMSARA_CLIENT_CLASS (int slot) clientside
{
    int toClass = SamsaraClientClass-1;
    int displaymode = GetCVar("samsara_cl_pickupmode");
    int oldslot = slot;
    int success = 0;
    int punchdrunk =  IsPunchdrunk & 1;
    int pdUniques  = (IsPunchdrunk & 2) || punchdrunk;
    int pdSaws     = (IsPunchdrunk & 4) || punchdrunk;

    if (slot == SLOT_CHAINSAW && pdSaws) { slot = SLOT_PUNCHDRUNKSAW; }
    if (slot == SLOT_BFG9000 && punchdrunk && pdUniques) { slot = SLOT_UNIQUE; }

    while (punchdrunk)
    {
        if (slot == SLOT_PUNCHDRUNKSAW && pdSaws) { break; }
        if (slot == SLOT_UNIQUE && pdUniques) { break; }

        SetActorState(0, "Invisible");
        terminate;
    }
    
    int hasSlot = SamsaraClientWeps[slot];

    if (displaymode != 0)
    {
        if ((SamsaraItemFlash >= (Timer() - 35)) && (Timer() >= 70))
        {
            Spawn("SamsaraChangeFlash", GetActorX(0), GetActorY(0), GetActorZ(0));
        }
        
        if (toClass == -1)
        {
            SetActorState(0, "NoGuy");
            terminate;
        }
    }
    
    if (slot == SLOT_UNIQUE)
    {
        switch (displaymode)
        {
          case 0:
            SetActorState(0, "NoGuy");
            break;
            
          case 1:
            if (pdUniques) { success = SetActorState(0, PickupStates[toClass][7]); }
            if (!pdUniques || !success) { SetActorState(0, PickupStates[toClass][3]); }
            break;
            
          case 2:
            if (pdUniques) { success = SetActorState(0, PickupStates[toClass][4]); }
            if (!pdUniques || !success) { SetActorState(0, PickupStates[toClass][0]); }
            break;
        }
    }
    else switch (displaymode)
    {
      case 0:
        SetActorState(0, "NoGuy");
        break;
        
      case 1:
        if ((SamsaraClientWepFlashes[slot] >= (Timer() - 35)) && (Timer() >= 35))
        {
            Spawn("SamsaraChangeFlash2", GetActorX(0), GetActorY(0), GetActorZ(0));
        }
        
        switch (slot)
        {
          case SLOT_CHAINSAW:
          case SLOT_PUNCHDRUNKSAW:
            if (pdSaws)
            {
                if (hasSlot) { success = SetActorState(0, PickupStates[toClass][5]); }
                else         { success = SetActorState(0, PickupStates[toClass][6]); }
            }
            break;
        }

        if (!success)
        {
            if (hasSlot) { SetActorState(0, PickupStates[toClass][1]); }
            else         { SetActorState(0, PickupStates[toClass][2]); }
        }
        break;
        
      case 2:
        switch (slot)
        {
          case SLOT_CHAINSAW:
          case SLOT_PUNCHDRUNKSAW:
            if (pdSaws) { success = SetActorState(0, PickupStates[toClass][4]); }
            else { success = SetActorState(0, PickupStates[toClass][0]); }
            break;
        }
          
        if (!success) { SetActorState(0, PickupStates[toClass][0]); }
        break;
    }
}

/*
*      WARNING
*  This script is over 20 variables. Check here first for segfaults.
*  Also, keep an eye on this script for potential desync issues.
*  (current count: 25)
*/

script SAMSARA_GIVEWEAPON (int slot, int dropped, int silent)
{
    // clients were running this before, that's bad
    if (!IsServer) { terminate; }
    if (slot < 0 || slot >= SLOTCOUNT) { terminate; }    

    int weaponStay = !!GetCVar("sv_weaponstay");
    int punchdrunk = IsPunchdrunk & 1;
    int pdSaws     = (IsPunchdrunk & 4) || punchdrunk;

    // hijack chainsaw for punchdrunk saw
    if (pdSaws && slot == SLOT_CHAINSAW) { slot = SLOT_PUNCHDRUNKSAW; }

    if (punchdrunk)
    { 
        // do unique giving instead
        if (slot == SLOT_BFG9000)
        {
            SetResultValue(ACS_ExecuteWithResult(SAMSARA_GIVEUNIQUE, 0));
            terminate;
        }

        // no weapon for you go away
        if (slot != SLOT_PUNCHDRUNKSAW)
        {
            SetResultValue(weaponStay * WEPFLAGS_WEAPONSTAY);
            terminate;
        }
    }

    
    int weaponGet  = 0;
    int pclass = samsaraClassNum();
    int hasWep = HasClassWeapon(pclass, slot);
    
    if (slot == SLOT_BFG9000) { weaponStay = !!GetCVar("samsara_permault"); }
    
    int a1cnt  = 0, a2cnt = 0;
    int a1max  = 0, a2max = 0;
    int a1max2 = 0, a2max2 = 0;
    int a1Full = 0, a2Full = 0;
    int a1diff = 0, a2diff = 0;
    
    int weapon  = ClassWeapons[pclass][slot][S_WEP],        wepbool = !!StrLen(weapon); 
    int ammo1   = ClassWeapons[pclass][slot][S_AMMO1],      a1bool  = !!StrLen(ammo1);
    int ammo2   = ClassWeapons[pclass][slot][S_AMMO2],      a2bool  = !!StrLen(ammo2);
    int check   = ClassWeapons[pclass][slot][S_CHECKITEM],  chkbool = !!StrLen(check);
    
    // If we have the check-fail item, kill the pickup if this wasn't a drop
    // (it's for the wastems and LevelLimiter)
    // (the wastems are still fucking terrible)
    if (!wepbool || (CheckInventory(ClassWeapons[pclass][slot][S_CHECKFAILITEM]) && !dropped))
    {
        SetResultValue(weaponStay * WEPFLAGS_WEAPONSTAY);
        terminate;
    }
    
    if (a1bool)
    {
        a1cnt   = CheckInventory(ammo1);
        a1max   = GetAmmoCapacity(ammo1);
        a1max2  = a1max * 4;
        // so that when shaving off half the given ammo, we don't run into the ammo cap
        // and note an erroneous amount of ammo given
    }
    
    if (a2bool)
    {
        a2cnt   = CheckInventory(ammo2);
        a2max   = GetAmmoCapacity(ammo2);
        a2max2  = a1max * 4; // ditto
    }
    
    if (a1Bool) { a1Full = (CheckInventory(ammo1) == a1max); }
    if (a2Bool) { a2Full = (CheckInventory(ammo2) == a2max); }
    
    if (dropped)
    {
        if (a1bool) { SetAmmoCapacity(ammo1, a1max2); }
        if (a2bool) { SetAmmoCapacity(ammo2, a2max2); }
    }
    
    // check for both the weapon AND the check item
    //  if we miss either, we're giving the weapon
    if (!hasWep || (chkbool && !CheckInventory(check)))
    {
        weaponGet = 1;
    }
    else if (!weaponStay || dropped)
    {
        // still attempt a pickup if we can get ammo from it
        if ((a1bool && !a1Full) || (a2Bool && !a2Full)) { weaponGet = 1; }
    }

    if (weaponGet)
    {
        int success = _giveclassweapon(pclass, slot, 3, dropped, 0);

        if (!silent && success)
        {
            // this is for mugshot grins
            if (!hasWep)
            {
                Spawn("WeaponGetYaaaay", GetActorX(0), GetActorY(0), GetActorZ(0));
                Spawn("WeaponGetYaaaay2", GetActorX(0), GetActorY(0), GetActorZ(0));
            }

            ACS_ExecuteAlways(SAMSARA_CLIENT_WEAPONPICKUP, 0, slot,GetCVar("compat_silentpickup"),dropped);
        }
    }
    
    if (dropped)
    {
        // shave off half the ammo given
        TakeInventory(ammo1, (CheckInventory(ammo1) - a1cnt) / 2);
        TakeInventory(ammo2, (CheckInventory(ammo2) - a2cnt) / 2);

        // go back to normal ammo capacity
        if (a1bool) { SetAmmoCapacity(ammo1, a1max); }
        if (a2bool) { SetAmmoCapacity(ammo2, a2max); }
    }
    
    // make sure we're not over max ammo capacity
    TakeInventory(ammo1, CheckInventory(ammo1) - a1max);
    TakeInventory(ammo2, CheckInventory(ammo2) - a2max);
    
    // tell the pickup if weaponstay was on, and if the pickup succeeded
    SetResultValue((weaponStay * WEPFLAGS_WEAPONSTAY) + (success * WEPFLAGS_GOTWEAPON));
}

script SAMSARA_GIVEUNIQUE (int alt)
{
    if (!IsServer) { terminate; }
    
    int uniqueGet = 0;
    int pclass = samsaraClassNum();
    int pd = GetCVar("samsara_punchdrunk") || GetCVar("samsara_punchdrunkuniques");

    while (!uniqueGet && alt >= 0)
    {
        uniqueGet = GiveUnique(pclass, alt);
        alt--;
    }
    
    if (uniqueGet && IsServer)
    {
        ACS_ExecuteAlways(SAMSARA_CLIENT_UNIQUEPICKUP, 0, GetCVar("compat_silentpickup"), pd, 0);
    }
    
    SetResultValue(uniqueGet);
}

int QuoteStorage[MSGCOUNT];

script SAMSARA_CLIENT_WEAPONPICKUP (int slot, int soundmode, int dropped) clientside
{
    int pln = PlayerNumber(), cpln = ConsolePlayerNumber();
    int pclass = samsaraClassNum();
    int i, j, quoteCount = 0;
    int logMsg;
    int pickupsound = ClassPickupSounds[pclass][slot];
    
    if (dropped) { pickupsound = ClassDropSounds[pclass][slot]; }
    
    if (cpln == pln && GetCVar("msg") == 0)
    {
        if (GetCVar("samsara_cl_moremessages"))
        {
            for (i = 0; i < MSGCOUNT; i++)
            {
                j = ClassPickupMessages[pclass][slot][i];
                if (!StrLen(j)) { continue; }
                
                QuoteStorage[quoteCount++] = j;
            }
            
            if (!quoteCount) { logMsg = "Oh bugger there's no messages for this weapon."; }
            else { logMsg = QuoteStorage[random(0, quoteCount-1)]; }
        }
        else
        {
            logMsg = ClassPickupMessages[pclass][slot][0];
            
            if (!StrLen(logMsg)) { logMsg = "Oh bugger there's no message for this weapon."; } 
        }

        if (GetCVar("samsara_cl_printpickup")) { Print(s:logMsg); }
        else { Log(s:msgColors[GetCVar("msg0color")], s:logMsg); }
    }
    
    if (soundmode == 1) { LocalAmbientSound(pickupsound, 127); }
    else { ActivatorSound(pickupsound, 127); }
    
    FadeRange(ClassFades[pclass][0], ClassFades[pclass][1], ClassFades[pclass][2], ClassFades[pclass][3],
    ClassFades[pclass][0], ClassFades[pclass][1], ClassFades[pclass][2], 0.0, itof(ClassFades[pclass][4]) / 35);
    
    if (pclass == CLASS_DUKE && !GetCVar("samsara_cl_ballgag") && !dropped)
    {
        Delay(8);

        if (!DukeQuoteCooldown[pln] && !CheckInventory("DukeTauntCooldown"))
        {
            if (soundmode == 1) { LocalAmbientSound("duke/weapontaunt", 127); }
            else { ActivatorSound("duke/weapontaunt", 127); }
            GiveInventory("DukeTauntCooldown",5);
            ACS_ExecuteAlways(205,0,0);
            DukeQuoteCooldown[pln] = 140;
        }
    }
}

script SAMSARA_CLIENT_UNIQUEPICKUP (int soundmode, int punchdrunk) clientside
{
    int pln = PlayerNumber(), cpln = ConsolePlayerNumber();
    int pclass = samsaraClassNum();
    int i, j, quoteCount = 0;
    int logMsg;
    int pickupsound;

    if (punchdrunk) { pickupsound = PunchdrunkUniqueSounds[pclass]; }
    else { pickupsound = ClassUniqueSounds[pclass]; }
    
    if (cpln == pln && GetCVar("msg") == 0)
    {
        if (GetCVar("samsara_cl_moremessages"))
        {
            for (i = 0; i < MSGCOUNT; i++)
            {
                if (punchdrunk) { j = PunchDrunkUniqueMessages[pclass][i]; }
                else { j = ClassUniqueMessages[pclass][i]; }

                if (!StrLen(j)) { continue; }
                QuoteStorage[quoteCount++] = j;
            }
            
            if (!quoteCount) { logMsg = "Oh bugger there's no messages for this unique."; }
            else { logMsg = QuoteStorage[random(0, quoteCount-1)]; }
        }
        else
        {
            logMsg = ClassUniqueMessages[pclass][0];
            
            if (!StrLen(logMsg)) { logMsg = "Oh bugger there's no message for this unique."; } 
        }

        if (GetCVar("samsara_cl_printpickup")) { Print(s:logMsg); }
        else { Log(s:msgColors[GetCVar("msg0color")], s:logMsg); }
    }
    
    if (soundmode == 1) { LocalAmbientSound(pickupsound, 127); }
    else { ActivatorSound(pickupsound, 127); }
    
    FadeRange(ClassFades[pclass][0], ClassFades[pclass][1], ClassFades[pclass][2], ClassFades[pclass][3],
    ClassFades[pclass][0], ClassFades[pclass][1], ClassFades[pclass][2], 0.0, itof(ClassFades[pclass][4]) / 35);
}
