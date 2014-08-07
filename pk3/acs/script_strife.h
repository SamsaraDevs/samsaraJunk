int KilledOracle = 0;
int AwakenSpectres = 0;
int SpectreTarget;

script SAMSARA_STRIFEACCURACY (int which)
{
    if (which == -1) { which = CheckInventory("StrifeAccuracyCounter"); }
    which = middle(1, which, ACCURACYCOUNT) - 1;

    int pln = PlayerNumber();
    int pclass = samsaraClassNum();
    int unique = "";

    while (!strcmp(unique, ""))
    {
        unique = AccuracyUniques[pclass][which];
        which--;
    }

    GiveInventory(unique, 1);

    ACS_ExecuteAlways(SAMSARA_CLIENT_ACCURACY, 0, GetCVar("compat_silentpickup"),0,0);
}

script SAMSARA_CLIENT_ACCURACY (int soundmode) clientside
{
    int pln = PlayerNumber(), cpln = ConsolePlayerNumber();
    int pclass = samsaraClassNum();
    int i, j, quoteCount = 0;
    int logMsg;
    
    if (cpln == pln)
    {
        if (GetCVar("samsara_cl_moremessages"))
        {
            for (i = 0; i < MSGCOUNT; i++)
            {
                j = AccuracyMessages[pclass][i];
                if (!StrLen(j)) { continue; }
                
                QuoteStorage[quoteCount++] = j;
            }
            
            if (!quoteCount) { logMsg = "Oh bugger there's no messages for this unique."; }
            else { logMsg = QuoteStorage[random(0, quoteCount-1)]; }
        }
        else
        {
            logMsg = AccuracyMessages[pclass][0];
            
            if (!StrLen(logMsg)) { logMsg = "Oh bugger there's no message for this unique."; } 
        }

        if (GetCVar("samsara_cl_printpickup")) { Print(s:logMsg); }
        else { Log(s:logMsg); }
    }
    
    if (soundmode == 1) { LocalAmbientSound(ClassUniqueSounds[pclass], 127); }
    else { ActivatorSound(ClassUniqueSounds[pclass], 127); }
    
    FadeRange(ClassFades[pclass][0], ClassFades[pclass][1], ClassFades[pclass][2], ClassFades[pclass][3],
    ClassFades[pclass][0], ClassFades[pclass][1], ClassFades[pclass][2], 0.0, itof(ClassFades[pclass][4]) / 35);
}

script SAMSARA_BEACON (int noloop)
{
    int pln, classnum, monType, monTID, success = 0;
    int beaconTID = defaultTID(0);
    int x = GetActorX(0), y = GetActorY(0), z = GetActorZ(0);

    SetActivatorToTarget(0);
    
    pln = PlayerNumber();
    classnum = samsaraClassNum();

    if (pln == -1)
    {
        SetActorState(beaconTID, "WhatAmIDoingWithMyLife");
        terminate;
    }

    monType = BeaconMonsters[classnum];

    SetActivator(beaconTID);

    while (!success)
    {
        monTID = unusedTID(14000, 24000);
        success = Spawn(monType, x, y, z, monTID);
        if (success) { Spawn("TeleportFog", x, y, z); }

        if (noloop)
        {
            SetResultValue(success);

            if (!success) { SetActorState(beaconTID, "FuckYouImAPickup"); }
            terminate;
        }

        Delay(1);
    }
}

script SAMSARA_SPECTRES (int mode, int arg1, int arg2)
{
    int i,j,k,l, x,y,z;
    int result;

    switch (mode)
    {
      default:
        x = GetActorX(0);
        y = GetActorY(0);
        z = GetActorZ(0);

        for (i = 0; i < 50; i++)
        {
            j = unusedTID(5000, 10000);
            Spawn("AmmoSatchel", x,y,z, j);
            k = random(0, 1.0);
            l = random(0, 1.0);

            SetActorVelocity(j, 35 * FixedMul(cos(k), cos(l)), 35 * FixedMul(sin(k), cos(l)), 35 * sin(l), 0, 0);

            if (i % 4 == 0) { Delay(1); }
        }
        break;

      case 1:
        SamsaraGlobal[GLOBAL_SIGILBASE+1] = 1;
        Floor_LowerToLowest(999, 4);
        SetActivatorToTarget(0);
        GiveInventory("Communicator", 1);
        SendToCommunicator(95, 0, 0, 0);
        break;

      case 2:
        SamsaraGlobal[GLOBAL_SIGILBASE+2] = 1;
        SetActivatorToTarget(0);
        Print(l:"TXT_KILLED_BISHOP");
        GiveInventory("Communicator", 1);
        GiveInventory("QuestItem21", 1);
        SendToCommunicator(74, 0, 0, 0);
        break;

      case 3:
        SamsaraGlobal[GLOBAL_SIGILBASE+3] = 1;
        Door_Open(222, 8);
        KilledOracle = 1;        
        SetActivatorToTarget(0);
        GiveInventory("Communicator", 1);
        Print(l:"TXT_KILLED_ORACLE");
        GiveInventory("QuestItem23", 1);

        if (CheckInventory("QuestItem21")) { GiveInventory("QuestItem22", 1); }

        if (CheckInventory("QuestItem24")) { SendToCommunicator(85, 0, 0, 0); }
        else { SendToCommunicator(87, 0, 0, 0); }

        Delay(35);

        KilledOracle = 0;
        break;

      case 4:
        SamsaraGlobal[GLOBAL_SIGILBASE+4] = 1;
        SetActivatorToTarget(0);
        Print(l:"TXT_KILLED_MACIL");
        GiveInventory("Communicator", 1);
        GiveInventory("QuestItem24", 1);

        if (CheckInventory("QuestItem25")) { SendToCommunicator(106, 0, 0, 0); }
        else { SendToCommunicator(79, 0, 0, 0); }
        break;

      case 5:
        SamsaraGlobal[GLOBAL_SIGILBASE+5] = 1;
        Floor_LowerToLowest(666, 4);
        SetActivatorToTarget(0);

        Print(l:"TXT_KILLED_LOREMASTER");
        GiveInventory("Communicator", 1);
        GiveInventory("QuestItem26", 1);

        ACS_ExecuteAlways(SAMSARA_STRIFEACCURACY, 0, -1,0,0);
        GiveInventory("UpgradeStamina", 1);

        if (CheckInventory("QuestItem23") && CheckInventory("QuestItem24") && CheckInventory("QuestItem21"))
        {
            SendToCommunicator(85, 0, 0, 0);
        }
        else
        {
            SendToCommunicator(83, 0, 0, 0);
        }
        break;

      case 6:
        if (arg1 < 1 || arg1 > SIGILCOUNT)
        {
            Log(s:"\caERROR\c-: Illegal value for sigil splinter given to script ", d:SAMSARA_SPECTRES, s:" (", d:arg1, s:")");
            break;
        }

        SamsaraGlobal[GLOBAL_SIGILBASE + arg1] = !!arg2;
        break;
      
      case 7:
        result = 1;
        i = samsaraClassNum();

        switch (i)
        {
          case CLASS_QUAKE:
            if (!CheckInventory("QuadDamagePower"))
            {
                Print(s:"You must be quadded to go spectral.");
                result = 0;
            }
            break;

          case CLASS_HERETIC:
            if (!CheckInventory("PowerHereticTome"))
            {
                Print(s:"You must be tomed to go spectral.");
                result = 0;
            }
            break;
          
          default:
            if (!HasClassWeapon(i, 8))
            {
                result = 0;
                Print(s:"You need the \ck", s:ClassWeapons[i][8][S_WEP], s:"\c- to go spectral.");
            }
        }
        break;

      case -1:
        SetActivatorToTarget(0);
        i = defaultTID(-1);

        AwakenSpectres = 1;
        SpectreTarget = i;

        Delay(35);
        AwakenSpectres = 0;
        SpectreTarget = 0;
        break;

      case -2:
        if (AwakenSpectres)
        {
            Thing_Hate(0, SpectreTarget, 0);
            SetActorState(0, "See");
        }
        break;
        
      case -3:
        result = KilledOracle;
        break;
    }

    SetResultValue(result);
}

script SAMSARA_SIGIL (int baseHP)
{
    int i, splinterCount = 0;
    baseHP = itof(baseHP) / SIGILCOUNT;

    if (!CheckInventory("SpectralFiring")) { terminate; }

    for (i = 0; i < SIGILCOUNT; i++)
    {
        if (CheckInventory(SigilSplinters[i])) { splinterCount++; }
    }

    baseHP *= min((SIGILCOUNT - splinterCount) + 1, SIGILCOUNT);
    baseHP = round(baseHP);

    if (baseHP) DamageThing(baseHP, 0);
}
