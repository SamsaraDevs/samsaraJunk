script SAMSARA_OPEN open
{
    IsServer = 1;
    
    if (CommandBitchingDone == 0)
    {
        if (isSinglePlayer())
        {
            HudMessage(s:HELPSTR;
            HUDMSG_PLAIN|HUDMSG_LOG, 92712, CR_WHITE, 1.5, 0.2, 2.5);
            HudMessage(s:"See the console for \caJAZZ\c-.";
            HUDMSG_FADEOUT, 92712, CR_WHITE, 1.5, 0.2, 2.0, 0.5);
        }
        else
        {
            Log(s:HELPSTR);
        }
        
        CommandBitchingDone = 1;
    }
    
    while (1)
    {
        // I'd use defaultCVar but best-ever breaks on it for some reason :/
        
        if (!GetCVar("samsara_banjetpack"))
        {   ConsoleCommand("set samsara_banjetpack 0");
        ConsoleCommand("archivecvar samsara_banjetpack"); }
        
        if (!GetCVar("samsara_lmslife"))
        {   ConsoleCommand("set samsara_lmslife 0");
        ConsoleCommand("archivecvar samsara_lmslife"); }
        
        if (!GetCVar("samsara_lmsunique"))
        {   ConsoleCommand("set samsara_lmsunique 0");
        ConsoleCommand("archivecvar samsara_lmsunique"); }
        
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
        
        if (!GetCVar("compat_clientssendfullbuttoninfo"))
        {   ConsoleCommand("set compat_clientssendfullbuttoninfo 1");
        ConsoleCommand("archivecvar compat_clientssendfullbuttoninfo"); }
        
        Delay(1);
    }
}

script SAMSARA_ENTER enter { ACS_ExecuteWithResult(SAMSARA_SPAWN, 0,0,0); }
script SAMSARA_RETURN return { UnloadingNow = 0; ACS_ExecuteWithResult(SAMSARA_SPAWN, 0,0,0); }
script SAMSARA_RESPAWN respawn { ACS_ExecuteWithResult(SAMSARA_SPAWN, 1,0,0); }

script SAMSARA_SPAWN (int respawning)
{
    int pln = PlayerNumber();
    int quadTimer, oQuadTimer;
    int regenTimer, oRegenTimer;
    int health, regenPulse, oPulse;
    int regenX, regenY;
    int healthGiven;
    int pcount, opcount;
    int startTime = Timer();
    int endloop;
    int canbuddha;
    int i;

    ACS_ExecuteAlways(SAMSARA_ENTER_CLIENT, 0, 0,0,0);
    ACS_ExecuteAlways(SAMSARA_WOLFMOVE, 0, 0,0,0);

    ServerEnterTimes[pln] = startTime;
    
    if (isLMS()) { ApplyLMS(); }
    if (isSinglePlayer()) { SamsaraWepType = samsaraClassNum()+1; }
    if (!respawning) { ClientTipboxes[pln] = 0; }

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
        health = GetActorProperty(0, APROP_Health);

        if (keyDown(BT_ATTACK)) { GiveInventory("SynthFireLeft", 1); }
        else { TakeInventory("SynthFireLeft", 0x7FFFFFFF); }
        
        if (keyDown(BT_ALTATTACK)) { GiveInventory("SynthFireRight", 1); }
        else { TakeInventory("SynthFireRight", 0x7FFFFFFF); }
        
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
        
        TakeInventory("WeaponGetYaaaay",  1);
        TakeInventory("WeaponGetYaaaay2", 1);
        TakeInventory("Mace", 1);
        TakeInventory("MacePowered", 1);
        ConvertClassWeapons(-1);

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

        // useless but harmless in Zandro - triggers buddha mode when Blazko has an extra life in ZDoom
        if (CheckInventory("WolfExtraLife") && !canbuddha) { SetPlayerProperty(0, 1, 16); }
        if (!CheckInventory("WolfExtraLife") && canbuddha) { SetPlayerProperty(0, 0, 16); }
        canbuddha = CheckInventory("WolfExtraLife");

        // Quakeguy quad shit
        oQuadTimer = quadTimer;
        quadTimer = CheckInventory("QuakeQuadTimer") - QUAD_THRESHOLD;

        if (quadTimer - 35 > oQuadTimer)
        {
            AmbientSound("quakeweps/quadon", 127);
        }

        if (quadTimer > 0)
        {
            if (quadTimer % 35 == 0)
            {
                SetHudSize(640, 480, 1);
                SetFont("QUADICO2");
                HudMessage(s:"A"; HUDMSG_FADEOUT, 58101, CR_UNTRANSLATED, 610.4, 380.0, 1.5, 1.0);
                SetHudSize(320, 240, 1);
                SetFont("QUA3HUDF");
                HudMessage(d:quadTimer / 35;  HUDMSG_FADEOUT | HUDMSG_COLORSTRING, 58102, "QuakeBrick", 295.2, 190.0, 1.5, 1.0);
            }
            GiveInventory("QuadDamagePower", 1);
        }
        else
        {
            if (quadTimer == 0)
            {
                HudMessage(s:""; HUDMSG_PLAIN, 58101, CR_UNTRANSLATED, 0, 0, 1);
                HudMessage(s:""; HUDMSG_PLAIN, 58102, CR_UNTRANSLATED, 0, 0, 1);
            }
            TakeInventory("QuadDamagePower", 1);
        }

        if (quadTimer == 105)
        {
            ActivatorSound("quakeweps/quadoff", 127);
        }

        if ((quadTimer % 35 == 0) && (quadTimer / 35 <= 3) && (quadTimer > 0))
        {
            FadeRange(0, 64, 255, 0.25, 0, 64, 255, 0, 0.33);
        }

        if (quadTimer == -QUAD_THRESHOLD)
        {
            if (CheckInventory("CantQuad") && CheckInventory("QuadDamageItem") && !UnloadingNow)
            {
                ActivatorSound("quakeweps/quadready", 96);
                SetHudSize(240, 180, 1);
                SetFont("QUADICO2");
                HudMessage(s:"A"; HUDMSG_FADEOUT, 58103, CR_UNTRANSLATED, 215.4, 142.0, 0.0, 1.0);
            }

            TakeInventory("CantQuad", 0x7FFFFFFF);
        }
        else
        {
            GiveInventory("CantQuad", 1);
        }

        TakeInventory("QuakeQuadTimer", 1);

        // End quad shit
        
        // Quakeguy regen shit
        oRegenTimer = regenTimer;
        regenTimer =  CheckInventory("QuakeRegenTimer");

        if (regenTimer != 0)
        {
            if (regenTimer - 35 > oRegenTimer) { AmbientSound("quakeweps/regenannounce", 127); }

            regenX = 640 - (regenPulse * 18);
            regenY = 480 - (regenPulse * 18);

            if (regenTimer % 35 == 0 || regenPulse != 0 || oPulse != 0)
            {
                SetHudSize(regenX, regenY, 1);
                regenX = ftoi(regenX * REGEN_CENTER_X);
                regenY = ftoi(regenY * REGEN_CENTER_Y);

                SetFont("REGENICO");
                HudMessage(s:"A"; HUDMSG_FADEOUT, 58103, CR_UNTRANSLATED, itof(regenX) + 0.4, itof(regenY), 1.5, 1.0);
                SetHudSize(320, 240, 1);
                SetFont("QUA3HUDF");
                HudMessage(d:regenTimer / 35;  HUDMSG_FADEOUT | HUDMSG_COLORSTRING, 58104, "QuakeBrick", 295.2, 165.0, 1.5, 1.0);
            }

            oPulse = regenPulse;
            regenPulse = max(0, regenPulse - 1);

            if (regenTimer % 35 == 18)
            {
                if (health >= getMaxHealth()) { giveHealthMax(5, 250); }
                else if (health + 10 >= getMaxHealth())
                {
                    SetActorProperty(0, APROP_Health, getMaxHealth());
                    giveHealthMax(5, 250);
                }
                else { giveHealthMax(15, 250); }

                if (GetActorProperty(0, APROP_Health) > health)
                {
                    FadeRange(255, 0, 0, 0.2, 255, 0, 0, 0.0, 0.3333);
                    ActivatorSound("quakeweps/regen", 127);
                    regenPulse = 12;
                }

                healthGiven += max(GetActorProperty(0, APROP_Health) - health, 0);
                health = GetActorProperty(0, APROP_Health);
            }

            if (regenTimer % 35 == 0 && regenTimer / 35 < 5)
            {
                ActivatorSound("quakeweps/regenout", PowerOutVols[regenTimer / 35]);
            }
        }
        else if (CheckInventory("RuneProsperity"))
        {
            regenTimer = 0;
        }
        else
        {
            if (oRegenTimer != 0)
            {
                HudMessage(s:""; HUDMSG_PLAIN, 58103, CR_UNTRANSLATED, 0, 0, 1);
                HudMessage(s:""; HUDMSG_PLAIN, 58104, CR_UNTRANSLATED, 0, 0, 1);
            }

            if (health < getMaxHealth()) { healthGiven = 0; }

            if (Timer() % 35 == 0 && healthGiven > 0 && (health - 1 >= getMaxHealth()))
            {
                SetActorProperty(0, APROP_Health, health - 1);
            }
        }

        TakeInventory("QuakeRegenTimer", 1);
        
            
        switch (samsaraClassNum())
        {
          case CLASS_MARATHON:
            if (GetCVar("samsara_nocustomgravity")) { SetActorProperty(0, APROP_Gravity, 1.0); }
            else { SetActorProperty(0, APROP_Gravity, 0.15); }
            break;

          case CLASS_QUAKE:
            if (GetCVar("samsara_nocustomgravity")) { SetActorProperty(0, APROP_Gravity, 1.0); }
            else { SetActorProperty(0, APROP_Gravity, 0.75); }

            if (UnloadingNow)
            {
                SetActorProperty(0, APROP_Health, middle(health, getMaxHealth(), health - healthGiven));
                health = GetActorProperty(0, APROP_Health);
                SetActorProperty(0, APROP_Health, max(health, getMaxHealth() / 2));
                endloop = 1;
            }
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
        
        if (isDead(0)) { endloop = 1; }
        
        Delay(1);

        opcount = pcount;
        pcount  = PlayerCount();

        /*
        if (isLMS() && (pcount == 1) && (opcount > pcount))
        {
            SetHudSize(800, 600, 1);
            SetFont("BIGFONT");
            HudMessageBold(s:"Winning stats of ", n:0;
                    HUDMSG_FADEOUT, 91028, CR_GOLD, 400.4, 350.1, 3.0, 2.0);
            
            SetFont("SMALLFONT");
            HudMessageBold(s:"+ \ca", d:GetActorProperty(0, APROP_Health);
                    HUDMSG_FADEOUT, 91027, CR_RED, 370.2, 370.1, 3.0, 2.0);
            HudMessageBold(d:CheckInventory("BasicArmor"), s:" \cqA";
                    HUDMSG_FADEOUT, 91029, CR_RED, 430.1, 370.1, 3.0, 2.0);

            HudMessage(s:""; HUDMSG_PLAIN, 91027, 0, 0, 0, 0);
            HudMessage(s:""; HUDMSG_PLAIN, 91028, 0, 0, 0, 0);
            HudMessage(s:""; HUDMSG_PLAIN, 91029, 0, 0, 0, 0);
        }
        */
    }

    quadTimer = CheckInventory("QuakeQuadTimer"); 
    TakeInventory("QuakeQuadTimer", quadTimer - QUAD_THRESHOLD);

    TakeInventory("SynthFireLeft", 0x7FFFFFFF);
    TakeInventory("SynthFireRight", 0x7FFFFFFF);
}

script SAMSARA_CONFIRMCLASS (int which) { SetResultValue(SamsaraWepType == which); }

script SAMSARA_WOLFMOVE enter
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
            SetActorProperty(0, APROP_Speed, realspeed);
            break;
        }

        if (!(CheckInventory("CanWolfMovement") && CheckInventory("WolfenMovement")) )
        {
            SetActorProperty(0, APROP_Speed, realspeed);
            Delay(1);
            continue;
        }
        
        if (GetActorProperty(0, APROP_Health) < 1)
        {
            velx = 0;
            vely = 0;
        }
        else
        {
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
