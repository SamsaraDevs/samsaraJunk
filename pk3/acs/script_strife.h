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

        if (noloop)
        {
            SetResultValue(success);

            if (!success) { SetActorState(beaconTID, "FuckYouImAPickup"); }
            terminate;
        }

        Delay(1);
    }

    Spawn("TeleportFog", x, y, z);
}
