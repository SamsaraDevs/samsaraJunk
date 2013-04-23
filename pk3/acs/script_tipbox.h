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
            if (j == "") { j = Tipboxes[modc][modn]; }

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
		Log(s:"FROM 0.28 TO 0.29");
Log(s:"========================");
Log(s:"- Further separated Samsara character scripts from other non-Samsara chars.");
Log(s:"- Ranger's Spectral weapons no longer use the old DoE ammo switching style.");
Log(s:"- Spectral Laser Cannon no longer flickers.");
Log(s:"- Spectral Thunderbolt no longer uses cells.");
Log(s:"- Spectral LAZ Device zorches properly.");
Log(s:"- Ranger no longer keeps his quadded face when he dies.");
Log(s:"- (ZDoom) The SO's lightamp now does BlueMap, as expected. BlueMap still broken in Zandroland.");
Log(s:"- The Thunderbolt discharge now actually escapes the sector it's in. Apparently low-radius projectiles don't explode properly.");
Log(s:"- sv_degeneration disables Ranger's degeneration, so he isn't hit with both of them.");
Log(s:"- Corvus' Tomed Phoenix Rod was getting confused with the Spectral Tomed Phoenix Rod. This was fixed.");
Log(s:"- Double-tapping Duke's jetpack to try and extend the lifespan of it now actually dramatically decreases its lifespan.");
Log(s:"- Spectre 2 and 4 are no longer immune to every non-Spectral weapon.");
Log(s:"- When using vanilla animations, Doomguy's pistol muzzle flash is no longer wildly to the side.");
Log(s:"- Hexen armor is now brown, and red armor is actually red again.");
Log(s:"- You can now see what armor type you have when using hud_althud, as you would expect.");
Log(s:"- Yellow armor is in for any people mapping for Samsara. It gives 150 AP at 40% protection, and its DoomEdNum is 19001.");
Log(s:"- samsara_cl_noadditivepickups has been added, for those whose bloom makes it near impossible to see the things.");
Log(s:"- sv_degeneration disables Ranger's degeneration, so he isn't hit with both of them.");
Log(s:"- sv_shotgunstart is now respected.");
Log(s:"- Duke pipebombs and devastators now make two explosion sounds; one local and loud, one global and quiet (with the pipebomb global sound limited).");
Log(s:"- The Fusion Pistol overcharge can no longer be used for griefing.");
Log(s:"- Apparently the Fusion Pistol didn't use ammo when overcharging? Okay then.");
Log(s:"- The first Fusion Pistol beep is now silent, and you can now charge for one more second.");
Log(s:"- The Quad Damage no longer plays weapons/sigil when powering down.");
Log(s:"- The Quad Damage now makes the 'no item' sound from Quake 3 if it's used when it's recharging.");
Log(s:"- Ranger now has invisibility to Quake's Ring of Shadows! He's much less visible when picking up a blursphere (10% visibility), and his weapons don't alert monsters.");
Log(s:"- B.J. Blazkowic's Machine Gun now gives 20 of ammo 1 on pickup, rather than 6.");
Log(s:"- Duke's Freezethrower sprites have been adjusted for better Widescreen compatibility.");
Log(s:"- The changelog has been changed from a graphic to a textdump.");
Log(s:"- The SO has been given generic color painflashes for easier add-on compatibility.");
Log(s:"- The SO no longer gains one cell upon raising the TOZT again.");
Log(s:"- Duke burning and freezing now causes him to lose his inventory stuff and triggers the 'press any button to respawn' message.");
Log(s:"- The SO's Marathon 2 Alien Weapon is now undroppable.");
Log(s:"- The Hectebus no longer gets locked in firing.");
Log(s:"- Weapon pickup messages now take into account msg0color.");
Log(s:"- BlackFish has provided cleaner rocketlauncher sprites for Quakeguy.");
Log(s:"- B.J. has had his Clip ammo doubled upon picking up Ammo 2.");
        break;
		
        /*case 2:
        SetHudSize(1024,768,1);
        SetFont("");
        HudMessage(s:"";HUDMSG_PLAIN,1,1,512.0,384.0,0);
        break;*/
}
}