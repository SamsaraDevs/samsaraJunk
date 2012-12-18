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
