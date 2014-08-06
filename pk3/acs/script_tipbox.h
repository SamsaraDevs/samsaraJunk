script SAMSARA_PUKE_CLIENT (int mode, int arg1, int arg2) net clientside
{
    switch (mode)
    {
      case 0:
        ClientTipClassModifier += arg1;
        break;

      case 1:
        ClientTipboxModifier += arg1;
        break;
    }
}

script SAMSARA_TIPBOX (void) net
{
    int pln = PlayerNumber();

    ClientTipboxes[pln] = !ClientTipboxes[pln];

    SetPlayerProperty(0, ClientTipboxes[pln], PROP_TOTALLYFROZEN);
    ACS_ExecuteAlways(SAMSARA_TIPBOX_CLIENT, 0, ClientTipboxes[pln], 0, 0);
}

script SAMSARA_TIPBOX_CLIENT (int tipon, int mode) clientside
{
    int tipclass;
    int tipnum = 0;
    int pln = PlayerNumber();

    int i, j, modc, modn, modx, mody, classmod, tipscroll;
    int isDM;

    if (GameType() != GAME_SINGLE_PLAYER && GameType() != GAME_NET_COOPERATIVE)
    {
        isDM = 1;
    }

    // Comment out for ZDoom
    if (ConsolePlayerNumber() != pln) { terminate; } // Unnecessary but nice to not have assumptions made

    if (PlayerIsSpectator(pln)) { tipclass = random(1, CLASSCOUNT) - 1; }
    else { tipclass = samsaraClassNum(); }

    ClientTipboxes[pln] = tipon;
    ClientTipboxModifier = 0;
    ClientTipClassModifier = 0;

    while (ClientTipboxes[pln])
    {
        SetHudSize(1024, 768, 1);

        for (i = -2; i <= 2; i++)
        {
            modc = mod(tipclass + i, CLASSCOUNT);
            modn = tipnum;

            j = 900 * tipscroll;
            j -= (j % 1.0);
            modx = itof(512 + (900 * i)) - j;


            //if (modx < 0) { modx -= 0.6; }
            //else { modx += 0.4; }

            j = "";
            if (isDM) { j = DMTipboxes[modc][modn]; }
            if (!strcmp(j, "")) { j = Tipboxes[modc][modn]; }

            SetFont(j);
            HudMessage(s:"A"; HUDMSG_FADEOUT, -6281 + i, CR_UNTRANSLATED, modx, 384.0, 1.0, 0.5);
        }

        classmod += keyPressed(BT_RIGHT) + keyPressed(BT_MOVERIGHT);
        classmod -= keyPressed(BT_LEFT) + keyPressed(BT_MOVELEFT);
        classmod += ClientTipClassModifier;
        
        tipscroll += itof(classmod) / TIP_SCROLLRATE;

        if (ftoi(abs(tipscroll)))
        {
            tipclass  += roundZero(tipscroll);
            classmod  -= roundZero(tipscroll);
            tipscroll %= 1.0;
        }

        if (classmod == 0)
        {
            classmod = -roundAway(tipscroll);
        }

        tipclass = mod(tipclass, CLASSCOUNT);

        tipnum -= keyPressed(BT_FORWARD) + keyPressed(BT_LOOKUP);
        tipnum += keyPressed(BT_BACK) + keyPressed(BT_LOOKDOWN);
        tipnum += ClientTipboxModifier;
        tipnum = mod(tipnum, TIPCOUNT);

        
        SetFont("SMALLFONT");
        if (PlayerIsSpectator(pln))
        {
            HudMessage(k:"samsara_tipleft", s:"\cj to scroll left";
                            HUDMSG_PLAIN, -8281, CR_GOLD, 362.4, 712.1, 1.0, 0.5);

            HudMessage(k:"samsara_tipright", s:"\cj to scroll right";
                            HUDMSG_PLAIN, -8282, CR_GOLD, 662.4, 712.1, 1.0, 0.5);

            HudMessage(k:"samsara_tipup", s:"\cj and \c-", k:"samsara_tipdown", s:"\cj change tipbox mode";
                            HUDMSG_PLAIN, -8283, CR_GOLD, 512.4, 726.1, 1.0, 0.5);
        }
        else
        {
            HudMessage(k:"+left", s:"\cj or \c-", k:"+moveleft", s:"\cj to scroll left";
                            HUDMSG_PLAIN, -8281, CR_GOLD, 362.4, 712.1, 1.0, 0.5);

            HudMessage(k:"+right", s:"\cj or \c-", k:"+moveright", s:"\cj to scroll right";
                            HUDMSG_PLAIN, -8282, CR_GOLD, 662.4, 712.1, 1.0, 0.5);

            HudMessage(s:"\cj(\c-", k:"+forward", s:"\cj, \c-", k:"+lookup", s:"\cj) and (\c-", k:"+back", s:"\cj, \c-", k:"+lookdown", s:"\cj) change tipbox mode";
                            HUDMSG_PLAIN, -8283, CR_GOLD, 512.4, 726.1, 1.0, 0.5);
        }

        ClientTipboxModifier = 0;
        ClientTipClassModifier = 0;
        Delay(1);
    }
}

script 302 (int changelogshit) NET CLIENTSIDE
{
switch (changelogshit)
{
        case 1:
        Log(s:ChangelogString);
        break;
		
        case 2:
        HudMessage(s:HELPSTR;
        HUDMSG_PLAIN|HUDMSG_LOG, 92712, CR_WHITE, 1.5, 0.2, 2.5);
        HudMessage(s:HELPSTR_CL;
        HUDMSG_PLAIN|HUDMSG_LOG, 92712, CR_WHITE, 1.5, 0.2, 2.5);
        HudMessage(s:"See the console for \cacvar information\c-.";
        HUDMSG_FADEOUT, 92712, CR_WHITE, 1.5, 0.2, 2.0, 0.5);
        break;
}
}
