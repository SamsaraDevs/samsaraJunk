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

    while (unique == "")
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

script SAMSARA_SPECTRES (int mode)
{
    int i,j,k,l, x,y,z;

    GiveInventory("Communicator", 1);

    if (mode != -2 && mode != -3) { PrintBold(s:"Mode is ", d:mode); }

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
        Floor_LowerToLowest(999, 4);
        SetActivatorToTarget(0);
        SendToCommunicator(95, 0, 0, 0);
        break;

      case 2:
        SetActivatorToTarget(0);
        Print(l:"TXT_KILLED_BISHOP");
        SendToCommunicator(74, 0, 0, 0);
        break;

      case 3:
        Door_Open(222, 8);
        KilledOracle = 1;        
        SetActivatorToTarget(0);
        Print(l:"TXT_KILLED_ORACLE");
        GiveInventory("QuestItem23", 1);

        if (CheckInventory("QuestItem21")) { GiveInventory("QuestItem22", 1); }

        if (CheckInventory("QuestItem24")) { SendToCommunicator(85, 0, 0, 0); }
        else { SendToCommunicator(87, 0, 0, 0); }

        Delay(35);

        KilledOracle = 0;
        break;

      case 4:
        SetActivatorToTarget(0);
        Print(l:"TXT_KILLED_MACIL");
        GiveInventory("QuestItem24", 1);

        if (CheckInventory("QuestItem25")) { SendToCommunicator(106, 0, 0, 0); }
        else { SendToCommunicator(79, 0, 0, 0); }
        break;

      case 5:
        Floor_LowerToLowest(666, 4);
        SetActivatorToTarget(0);

        Print(l:"TXT_KILLED_LOREMASTER");
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
        SetResultValue(KilledOracle);
        break;
    }
}
