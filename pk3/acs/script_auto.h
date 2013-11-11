script SAMSARA_OPEN open
{
    IsServer = 1;
    int opd, pd;
    int opdu, pdu;
    int opds, pds;
        
    if (!GetCVar("samsara_cvarinfo"))
    {   ConsoleCommand("set samsara_cvarinfo 0");
    ConsoleCommand("archivecvar samsara_cvarinfo"); }
    
    if (SamsaraGlobal[GLOBAL_DONEBITCHING] == 0)
    {
        if (GameType() == GAME_TITLE_MAP)
        {
        }
        if (isSinglePlayer())
        {
            if(GetCvar("samsara_cvarinfo") == 0)
            {
            HudMessage(s:HELPSTR;
            HUDMSG_PLAIN|HUDMSG_LOG, 92712, CR_WHITE, 1.5, 0.2, 2.5);
            HudMessage(s:HELPSTR_CL;
            HUDMSG_PLAIN|HUDMSG_LOG, 92712, CR_WHITE, 1.5, 0.2, 2.5);
            HudMessage(s:"\cjSee the console for \cacvar information\c-.";
            HUDMSG_FADEOUT, 92712, CR_WHITE, 1.5, 0.2, 2.0, 0.5);
            ConsoleCommand("set samsara_cvarinfo 1");
            ConsoleCommand("archivecvar samsara_cvarinfo");
            }
        }
        else
        {
            Log(s:HELPSTR);
        }
        
        SamsaraGlobal[GLOBAL_DONEBITCHING] = 1;
    }

    ArmorMode = middle(0, GetCVar("samsara_armormode"), ARMORMODES-1);
    CheckMapArmors();
    
    while (1)
    {
        // I'd use defaultCVar but best-ever breaks on it for some reason :/
        
        if (!GetCVar("samsara_banjetpack"))
        {   ConsoleCommand("set samsara_banjetpack 0");
        ConsoleCommand("archivecvar samsara_banjetpack"); }
        
        if (!GetCVar("samsara_banwolfmove"))
        {   ConsoleCommand("set samsara_banwolfmove 0");
        ConsoleCommand("archivecvar samsara_banwolfmove"); }
        
        if (!GetCVar("samsara_lmslife"))
        {   ConsoleCommand("set samsara_lmslife 0");
        ConsoleCommand("archivecvar samsara_lmslife"); }
        
        if (GetCVar("samsara_lmsunique"))
        {
            ConsoleCommand("unset samsara_lmsunique");

            if (!GetCVar("samsara_uniquestart"))
            {
                ConsoleCommand("set samsara_uniquestart 2");
                ConsoleCommand("archivecvar samsara_uniquestart");
            }
        }
        
        if (!GetCVar("samsara_lmsult"))
        {   ConsoleCommand("set samsara_lmsult 0");
        ConsoleCommand("archivecvar samsara_lmsult"); }
        
        if (!GetCVar("samsara_nocustomgravity"))
        {   ConsoleCommand("set samsara_nocustomgravity 0");
        ConsoleCommand("archivecvar samsara_nocustomgravity"); }
        
        if (!GetCVar("samsara_permault"))
        {   ConsoleCommand("set samsara_permault 0");
        ConsoleCommand("archivecvar samsara_permault"); }
        
        if (!GetCVar("samsara_jumpmod"))
        {   ConsoleCommand("set samsara_jumpmod 0");
        ConsoleCommand("archivecvar samsara_jumpmod"); }
        
        if (!GetCVar("samsara_peoplediewhentheyarekilled"))
        {   ConsoleCommand("set samsara_peoplediewhentheyarekilled 0");
        ConsoleCommand("archivecvar samsara_peoplediewhentheyarekilled"); }
        
        if (!GetCVar("compat_clientssendfullbuttoninfo"))
        {   ConsoleCommand("set compat_clientssendfullbuttoninfo 1");
        ConsoleCommand("archivecvar compat_clientssendfullbuttoninfo"); }
        
        if (!GetCVar("samsara_armormode"))
        {   ConsoleCommand("set samsara_armormode 0");
        ConsoleCommand("archivecvar samsara_armormode"); }
        
        if (!GetCVar("samsara_nohealthcap"))
        {   ConsoleCommand("set samsara_nohealthcap 0");
        ConsoleCommand("archivecvar samsara_nohealthcap"); }
        
        if (!GetCVar("samsara_chainsawstart"))
        {   ConsoleCommand("set samsara_chainsawstart 0");
        ConsoleCommand("archivecvar samsara_chainsawstart"); }
        
        if (!GetCVar("samsara_uniquestart"))
        {   ConsoleCommand("set samsara_uniquestart 0");
        ConsoleCommand("archivecvar samsara_uniquestart"); }
        
        if (!GetCVar("samsara_punchdrunk"))
        {   ConsoleCommand("set samsara_punchdrunk 0");
        ConsoleCommand("archivecvar samsara_punchdrunk"); }
        
        if (!GetCVar("samsara_nomonologues"))
        {   ConsoleCommand("set samsara_nomonologues 0");
        ConsoleCommand("archivecvar samsara_nomonologues"); }
        
        if (!GetCVar("samsara_punchdrunkuniques"))
        {   ConsoleCommand("set samsara_punchdrunkuniques 0");
        ConsoleCommand("archivecvar samsara_punchdrunkuniques"); }
        
        if (!GetCVar("samsara_punchdrunksaws"))
        {   ConsoleCommand("set samsara_punchdrunksaws 0");
        ConsoleCommand("archivecvar samsara_punchdrunksaws"); }
        
        if (!GetCVar("samsara_noult"))
        {   ConsoleCommand("set samsara_noult 0");
        ConsoleCommand("archivecvar samsara_noult"); }
        
        if (!GetCVar("samsara_backpackstart"))
        {   ConsoleCommand("set samsara_backpackstart 0");
        ConsoleCommand("archivecvar samsara_backpackstart"); }
        
        if (!GetCVar("samsara_superturboturkeypuncher3000"))
        {   ConsoleCommand("set samsara_superturboturkeypuncher3000 0");
        ConsoleCommand("archivecvar samsara_superturboturkeypuncher3000"); }
        
        if (!GetCVar("samsara_superturboturkeylimit"))
        {   ConsoleCommand("set samsara_superturboturkeylimit 0");
        ConsoleCommand("archivecvar samsara_superturboturkeylimit"); }
        
        if (!GetCVar("samsara_superturboturkeyfrequency"))
        {   ConsoleCommand("set samsara_superturboturkeyfrequency 35");
        ConsoleCommand("archivecvar samsara_superturboturkeyfrequency"); }
        
        if (!GetCVar("samsara_classiclaz"))
        {   ConsoleCommand("set samsara_classiclaz 0");
        ConsoleCommand("archivecvar samsara_classiclaz"); }
        
        if (!GetCVar("samsara_allcanrj"))
        {   ConsoleCommand("set samsara_allcanrj 0");
        ConsoleCommand("archivecvar samsara_allcanrj"); }
        
        if (!GetCVar("samsara_nounique"))
        {   ConsoleCommand("set samsara_nounique 0");
        ConsoleCommand("archivecvar samsara_nounique"); }
        
        if (!GetCVar("samsara_noinvuln"))
        {   ConsoleCommand("set samsara_noinvuln 0");
        ConsoleCommand("archivecvar samsara_noinvuln"); }
        
        if (!GetCVar("samsara_bandoomguy"))
        {   ConsoleCommand("set samsara_bandoomguy 0");
        ConsoleCommand("archivecvar samsara_bandoomguy"); }
        
        if (!GetCVar("samsara_banchex"))
        {   ConsoleCommand("set samsara_banchex 0");
        ConsoleCommand("archivecvar samsara_banchex"); }
        
        if (!GetCVar("samsara_bancorvus"))
        {   ConsoleCommand("set samsara_bancorvus 0");
        ConsoleCommand("archivecvar samsara_bancorvus"); }
        
        if (!GetCVar("samsara_banbj"))
        {   ConsoleCommand("set samsara_banbj 0");
        ConsoleCommand("archivecvar samsara_banbj"); }
        
        if (!GetCVar("samsara_banparias"))
        {   ConsoleCommand("set samsara_banparias 0");
        ConsoleCommand("archivecvar samsara_banparias"); }
        
        if (!GetCVar("samsara_banduke"))
        {   ConsoleCommand("set samsara_banduke 0");
        ConsoleCommand("archivecvar samsara_banduke"); }
        
        if (!GetCVar("samsara_banso"))
        {   ConsoleCommand("set samsara_banso 0");
        ConsoleCommand("archivecvar samsara_banso"); }
        
        if (!GetCVar("samsara_banranger"))
        {   ConsoleCommand("set samsara_banranger 0");
        ConsoleCommand("archivecvar samsara_banranger"); }
        
        if (!GetCVar("samsara_lmsrules"))
        {   ConsoleCommand("set samsara_lmsrules 0");
        ConsoleCommand("archivecvar samsara_lmsrules"); }
        
        if (!GetCVar("samsara_doomguydamage"))
        {   ConsoleCommand("set samsara_doomguydamage 0");
        ConsoleCommand("archivecvar samsara_doomguydamage"); }
        
        if (!GetCVar("samsara_doomguydefense"))
        {   ConsoleCommand("set samsara_doomguydefense 0");
        ConsoleCommand("archivecvar samsara_doomguydefense"); }
        
        if (!GetCVar("samsara_chexdamage"))
        {   ConsoleCommand("set samsara_chexdamage 0");
        ConsoleCommand("archivecvar samsara_chexdamage"); }
        
        if (!GetCVar("samsara_chexdefense"))
        {   ConsoleCommand("set samsara_chexdefense 0");
        ConsoleCommand("archivecvar samsara_chexdefense"); }
        
        if (!GetCVar("samsara_corvusdamage"))
        {   ConsoleCommand("set samsara_corvusdamage 0");
        ConsoleCommand("archivecvar samsara_corvusdamage"); }
        
        if (!GetCVar("samsara_corvusdefense"))
        {   ConsoleCommand("set samsara_corvusdefense 0");
        ConsoleCommand("archivecvar samsara_corvusdefense"); }
        
        if (!GetCVar("samsara_bjdamage"))
        {   ConsoleCommand("set samsara_bjdamage 0");
        ConsoleCommand("archivecvar samsara_bjdamage"); }
        
        if (!GetCVar("samsara_bjdefense"))
        {   ConsoleCommand("set samsara_bjdefense 0");
        ConsoleCommand("archivecvar samsara_bjdefense"); }
        
        if (!GetCVar("samsara_pariasdamage"))
        {   ConsoleCommand("set samsara_pariasdamage 0");
        ConsoleCommand("archivecvar samsara_pariasdamage"); }
        
        if (!GetCVar("samsara_pariasdefense"))
        {   ConsoleCommand("set samsara_pariasdefense 0");
        ConsoleCommand("archivecvar samsara_pariasdefense"); }
        
        if (!GetCVar("samsara_dukedamage"))
        {   ConsoleCommand("set samsara_dukedamage 0");
        ConsoleCommand("archivecvar samsara_dukedamage"); }
        
        if (!GetCVar("samsara_dukedefense"))
        {   ConsoleCommand("set samsara_dukedefense 0");
        ConsoleCommand("archivecvar samsara_dukedefense"); }
        
        if (!GetCVar("samsara_sodamage"))
        {   ConsoleCommand("set samsara_sodamage 0");
        ConsoleCommand("archivecvar samsara_sodamage"); }
        
        if (!GetCVar("samsara_sodefense"))
        {   ConsoleCommand("set samsara_sodefense 0");
        ConsoleCommand("archivecvar samsara_sodefense"); }
        
        if (!GetCVar("samsara_rangerdamage"))
        {   ConsoleCommand("set samsara_rangerdamage 0");
        ConsoleCommand("archivecvar samsara_rangerdamage"); }
        
        if (!GetCVar("samsara_rangerdefense"))
        {   ConsoleCommand("set samsara_rangerdefense 0");
        ConsoleCommand("archivecvar samsara_rangerdefense"); }
        
        if (!GetCVar("samsara_permaquad"))
        {   ConsoleCommand("set samsara_permaquad 0");
        ConsoleCommand("archivecvar samsara_permaquad"); }

        opd = pd;
        pd = !!GetCVar("samsara_punchdrunk");

        opdu = pdu;
        pdu = !!GetCVar("samsara_punchdrunkuniques");

        opds = pds;
        pds = !!GetCVar("samsara_punchdrunksaws");

        if (pd != opd || opdu != pdu || opds != pds || Timer() % 18 == 0)
        {
            IsPunchdrunk  = pd  << 0;
            IsPunchdrunk |= pdu << 1;
            IsPunchdrunk |= pds << 2;
            ACS_ExecuteAlways(SAMSARA_CLIENT_DECORATE, 0, 8, IsPunchdrunk);
        }
        
        Delay(1);
    }
}

script SAMSARA_ENTER enter { ACS_ExecuteWithResult(SAMSARA_SPAWN, 0,0,0); }
script SAMSARA_RESPAWN respawn { ACS_ExecuteWithResult(SAMSARA_SPAWN, 1,0,0); }
script SAMSARA_RETURN return { UnloadingNow = 0; ACS_ExecuteWithResult(SAMSARA_SPAWN, 0,0,0); }

script SAMSARA_SPAWN (int respawning)
{
    int pln = PlayerNumber();
    int pcount, opcount;
    int startTime = Timer();
    int endloop;
    int canbuddha;
    int wsteSide;
    int armor, oarmor, type, otype;
    int i;

    ServerEnterTimes[pln] = startTime;
    ACS_ExecuteWithResult(SAMSARA_SYNTHFIRE, startTime);

    if (!CheckInventory("IsSamsaraClass")) { terminate; }


    ACS_ExecuteAlways(SAMSARA_ENTER_CLIENT, 0, startTime,0,0);
    ACS_ExecuteWithResult(SAMSARA_WOLFMOVE, startTime,0,0);
    ACS_ExecuteWithResult(SAMSARA_QPOWERS,  startTime,0,0);
    
    if (isLMS()) { ApplyLMS(); }
    if (GetCvar("samsara_lmsrules") == 1)
    {
        ApplyLMS();
        GiveInventory("CanDualShotties",1);
    }
    if (isSinglePlayer()) { SamsaraWepType = samsaraClassNum()+1; }

    if (!respawning)
    {
        ClientTipboxes[pln] = 0;
        ACS_ExecuteAlways(SAMSARA_SCHEDULED, 0, respawning,1,0);

        if (GetCVar("sv_shotgunstart") > 0) { GiveClassWeapon(samsaraClassNum(), 3, 3);}
        if (GetCvar("samsara_backpackstart") == 1) { GiveInventory("Backpack",1); }
    }
    else
    { if (GameType() != GAME_NET_COOPERATIVE)
    {
        if (GetCVar("sv_shotgunstart") > 0) { GiveClassWeapon(samsaraClassNum(), 3, 3);}
        if (GetCvar("samsara_backpackstart") == 1) { GiveInventory("Backpack",1); }
    }}

    HandleChainsawSpawn(respawning);
    HandleUniqueSpawn(respawning);
    HandlePunchdrunk(respawning);
    HandleInstagib(respawning);

    ACS_ExecuteAlways(SAMSARA_SCHEDULED, 0, respawning,0,0);

    switch (samsaraClassNum())
    {
      case CLASS_DUKE:
        if (!respawning) { GiveInventory("DukeReady", 1); }
        break;
        
      case CLASS_MARATHON:
        if (isInvasion()) { GiveInventory("InvasionDualShottyCheck", 1); }
        break;
    }

    if (isSinglePlayer())
    {
        GiveInventory("SPModeOn", 1);
    }
    else if (isCoop())
    {
        GiveInventory("CoopModeOn", 1);
        SetActorState(0, "CoOpModeOn");
    }

    pcount = PlayerCount();
    
    while (!endloop && ServerEnterTimes[pln] == startTime)
    {
        
        if (array_wolfmove[pln]) { GiveInventory("WolfenMovement", 1); }
        else { TakeInventory("WolfenMovement", 0x7FFFFFFF); }
        
        if (array_ballgag[pln]) { GiveInventory("DukeBallgag", 1); }
        else { TakeInventory("DukeBallgag", 0x7FFFFFFF); }
        
        if (array_vanillaAnim[pln]) { GiveInventory("VanillaDoom", 1); }
        else { TakeInventory("VanillaDoom", 0x7FFFFFFF); }
        
        if (array_weaponBar[pln]) { GiveInventory("ExpandedHud", 1); }
        else { TakeInventory("ExpandedHud", 0x7FFFFFFF); }

        if (GetCVar("dmflags2") & 256) { TakeInventory("DoomNoBFGAim", 0x7FFFFFFF); }
        else { GiveInventory("DoomNoBFGAim", 1); }

        if (GetCVar("samsara_classiclaz") == 1) { GiveInventory("ChexClassicLaz", 1); }
        else { TakeInventory("ChexClassicLaz", 0x7FFFFFFF); }

        if (GetCVar("samsara_allcanrj") == 1) { GiveInventory("RJingModeOn", 1); }
        else { TakeInventory("RJingModeOn", 0x7FFFFFFF); }
        
        HandleBans();
        HandleBuffCVars(respawning);
        
        TakeInventory("WeaponGetYaaaay",  1);
        TakeInventory("WeaponGetYaaaay2", 1);
        TakeInventory("Mace", 1);
        TakeInventory("MacePowered", 1);
        ConvertClassWeapons(-1);

        if (CheckInventory("SpectralFiring"))
        {
            TakeInventory("SpectralFiring", 1);
            GiveInventory("SpectralFiring", 1);
        }

        if (GetCVar("samsara_banjetpack") && CheckInventory("DukePortJetpack"))
        {
            GiveInventory("DukeBootserk", 1);
            TakeInventory("DukePortJetpack", 0x7FFFFFFF);
            TakeInventory("DukeJetpackFuel", 0x7FFFFFFF);
            Print(s:"The jetpack is banned on this server. Have 8x boot damage instead.");
        }

        for (i = 0; i < 10; i++)
        {
            if (CheckInventory("DoomZerkCounter") > i && !CheckInventory(DoomAccuracyZerks[i]))
            {
                GiveInventory(DoomAccuracyZerks[i], 1);
            }
        }

        for (i = 0; i < SIGILCOUNT; i++)
        {
            if (SamsaraGlobal[GLOBAL_SIGILBASE+1+i])
            {
                if (!SamsaraGlobal[GLOBAL_SIGILBASE+i+2]) { GiveInventory(SpectralTogglers[i], 1); }
                else { TakeInventory(SpectralTogglers[i], 0x7FFFFFFF); }
                GiveInventory(SigilSplinters[i], 1);
            }
            else
            {
                TakeInventory(SigilSplinters[i], 0x7FFFFFFF);
                TakeInventory(SpectralTogglers[i], 0x7FFFFFFF);
            }
        }

        // useless but harmless in Zandro - triggers buddha mode when Blazko has an extra life in ZDoom
        if (CheckInventory("WolfExtraLife") && !canbuddha) { SetPlayerProperty(0, 1, 16); }
        if (!CheckInventory("WolfExtraLife") && canbuddha) { SetPlayerProperty(0, 0, 16); }
        canbuddha = CheckInventory("WolfExtraLife");

        i = CheckInventory("Shell");

        if (CheckInventory("RWastemUnloaded")) { wsteSide = 1; }
        if (CheckInventory("LWastemUnloaded")) { wsteSide = 0; }

        if (i <= 2)
        {
            if (!CheckInventory("WastemEmpty"))  { GiveInventory("WastemEmpty", 1); };
            if (!CheckInventory("LWastemEmpty")) { GiveInventory("LWastemEmpty", 1); };
            if (!CheckInventory("RWastemEmpty")) { GiveInventory("RWastemEmpty", 1); };
        }
        else if (i <= 4)
        {
            if (CheckInventory("WastemEmpty"))  { TakeInventory("WastemEmpty", 0x7FFFFFFF); };

            if (!wsteSide)
            {
                if (!CheckInventory("LWastemEmpty")) { GiveInventory("LWastemEmpty", 1); };
                if (CheckInventory("RWastemEmpty")) { TakeInventory("RWastemEmpty", 0x7FFFFFFF); };
            }
            else
            {
                if (!CheckInventory("RWastemEmpty")) { GiveInventory("RWastemEmpty", 1); };
                if (CheckInventory("LWastemEmpty")) { TakeInventory("LWastemEmpty", 0x7FFFFFFF); };
            }
        }
        else
        {
            if (CheckInventory("WastemEmpty"))  { TakeInventory("WastemEmpty", 0x7FFFFFFF); };
            if (CheckInventory("LWastemEmpty") && !wsteSide) { TakeInventory("LWastemEmpty", 0x7FFFFFFF); };
            if (CheckInventory("RWastemEmpty") &&  wsteSide) { TakeInventory("RWastemEmpty", 0x7FFFFFFF); };
        }

        oarmor = armor;
        otype = type;
        armor = CheckInventory("Armor");
        type = SamsaraArmorType();

        if (oarmor > armor && otype == 3)
        {
            if (samsaraClassNum() == CLASS_MARATHON)
            {
                ActivatorSound("marathon/shieldhit", 127);
                FadeRange(255, 255, 255, 0.4, 0, 0, 0, 0.0, 8.0 / 35);
            }
            else
            {
                ActivatorSound("samsara/shieldhit", 127);
                FadeRange(255, 255, 0, min(0.5, (oarmor - armor) * 0.015), 0, 0, 0, 0.0, min(35.0, 1.5 * (oarmor - armor)) / 35);
            }
        }

        /*
         * Jumping shit
         */

        switch (samsaraClassNum())
        {
          case CLASS_MARATHON:
            if (GetCVar("samsara_nocustomgravity")) { SetActorProperty(0, APROP_Gravity, 1.0); }
            else { SetActorProperty(0, APROP_Gravity, 0.15); }
            break;

          case CLASS_QUAKE:
            if (GetCVar("samsara_nocustomgravity")) { SetActorProperty(0, APROP_Gravity, 1.0); }
            else { SetActorProperty(0, APROP_Gravity, 0.75); }
            break;

          default:
            SetActorProperty(0, APROP_Gravity, 1.0);
            break;
        }

        if (CheckInventory("ForceRangerGravity")) { SetActorProperty(0, APROP_Gravity, 0.75); }
        if (CheckInventory("ForceSOGravity")) { SetActorProperty(0, APROP_Gravity, 0.15); }

        if (samsaraClassNum() == CLASS_HEXEN) { i = JumpZFromHeight(41 + GetCVar("samsara_jumpmod"), GetActorProperty(0, APROP_Gravity)); }
        else { i = JumpZFromHeight(32 + GetCVar("samsara_jumpmod"), GetActorProperty(0, APROP_Gravity)); }

        SetActorProperty(0, APROP_JumpZ, max(i, 0));
        
        //if (isDead(0)) { endloop = 1; }
        
        Delay(1);

        opcount = pcount;
        pcount  = PlayerCount();
    }

}

script 677 ENTER
{
    delay(GetCvar("samsara_superturboturkeyfrequency"));
    if (GetCvar("samsara_superturboturkeypuncher3000") > 0)
    { GiveInventory("ChickenModeOn",1);
    
        if (GetCvar("samsara_superturboturkeypuncher3000") == 3)
        { str ChickenClass = "SuperFriendlyChicken";
        if (!random(0, 15))
        { ChickenClass = "AssaultFriendlyChicken";
        if (!random(0, 2))
        ChickenClass = "ExplosiveFriendlyChicken"; }}
    
        if (GetCvar("samsara_superturboturkeypuncher3000") == 2)
        { ChickenClass = "SuperDMChicken";
        if (!random(0, 15))
        { ChickenClass = "AssaultChicken";
        if (!random(0, 2))
        ChickenClass = "ExplosiveChicken"; }}
        
        if (GetCvar("samsara_superturboturkeypuncher3000") == 1)
        { ChickenClass = "SuperCoopChicken";
        if (!random(0, 15))
        { ChickenClass = "AssaultChicken";
        if (!random(0, 2))
        ChickenClass = "ExplosiveChicken"; }}
     
        do
        {
           delay(1);
           int x = GetActorX(0) + random(-128.0, 128.0);
           int y = GetActorY(0) + random(-128.0, 128.0);
           int z = GetActorCeilingZ(0) - 38.0;
           int angle = GetActorAngle(0) >> 8;
        }
        until (Spawn(ChickenClass, x, y, z, 0, angle));
        
        
        if (GetCvar("samsara_superturboturkeylimit") > 0)
        {
            if(CheckInventory("ChickenKillCount") >= (GetCvar("samsara_superturboturkeylimit")))
            { SetFont("BIGFONT");
            HudMessageBold(n:0,s:" \cjhit the kill limit!";
            HUDMSG_PLAIN, 0, CR_WHITE, 0.5, 0.2, 5.0);
            if(CheckInventory("ChickenKillCount") < (GetCvar("samsara_superturboturkeylimit")))
            { TakeInventory("ChickenKillCount",999); }
            Delay(35*5);
            ConsoleCommand("nextmap");
            }       
        }
        
    }
    else
    { TakeInventory("ChickenModeOn",999); }
    restart;
    //}
}

/*script 678 (int respawning)
{
    int punchdrunkloop;
    //punchdrunkloop = 0;
    
    print(s:"678 is being called");
    if (!respawning)
    {
        if (!punchdrunkloop)
        { delay(15);
          print(s:"punchdrunkloop 0 is being called");
          punchdrunkloop = 1; }
        else
        { HandlePunchdrunk(respawning);
        print(s:"handlepunchdrunk is being called");
        HandleInstagib(respawning); }
    }
    delay(1);
    restart;
}*/

script SAMSARA_SYNTHFIRE (int startTime)
{
    int pln = PlayerNumber();

    while (ServerEnterTimes[pln] == startTime)
    {
        if (CheckInventory("Health") > 0)
        {
        if (keyDown(BT_ATTACK)) { GiveInventory("SynthFireLeft", 1); }
        else { TakeInventory("SynthFireLeft", 0x7FFFFFFF); }
        
        if (keyDown(BT_ALTATTACK)) { GiveInventory("SynthFireRight", 1); }
        else { TakeInventory("SynthFireRight", 0x7FFFFFFF); }
        }

        Delay(1);
    }

    TakeInventory("SynthFireLeft", 0x7FFFFFFF);
    TakeInventory("SynthFireRight", 0x7FFFFFFF);
}

script SAMSARA_CONFIRMCLASS (int which) { SetResultValue(SamsaraWepType == which); }

script SAMSARA_WOLFMOVE (void)
{ 
    int pln = PlayerNumber();
    int realspeed = GetActorProperty(0, APROP_Speed);
    int forward, side, angle, xthrust, ythrust;
    int forwardx, forwardy, sidex, sidey;
    int velx, vely;
    int moving;
    int fired;
    int startTime = Timer();

    WolfenEnterTimes[pln] = startTime;

    while (WolfenEnterTimes[pln] == startTime)
    {
        if (UnloadingNow)
        {
            if (GetActorProperty(0, APROP_Speed) == 0)
            {
                SetActorProperty(0, APROP_Speed, realspeed);
            }
            break;
        }

        if (!CheckInventory("CanWolfMovement")) { break; }
        if (!CheckInventory("WolfenMovement") || GetCVar("samsara_banwolfmove"))
        {
            if (GetActorProperty(0, APROP_Speed) == 0)
            {
                SetActorProperty(0, APROP_Speed, realspeed);
            }

            Delay(1);
            continue;
        }

        
        if (GetActorProperty(0, APROP_Health) < 1)
        {
            SetActorProperty(0, APROP_Speed, realspeed);
            velx = 0;
            vely = 0;
        }
        else
        {
            if (WolfenEnterTimes[pln] != startTime) { break; }
            SetActorProperty(0, APROP_Speed, 0);
            forward = keyDown(BT_FORWARD) - keyDown(BT_BACK);
            forward *= SPEED_FORWARD;
            
            side    = keyDown(BT_MOVERIGHT) - keyDown(BT_MOVELEFT);
            side    *= SPEED_SIDE;
            
            angle   = GetActorAngle(0);
            
            forwardx = cos(angle) * forward;
            forwardy = sin(angle) * forward;
            
            sidex = cos(angle-0.25) * side;
            sidey = sin(angle-0.25) * side;
            
            velx = forwardx + sidex;
            vely = forwardy + sidey;
        }

        if (keyDown(BT_CROUCH)) { velx /= 2; vely /= 2; }
        
        if ((velx != 0) || (vely != 0))
        {
            GiveInventory("WolfMoving", 1);
        }
        else if (CheckInventory("WolfMoving"))
        {
            TakeInventory("WolfMoving", 0x7FFFFFFF);
        }
        
        SetActorVelocity(0, velx, vely, GetActorVelZ(0), 0, 0);
        Delay(1);
    }

    if (GetActorProperty(0, APROP_Speed) == 0)
    {
        SetActorProperty(0, APROP_Speed, realspeed);
    }
}

script SAMSARA_ENTER_CLIENT (void) clientside
{
    int execInt, oExecInt, execStr;
    int class, oClass;
    int pln = PlayerNumber();
    int i, j;
    int startTime = Timer();
    
    // Comment out this line for zdoom
    int cpln = ConsolePlayerNumber();

    ClientEnterTimes[pln] = startTime;
    ClientTipboxes[pln] = 0;
    
    execInt = 0; oExecInt = 0;
    
    if (GetCVar("samsara_cl_exists") != SAMSARA_CL_VERSION)
    {
        ConsoleCommand(StrParam(s:"set samsara_cl_exists ", d:SAMSARA_CL_VERSION));
        ConsoleCommand("archivecvar samsara_cl_exists");
	
        if (!GetCVar("samsara_cl_wolfmove"))
        {   ConsoleCommand("set samsara_cl_wolfmove 0");
        ConsoleCommand("archivecvar samsara_cl_wolfmove"); }
        
        if (!GetCVar("samsara_cl_vanilladoom"))
        {   ConsoleCommand("set samsara_cl_vanilladoom 0");
        ConsoleCommand("archivecvar samsara_cl_vanilladoom"); }
        
        if (!GetCVar("samsara_cl_weaponhud"))
        {   ConsoleCommand("set samsara_cl_weaponhud 1");
        ConsoleCommand("archivecvar samsara_cl_weaponhud"); }
        
        if (!GetCVar("samsara_cl_ballgag"))
        {   ConsoleCommand("set samsara_cl_ballgag 0");
        ConsoleCommand("archivecvar samsara_cl_ballgag"); }
        
        if (!GetCVar("samsara_cl_moremessages"))
        {   ConsoleCommand("set samsara_cl_moremessages 0");
        ConsoleCommand("archivecvar samsara_cl_moremessages"); }
        
        if (!GetCVar("samsara_cl_pickupmode"))
        {   ConsoleCommand("set samsara_cl_pickupmode 1");
        ConsoleCommand("archivecvar samsara_cl_pickupmode"); }
        
        if (!GetCVar("samsara_cl_printpickup"))
        {   ConsoleCommand("set samsara_cl_printpickup 0");
        ConsoleCommand("archivecvar samsara_cl_printpickup"); }
    }
        
        if (!GetCVar("samsara_cl_bloodyhell"))
        {   ConsoleCommand("set samsara_cl_bloodyhell 0");
        ConsoleCommand("archivecvar samsara_cl_bloodyhell"); }
        
        if (!GetCVar("samsara_cl_bloodypersistent"))
        {   ConsoleCommand("set samsara_cl_bloodypersistent 0");
        ConsoleCommand("archivecvar samsara_cl_bloodypersistent"); }

    for (i = 0; i < RESCOUNT; i++)
    {
        ACS_ExecuteAlways(SAMSARA_RESONATE, 0, i, 16, 0);
    }
    
    DukeQuoteCooldown[pln] = 0; 

    //Log(s:"Client ", n:pln+1, s:"\c- (", d:pln, s:") has spawned (startTime is ", d:startTime, s:")");
    
    while (ClientEnterTimes[pln] == startTime)
    {
        oClass = class;
        class  = samsaraClassNum();
        DukeQuoteCooldown[pln] = max(0, DukeQuoteCooldown[pln]-1); 

        // Also this line
        if (cpln != pln) { Delay(1); continue; }
        
        SamsaraClientClass = class+1;
        
        if (oClass != class) { SamsaraItemFlash = Timer(); }
        
        for (i = 0; i < SLOTCOUNT; i++)
        {
            j = SamsaraClientWeps[i];
            SamsaraClientWeps[i] = HasClassWeapon(class, i);
            
            if (j != SamsaraClientWeps[i]) { SamsaraClientWepFlashes[i] = Timer(); }
        }
        
        if (IsServer)
        {
            array_wolfmove[pln]     = !!GetCVar("samsara_cl_wolfmove");
            array_vanillaAnim[pln]  = !!GetCVar("samsara_cl_vanilladoom");
            array_ballgag[pln]      = !!GetCVar("samsara_cl_ballgag");
            array_weaponBar[pln]    = !!GetCVar("samsara_cl_weaponhud");
            array_pickupswitch[pln] = !!GetCVar("switchonpickup");
        }
        else
        {
            oExecInt = execInt;
            execInt = SamsaraClientVars();
            
            if (execInt != oExecInt)
            {
                execStr = StrParam(s:"puke -", d:SAMSARA_PUKE, s:" ", d:execInt, s:" ", d:pln);
                ConsoleCommand(execStr);
            }
        }

        Delay(1);
    }
    //Log(s:"Client ", n:pln+1, s:"\c- (", d:pln, s:") has left spawn (", d:startTime, s:" vs. ", d:ClientEnterTimes[pln], s:")");
}

script SAMSARA_DISCONNECT_CLIENT (int pln) disconnect clientside
{
    // Comment out these lines for zdoom
    int cpln = ConsolePlayerNumber();
    if (cpln != pln) { terminate; }
    
    SamsaraClientClass  = 0;
    SamsaraItemFlash    = Timer();
}

script SAMSARA_PUKE (int values, int pln) net
{
    array_wolfmove[pln]     = values & 1;
    array_vanillaAnim[pln]  = values & 2;
    array_ballgag[pln]      = values & 4;
    array_weaponBar[pln]    = values & 8;
    array_pickupswitch[pln] = values & 16;
}


script SAMSARA_DEATH death
{
    Delay(1);
    TakeInventory("QuadDamagePower", 1);
}
