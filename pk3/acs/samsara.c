#include "zcommon.acs"
#library "samsara"

#define DEBUG 0

#include "commonFuncs.h"

#include "samsaraDefs.h"
#include "samsaraWeps.h"
#include "samsaraMsgs.h"

int array_wolfmove[PLAYERMAX];
int array_vanillaAnim[PLAYERMAX];
int array_ballgag[PLAYERMAX];
int array_weaponBar[PLAYERMAX];
int array_pickupswitch[PLAYERMAX];
int DukeQuoteCooldown[PLAYERMAX];

int SamsaraWepType, SamsaraClientClass, SamsaraItemFlash;
int SamsaraClientWeps[SLOTCOUNT] = {0};
int SamsaraClientWepFlashes[SLOTCOUNT] = {0};
int IsServer = 0;
int LMSMessaged = 0;
int UnloadingNow = 0;

global int 0:CommandBitchingDone;

#include "samsaraFuncs.h"

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
        
        if (!GetCVar("samsara_hexenjump"))
        {   ConsoleCommand("set samsara_hexenjump 0");
        ConsoleCommand("archivecvar samsara_hexenjump"); }
        
        if (!GetCVar("samsara_sogravity"))
        {   ConsoleCommand("set samsara_sogravity 0");
        ConsoleCommand("archivecvar samsara_sogravity"); }
        
        if (!GetCVar("samsara_permault"))
        {   ConsoleCommand("set samsara_permault 0");
        ConsoleCommand("archivecvar samsara_permault"); }
        
        if (!GetCVar("compat_clientssendfullbuttoninfo"))
        {   ConsoleCommand("set compat_clientssendfullbuttoninfo 1");
        ConsoleCommand("archivecvar compat_clientssendfullbuttoninfo"); }
        
        Delay(1);
    }
}

script SAMSARA_ENTER enter { ACS_ExecuteWithResult(SAMSARA_SPAWN, 0,0,0); }
script SAMSARA_RESPAWN respawn { ACS_ExecuteWithResult(SAMSARA_SPAWN, 1,0,0); }

script SAMSARA_SPAWN (int respawning)
{
    int pln = PlayerNumber();
    int pariasMod;
    int quadTimer, oQuadTimer;
    int regenTimer, oRegenTimer;
    int health, regenPulse, oPulse;
    int regenX, regenY;
    int healthGiven;
    int pcount, opcount;
    
    if (DEBUG) { Print(s:"respawning is ", d:respawning); }
    
    if (isLMS()) { ApplyLMS(); }
    
    if (isSinglePlayer())
    {
        SamsaraWepType = samsaraClassNum()+1;
    }

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
    
    while (1)
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

        if (GetCVar("sv_bfgfreeaim")) { TakeInventory("DoomNoBFGAim", 0x7FFFFFFF); }
        else { GiveInventory("DoomNoBFGAim", 1); }
        
        TakeInventory("WeaponGetYaaaay",  1);
        TakeInventory("WeaponGetYaaaay2", 1);
        
        TakeInventory("Mace", 1);
        TakeInventory("MacePowered", 1);

        if (GetCVar("samsara_banjetpack") && CheckInventory("DukePortJetpack"))
        {
            GiveInventory("DukeBootserk", 1);
            TakeInventory("DukePortJetpack", 0x7FFFFFFF);
            TakeInventory("DukeJetpackFuel", 0x7FFFFFFF);
            Print(s:"The jetpack is banned on this server. Have 8x boot damage instead.");
        }


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
                HudMessage(d:quadTimer / 35;  HUDMSG_FADEOUT, 58102, CR_UNTRANSLATED, 295.2, 190.0, 1.5, 1.0);
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
            if (CheckInventory("CantQuad") && !UnloadingNow)
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

        if (DEBUG) { Print(d:oQuadTimer, s:" -> ", d:quadTimer); }
        // End quad shit
        
        // Quakeguy regen shit
        oRegenTimer = regenTimer;
        regenTimer =  CheckInventory("QuakeRegenTimer");

        if (regenTimer == 0)
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
        else
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
                HudMessage(d:regenTimer / 35;  HUDMSG_FADEOUT, 58104, CR_UNTRANSLATED, 295.2, 165.0, 1.5, 1.0);
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

        TakeInventory("QuakeRegenTimer", 1);
        
            
        if (SamsaraClassNum() == CLASS_MARATHON)
        {
            if (GetCVar("samsara_sogravity")) { SetActorProperty(0, APROP_Gravity, 1.0); }
            else { SetActorProperty(0, APROP_Gravity, 0.15); }
        }

        pariasMod = 9 * (SamsaraClassNum() == CLASS_HEXEN);

        if (GetCVar("samsara_hexenjump"))
        {
            SetActorProperty(0, APROP_JumpZ, JumpZFromHeight(41 + pariasMod, GetActorProperty(0, APROP_Gravity)));
        }
        else
        {
            SetActorProperty(0, APROP_JumpZ, JumpZFromHeight(32 + pariasMod, GetActorProperty(0, APROP_Gravity)));
        }

        if (UnloadingNow && samsaraClassNum() == CLASS_QUAKE)
        {
            SetActorProperty(0, APROP_Health, max(50, health - healthGiven));
            break;
        }
        
        Delay(1);
        
        if (isDead(0)) { break; }

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

    TakeInventory("QuakeQuadTimer", 0x7FFFFFFF);
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
    
    
    while (PlayerInGame(pln))
    {
        if (!(CheckInventory("WolfenClass") && CheckInventory("WolfenMovement")) )
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

script SAMSARA_PUKE (int values, int pln) net
{
    if (DEBUG)
    {
        PrintBold(s:"Player \"", n:pln+1, s:"\c-\" puked script ", d:SAMSARA_PUKE, s:" (", d:values, s:", ", d:pln, s:")");
    }
    
    array_wolfmove[pln]     = values & 1;
    array_vanillaAnim[pln]  = values & 2;
    array_ballgag[pln]      = values & 4;
    array_weaponBar[pln]    = values & 8;
    array_pickupswitch[pln] = values & 16;
}

script SAMSARA_OPEN_CLIENT open clientside
{
    int i;
    if (!DEBUG) { terminate; }
    
    SetHudSize(1280, 1024, 1);
    PrintBold(s:"OPEN CLIENTSIDE running now");
    
    while (1)
    {
        for (i = 0; i < PLAYERMAX; i++)
        {
            HudMessageBold(s:"Cooldown (\cd", d:i, s:"\c-): \ck", d:DukeQuoteCooldown[i];
            HUDMSG_FADEOUT, 8271+i, CR_LIGHTBLUE, 40.1, 20.0 + (8.0 * i), 2.0, 0.5);
        }
        
        Delay(1);
    }
}

script SAMSARA_ENTER_CLIENT enter clientside
{
    int execInt, oExecInt, execStr;
    int class, oClass;
    int pln = PlayerNumber();
    int i, j;
    
    // Comment out these lines for zdoom
    int cpln = ConsolePlayerNumber();
    if (cpln != pln) { terminate; }
    
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
    
    class = samsaraClassNum() + 1;
    
    DukeQuoteCooldown[pln] = 0; 
    
    while (PlayerInGame(pln))
    {
        oClass = class;
        class  = samsaraClassNum();
        DukeQuoteCooldown[pln] = max(0, DukeQuoteCooldown[pln]-1); 
        
        SamsaraClientClass = class+1;
        
        if (oClass != class) { SamsaraItemFlash = Timer(); }
        
        for (i = 0; i < SLOTCOUNT; i++)
        {
            j = SamsaraClientWeps[i];
            SamsaraClientWeps[i] = HasClassWeapon(class, i);
            
            if (j != SamsaraClientWeps[i]) { SamsaraClientWepFlashes[i] = Timer(); }
        }
        
        if (isSinglePlayer())
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
                if (DEBUG) { Print(s:execStr); }
                ConsoleCommand(execStr);
            }
        }
        
        Delay(1);
    }
}

script SAMSARA_DISCONNECT_CLIENT (int pln) disconnect clientside
{
    // Comment out these lines for zdoom
    int cpln = ConsolePlayerNumber();
    if (cpln != pln) { terminate; }
    
    SamsaraClientClass  = 0;
    SamsaraItemFlash    = Timer();
}

script SAMSARA_CLIENT_CLASS (int slot) clientside
{
    int toClass = SamsaraClientClass-1;
    int displaymode = GetCVar("samsara_cl_pickupmode");
    int oldslot = slot;
    slot = itemToSlot(slot);
    int hasSlot = SamsaraClientWeps[slot];
    
    //PrintBold(s:"Has weapon \"", s:ClassWeapons[toClass][slot][S_WEP], s:"\": ", d:hasSlot);
    
    if (displaymode != 0)
    {
        if ((SamsaraItemFlash >= (Timer() - 35)) && (Timer() >= 35))
        {
            Spawn("SamsaraChangeFlash", GetActorX(0), GetActorY(0), GetActorZ(0));
        }
        
        if (toClass == -1)
        {
            SetActorState(0, "NoGuy");
            terminate;
        }
    }
    
    if (slot == -1)
    {
        switch (displaymode)
        {
          case 0:
            SetActorState(0, "NoGuy");
            break;
            
          case 1:
            SetActorState(0, PickupStates[toClass][3]);
            break;
            
          case 2:
            SetActorState(0, PickupStates[toClass][0]);
            break;
        }
        
        terminate;
    }
    
    switch (displaymode)
    {
      case 0:
        SetActorState(0, "NoGuy");
        break;
        
      case 1:
        if ((SamsaraClientWepFlashes[slot] >= (Timer() - 35)) && (Timer() >= 35))
        {
            Spawn("SamsaraChangeFlash2", GetActorX(0), GetActorY(0), GetActorZ(0));
        }
        
        if (hasSlot) { SetActorState(0, PickupStates[toClass][1]); }
        else         { SetActorState(0, PickupStates[toClass][2]); }
        break;
        
      case 2:
        SetActorState(0, PickupStates[toClass][0]);
        break;
    }
}

script SAMSARA_DECORATE (int choice, int arg1, int arg2)
{
    int result;
    
    switch (choice)
    {
      case 1:
        result = GetActorProperty(0, APROP_Dropped);
        break;
        
      case 2:
        if (CheckInventory("WolfenMovement") == 1) { SetActorState(0, "Spawn"); }
        break;
        
      case 3:
        result = !(GetCVar("sv_itemrespawn") || GetCVar("sv_weaponstay"));
        break;
        
      case 4:
        result = isInvasion() || !(isCoop() || isSinglePlayer());

        if (DEBUG)
        {
            Log(d:isInvasion(), s:"     ", d:isCoop(), s:"     ", d:isSinglePlayer());
            Log(d:result);
        }
        break;

      case 5:
        SetActivatorToTarget(0);
        result = CheckInventory("Cell");
        if (arg1) { TakeInventory("Cell", result); }
        break;
    }
    
    SetResultValue(result);
}

/*
*      WARNING
*  This script is over 20 variables. Check here first for segfaults.
*  Also, keep an eye on this script for potential desync issues.
*  (current count: 25)
*/

script SAMSARA_GIVEWEAPON (int slot, int dropped, int silent)
{
    slot = itemToSlot(slot);
    
    if (DEBUG) { Print(s:"running on server tic ", d:Timer(), s:", cpln = ", d:ConsolePlayerNumber()); }
    
    int weaponStay = !!GetCVar("sv_weaponstay");
    int weaponGet  = 0;
    int pclass = samsaraClassNum();
    int hasWep = HasClassWeapon(pclass, slot);
    
    if (slot == SLOTCOUNT-1) { weaponStay &= !!GetCVar("samsara_permault"); }
    
    int a1cnt  = 0, a2cnt = 0;
    int a1max  = 0, a2max = 0;
    int a1max2 = 0, a2max2 = 0;
    int a1Full = 0, a2Full = 0;
    int a1diff = 0, a2diff = 0;
    
    int weapon  = ClassWeapons[pclass][slot][S_WEP],        wepbool = !!StrLen(weapon); 
    int ammo1   = ClassWeapons[pclass][slot][S_AMMO1],      a1bool  = !!StrLen(ammo1);
    int ammo2   = ClassWeapons[pclass][slot][S_AMMO2],      a2bool  = !!StrLen(ammo2);
    int check   = ClassWeapons[pclass][slot][S_CHECKITEM],  chkbool = !!StrLen(check);
    
    if (!wepbool || (CheckInventory(ClassWeapons[pclass][slot][S_CHECKFAILITEM] && !dropped)))
    {
        SetResultValue(weaponStay * WEPFLAGS_WEAPONSTAY);
        terminate;
    }
    
    if (a1bool)
    {
        a1cnt   = CheckInventory(ammo1);
        a1max   = GetAmmoCapacity(ammo1);
        a1max2  = a1max * 4; // ya never know
    }
    
    if (a2bool)
    {
        a2cnt   = CheckInventory(ammo2);
        a2max   = GetAmmoCapacity(ammo2);
        a2max2  = a1max * 4;
    }
    
    if (a1Bool) { a1Full = (CheckInventory(ammo1) == a1max); }
    if (a2Bool) { a2Full = (CheckInventory(ammo2) == a2max); }
    
    if (dropped && IsServer)
    {
        if (a1bool) { SetAmmoCapacity(ammo1, a1max2); }
        if (a2bool) { SetAmmoCapacity(ammo2, a2max2); }
    }
    
    // do we even have this?
    if (!hasWep || (chkbool && !CheckInventory(check)))
    {
        weaponGet = 1;
    }
    else if (!weaponStay || dropped)     // does this not stay on the ground?
    {
        if ((a1bool && !a1Full) || (a2Bool && !a2Full)) { weaponGet = 1; }
    }

    if (DEBUG)
    {
        Print(s:"hasWep -> ", d:hasWep, s:"; hasCheckItem -> ", d:CheckInventory(ClassWeapons[pclass][slot][S_CHECKITEM]),
            s:"\nweaponGet -> ", d:weaponGet);
    }
    
    if (weaponGet && IsServer)
    {
        Spawn("WeaponGetYaaaay", GetActorX(0), GetActorY(0), GetActorZ(0));
        Spawn("WeaponGetYaaaay2", GetActorX(0), GetActorY(0), GetActorZ(0));

        int success = !_giveclassweapon(pclass, slot, 3, dropped);

        if (!silent && success)
        {
            ACS_ExecuteAlways(SAMSARA_CLIENT_WEAPONPICKUP, 0, slot,GetCVar("compat_silentpickup"),0);
        }
    }
    
    if (dropped && IsServer)
    {
        TakeInventory(ammo1, (CheckInventory(ammo1) - a1cnt) / 2);
        TakeInventory(ammo2, (CheckInventory(ammo2) - a2cnt) / 2);
        if (a1bool) { SetAmmoCapacity(ammo1, a1max); }
        if (a2bool) { SetAmmoCapacity(ammo2, a2max); }
    }
    
    if (IsServer)
    {
        TakeInventory(ammo1, CheckInventory(ammo1) - a1max);
        TakeInventory(ammo2, CheckInventory(ammo2) - a2max);
    }
    
    SetResultValue((weaponStay * WEPFLAGS_WEAPONSTAY) + (success * WEPFLAGS_GOTWEAPON));
}

script SAMSARA_GIVEUNIQUE (int alt)
{
    if (DEBUG) { Print(s:"running on server tic ", d:Timer(), s:", cpln = ", d:ConsolePlayerNumber()); }
    
    int uniqueGet  = 0;
    int pclass = samsaraClassNum();
    
    int ammo, abool;
    int acnt  = 0;
    int amax  = 0;
    int umax  = 0;
    int aFull = 0;
    int hasWep;
    int unique, unbool;
    
    if (alt)
    {
        unique = ClassUniques[pclass][U_UNIQUE2];
        ammo   = ClassUniques[pclass][U_AMMO2];
        umax   = UniqueMaxes[pclass][U_UNIQUE2];
        amax   = UniqueMaxes[pclass][U_AMMO2];
    }
    else
    {
        unique = ClassUniques[pclass][U_UNIQUE1];
        ammo   = ClassUniques[pclass][U_AMMO1];
        umax   = UniqueMaxes[pclass][U_UNIQUE1];
        amax   = UniqueMaxes[pclass][U_AMMO1];
    }

    unbool = !!StrLen(unique);
    abool  = !!StrLen(ammo);
    
    if (!unbool)
    {
        if (DEBUG) { Print(s:"unbool is false (class = ", d:pclass, s:", alt = ", d:alt, s:")"); }
        SetResultValue(0);
        if (alt) { SetResultValue(ACS_ExecuteWithResult(SAMSARA_GIVEUNIQUE, 0,0,0,0)); }
        terminate;
    }
    
    if (aBool) { aFull = CheckInventory(ammo) >= amax; }
    
    hasWep = (CheckInventory(unique) >= umax) && (umax != 0);
    
    if (!hasWep || (abool && !aFull)) { uniqueGet = 1; }
    
    if (uniqueGet && IsServer)
    {
        GiveClassUnique(pclass, alt);
        ACS_ExecuteAlways(SAMSARA_CLIENT_UNIQUEPICKUP, 0, GetCVar("compat_silentpickup"), 0, 0);
    }
    
    SetResultValue(uniqueGet);
}

int QuoteStorage[MSGCOUNT];

script SAMSARA_CLIENT_WEAPONPICKUP (int slot, int soundmode) clientside
{
    int pln = PlayerNumber(), cpln = ConsolePlayerNumber();
    int pclass = samsaraClassNum();
    int i, j, quoteCount = 0;
    int logMsg;
    
    if (DEBUG) { Print(s:"running on local tic ", d:Timer()); }
    
    if (cpln == pln)
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
        else { Log(s:logMsg); }
    }
    
    if (soundmode == 1) { LocalAmbientSound(ClassPickupSounds[pclass][slot], 127); }
    else { ActivatorSound(ClassPickupSounds[pclass][slot], 127); }
    
    if (DEBUG)
    {
        Print(d:ClassFades[pclass][0], s:", ", d:ClassFades[pclass][1], s:", ", d:ClassFades[pclass][2],
        s:" for ", d:ClassFades[pclass][4], s:" tics");
    }
    
    FadeRange(ClassFades[pclass][0], ClassFades[pclass][1], ClassFades[pclass][2], ClassFades[pclass][3],
    ClassFades[pclass][0], ClassFades[pclass][1], ClassFades[pclass][2], 0.0, itof(ClassFades[pclass][4]) / 35);
    
    if (pclass == CLASS_DUKE && !GetCVar("samsara_cl_ballgag"))
    {
        Delay(8);

        if (DEBUG) { Print(s:"cooldown is ", d:DukeQuoteCooldown[pln]); }
        
        if (!DukeQuoteCooldown[pln])
        {
            if (soundmode == 1) { LocalAmbientSound("duke/weapontaunt", 127); }
            else { ActivatorSound("duke/weapontaunt", 127); }
            DukeQuoteCooldown[pln] = 140;
        }
    }
}

script SAMSARA_CLIENT_UNIQUEPICKUP (int soundmode) clientside
{
    int pln = PlayerNumber(), cpln = ConsolePlayerNumber();
    int pclass = samsaraClassNum();
    int i, j, quoteCount = 0;
    int logMsg;
    
    if (DEBUG) { Print(s:"running on local tic ", d:Timer()); }
    
    if (cpln == pln)
    {
        if (GetCVar("samsara_cl_moremessages"))
        {
            for (i = 0; i < MSGCOUNT; i++)
            {
                j = ClassUniqueMessages[pclass][i];
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
        else { Log(s:logMsg); }
    }
    
    if (soundmode == 1) { LocalAmbientSound(ClassUniqueSounds[pclass], 127); }
    else { ActivatorSound(ClassUniqueSounds[pclass], 127); }
    
    if (DEBUG)
    {
        Print(d:ClassFades[pclass][0], s:", ", d:ClassFades[pclass][1], s:", ", d:ClassFades[pclass][2],
        s:" for ", d:ClassFades[pclass][4], s:" tics");
    }
    
    FadeRange(ClassFades[pclass][0], ClassFades[pclass][1], ClassFades[pclass][2], ClassFades[pclass][3],
    ClassFades[pclass][0], ClassFades[pclass][1], ClassFades[pclass][2], 0.0, itof(ClassFades[pclass][4]) / 35);
}

script SAMSARA_MARATHON (int class, int slot, int dropped)
{
    int giveboth    = isInvasion() || !(isCoop() || isSinglePlayer());
    int hasShotty   = CheckInventory("WSTE-M5 Combat Shotgun");
    int hasBoth     = CheckInventory("CanDualShotties");
    int limited     = CheckInventory("LevelLimiter");
    int limit       = GetCVar("sv_itemrespawn") || GetCVar("sv_weaponstay");
    int ammoFull    = CheckInventory("AmmoShell") >= (GetAmmoCapacity("AmmoShell") / ((dropped*3)+1));
    // The above line is because of the quadupling of ammo capacity with dropped pickups
    // It's a really gross hack. I hate it. But it works.
    
    if (DEBUG)
    {
        PrintBold(s:"\ca[MARATHON]\c- dropped is ", d:dropped,
                s:"\ngiveBoth, hasShotty, limited, limit, ammoFull = (",
                d:giveboth, s:", ", d:hasShotty, s:", ", d:hasBoth, s:", ",
                d:limited, s:", ", d:limit, s:", ", d:ammoFull, s:")");
    }
    
    switch (slot)
    {
      case 1:
        GiveInventory("CanDualPistols", 1);
        GiveInventory("KKV-7 SMG Flechette", 1);
        break;
        
      case 3:
        if (limited && !dropped)
        {
            SetResultValue(0);
            terminate;
        }

        if ((dropped && hasShotty && ammoFull) ||
            (!dropped && (hasBoth || (hasShotty && limited)) && ammoFull)
           )
        {
            SetResultValue(0);
            terminate;
        }
        
        GiveInventory("Shell", 8 / (!!dropped+1));
        GiveInventory("AmmoShell", 8 / (!!dropped+1));
        GiveInventory("WSTE-M5 Combat Shotgun", 1);
        
        if (giveboth || (hasShotty && !dropped))
        {
            GiveInventory("CanDualShotties", 1);
        }
        
        if (limit)
        {
            GiveInventory("LevelLimiter", 1);
        }
        break;
    }
}

script SAMSARA_MEGAHEALTH (int hpcount, int hpPerSec, int delayTics)
{
    int hpGiven = GetActorProperty(0, APROP_Health);
    int startHealth = hpGiven;
    SetActorProperty(0, APROP_Health, min(hpGiven + hpcount, 250));
    hpGiven = GetActorProperty(0, APROP_Health) - hpGiven;

    hpPerSec = 1.0 / (hpPerSec * 35);

    int takeCounter, hpToTake;

    while (1)
    {
        if (DEBUG) { Print(s:"hpGiven = ", d:hpGiven, s:", startHealth = ", d:startHealth); }

        if (UnloadingNow)
        {
            hpToTake = GetActorProperty(0, APROP_Health);

            if (DEBUG) { Print(d:hpToTake, s:", ", d:getMaxHealth(), s:", ", d:hpToTake - hpGiven); }

            hpToTake = middle(hpToTake, getMaxHealth(), hpToTake - hpGiven);
            SetActorProperty(0, APROP_Health, hpToTake);
            break;
        }

        if (delayTics > 0) { delayTics--; }
        else
        {
            takeCounter += hpPerSec;
            hpToTake = min(ftoi(takeCounter), hpGiven);
            hpGiven -= hpToTake;

            SetActorProperty(0, APROP_Health, GetActorProperty(0, APROP_Health) - hpToTake);
            takeCounter -= itof(hpToTake);
        }

        if (hpGiven <= 0) { break; }
        if (GetActorProperty(0, APROP_Health) <= startHealth ||
            GetActorProperty(0, APROP_Health) <= getMaxHealth()) { break; }
        Delay(1);
    }
}


// TIPBOX START

str CoolTips1[CLASSCOUNT] = {"DOOMTIP1", "CHEXTIP1", "HERETIP1", "WOLFTIP1", "HEXNTIP1", "DUKETIP1", "MARATIP1", "QUAKTIP1"};
//str CoolTips2[CLASSCOUNT] = {"DOOMTIP2", "CHEXTIP2", "HERETIP2", "WOLFTIP2", "HEXNTIP2", "DUKETIP2", "MARATIP2"};
//str CoolTips3[CLASSCOUNT] = {"DOOMTIP2", "CHEXTIP3", "HERETIP3", "WOLFTIP3", "HEXNTIP3", "DUKETIP3", "MARATIP3"}; // NOPE AIN'T WORKIN'

script 300 (int tipboxshit) NET
{
    int classNumber = samsaraClassNum();
    switch (tipboxshit)
    {
      case 1:
        SetHudSize(1024,768,1);
        SetFont(CoolTips1[classNumber]);
        HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
        break;
        
      case 2:
        if (GameType () == GAME_NET_COOPERATIVE)
        {
            if (CheckInventory("DoomguyClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("DOOMTIP2");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("ChexClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("CHEXTIP2");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("CorvusClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("HERETIP2");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("WolfenClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("WOLFTIP2");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("HexenClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("HEXNTIP2");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("DukeClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("DUKETIP2");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("MarathonClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("MARATIP2");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("QuakeClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("QUAKTIP2");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
        }
        else if (GameType () == GAME_SINGLE_PLAYER)
        {
            if (CheckInventory("DoomguyClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("DOOMTIP2");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("ChexClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("CHEXTIP2");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("CorvusClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("HERETIP2");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("WolfenClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("WOLFTIP2");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("HexenClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("HEXNTIP2");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("DukeClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("DUKETIP2");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("MarathonClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("MARATIP2");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("QuakeClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("QUAKTIP2");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
        }
        else
        {
            if (CheckInventory("DoomguyClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("DOOMTIP2");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("ChexClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("CHEXTIP3");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("CorvusClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("HERETIP3");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("WolfenClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("WOLFTIP3");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("HexenClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("HEXNTIP3");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("DukeClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("DUKETIP3");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("MarathonClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("MARATIP3");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
            if (CheckInventory("QuakeClass"))
            {
                SetHudSize(1024,768,1);
                SetFont("QUAKTIP3");
                HudMessage(s:"A";HUDMSG_PLAIN,1,1,512.0,384.0,0);
            }
        }
        break;
        
      case 3:
        SetHudSize(1024,768,1);
        SetFont(CoolTips1[8]);
        HudMessage(s:"";HUDMSG_PLAIN,1,1,512.0,384.0,0);
        break;
    }
}


// TIPBOX END DERP

/*
*
* This is still to be converted.
*
*/

int keys[3][26] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {"RedCard", "YellowCard", "BlueCard", "RedSkull", "YellowSkull", "BlueSkull", "KeyBlue", "KeyGreen", "KeyYellow", "ChexRedCard", "ChexYellowCard", "ChexBlueCard", "RedFlemKey", "YellowFlemKey", "BlueFlemKey", "KeyAxe", "KeyCastle", "KeyCave", "KeyDungeon", "KeyEmerald", "KeyFire", "KeyHorn", "KeyRusted", "KeySilver", "KeySteel", "KeySwamp"},
{"\cgRed Keycard", "\ckYellow Keycard", "\chBlue Keycard", "\cgRed Skull", "\ckYellow Skull", "\chBlue Skull", "\chBlue Prism Key", "\cqGreen Prism Key", "\ckYellow Prism Key", "\cgRed Card", "\ckYellow Card", "\chBlue Card", "\cgRed Flem Key", "\ckYellow Flem Key", "\chBlue Flem Key", "\cuAxe Key", "\cfCastle Key", "\csCave Key", "\cuDungeon Key", "\cdEmerald Key", "\cgFire Key", "\ceHorn Key", "\cbRusted Key", "\cuSilver Key", "\cmSteel Key", "\cpSwamp Key"}};
//0, 1, 2: Doom R/Y/B Keycard. - \cg, \ck, \ch
//3, 4, 5: Doom R/Y/B Skull. - \cg, \ck, \ch
//6, 7, 8: Heretic B/G/Y Prism Key. - \ch, \cq, \ck
//9, 10, 11: Chex Quest R/Y/B Keycard. - \cg, \ck, \ch
//12, 13, 14: Chex Quest R/Y/B Flem Key. - \cg, \ck, \ch
//15: Axe Key - \cu (Grey)
//16: Castle Key - \cf (Gold)
//17: Cave Key - \cs (Brown)
//18: Dungeon Key - \cu (Grey)
//19: Emerald Key - \cd (Light Green)
//20: Fire Key - \cg (Red)
//21: Horn Key - \ce (Beige)
//22: Rusted Key - \cb (Very light off-white)
//23: Silver Key - \cu (Grey)
//24: Steel Key - \cm (Black)
//25: Swamp Key - \cp (Drab green-brown)
//SCRIPTS
//200:
//201: Check if SP/DM/co-op for the Tome of Power/Morph Ovum.
//202: Cooldown for Tome of Power.
//203: Take away all the shit on level ending!
//204: Boss monologues.
//205: Cooldown for Duke's taunts.
//207: Flechette cooldown.
//208: Buddha mode for B.J.'s Extra Life.
//209: Activate Send Full Button Info and activate sv_banjetpack/sv_lmslife/sv_lmsult.
//211: Coop/SP mode on, as well as singleplayer "what class is playing this game" determines.
//212: Displaying text.
//214: Duke Jetpack/Visor fuel draining.
//215: Turn on and off Retro Movement.
//216: Turn on and off Ballgag.
//217: Weapon bar.
//219: Weapon VII co-op perma-spawn check.
//220: Denying Duke his jetpack if the server has that option.
//273-275: Ijon Tichy's wizardry for B.J.'s retro movement.
//901-902: I'm pretty sure Synert is a wizard, too.
//224: Doomguy's vanilla animations. By Ijon Tichy, transcribed by Llewellyn.
//225: Weapon bar. By Ijon Tichy, transcribed by Llewellyn.
////////////////////
// SHARED KEYS
// (by Synert)
/////////////////

// Give keys as needed, for people joining the game.

script 901 ENTER
{
    while (PlayerInGame(PlayerNumber()))
    {
        for (int a = 0; a < 26; a++)
        {
            if (keys[0][a] == 1)
            {
                GiveInventory(keys[1][a], 1);
            }
        }
        delay(10);
    }
}

script 902 (int a) { // Picked up a key, broadcast that shit to the whole world!
    if(keys[0][a] == 0 && !isSinglePlayer()) {
        Log(n:0,s:"\c* has picked up the ",s:keys[2][a],s:"\c*."); // Let the server admins know.
        HudMessageBold(n:0,s:"\c* has picked up the ",s:keys[2][a],s:"\c*.";HUDMSG_FADEOUT, 900, CR_GOLD, 0.5, 0.1, 3.0, 0.5);
    }
    keys[0][a] = 1;
}

///////////////
// ITEM STUFF
//////////////

script 220 (void)
{
    if (GetCvar("samsara_banjetpack"))
    {
        Print(s:"The server has forbidden the jetpack. Sorry.");
    }
    else
    {
        Print(s:"You got the Jetpack!");
        GiveInventory("DukePortJetpack",1);
        GiveInventory("DukeJetpackFuel",100);
    }
}

script 214 (int dukeshit)
{
    switch(dukeshit)
    {
      case 1:
        if (CheckInventory("DukeFlying") == 1)
        {
            if (CheckInventory("DukeJetpackFuel") > 0)
            {
                delay(15);
                TakeInventory("DukeJetpackFuel",1);
                restart;
            }
            else
            {
                TakeInventory("DukeFlying",1);
                TakeInventory("DukePetJack",1);
            }
        }
        break;
        
      case 2:
        if (CheckInventory("DukeVision") == 1)
        {
            if (CheckInventory("DukeVisionFuel") > 0)
            {
                delay(10);
                TakeInventory("DukeVisionFuel",1);
                restart;
            }
            else
            {
                TakeInventory("DukeVision",1);
                TakeInventory("PowerLightAmp",1);
            }
        }
        break;
    }
}

script 202 (void)
{
    if (CheckInventory("TomeOfPowerCooldown") > 0)
    {
        if (CheckInventory("TomeOfPowerCooldown") > 240)
        {
            delay(35);
            TakeInventory("TomeOfPowerCooldown",1);
            restart;
        }
        else
        {
            delay(35);
            TakeInventory("TomeOfPowerCooldown",1);
            TakeInventory("RainTracker",1);
            restart;
        }
    }
}

script 207 (void)
{
    if (CheckInventory("DiscOfRepulsionCooldown") > 0)
    {
        delay(35);
        TakeInventory("DiscOfRepulsionCooldown",1);
        restart;
    }
}

script 208 (void)
{
    if (CheckInventory("WolfExtraLife") > 0)
    {
        SetPlayerProperty(0,1,16);
    }
    else
    {
        SetPlayerProperty(0,0,16);
    }
}

script 203 UNLOADING
{
    UnloadingNow = 1;
    int i;
    for (i = 0; i < UNLOADCOUNT; i++) { TakeInventory(UnloadRemove[i], 0x7FFFFFFF); }
}

script 201 (int hereticshit)
{
    switch(hereticshit)
    {
      case 1:
        if (GameType () == GAME_NET_COOPERATIVE)
        {
            GiveInventory ("PortTomeCoOp",1);
            Print(s:"You got the Tome of Power! Oh, yes.");
        }
        else
        {
            if (GameType () == GAME_SINGLE_PLAYER)
            {
                GiveInventory ("PortTomeCoOp",1);
                Print(s:"You got the Tome of Power! Oh, yes.");
            }
            else
            {
                GiveInventory ("PortTome",1);
                Print(s:"You got a Tome of Power! Oh, yes.");
            }
        }
        break;
        
      case 2:
        if (GameType () == GAME_NET_COOPERATIVE)
        {
            GiveInventory ("ArtiEgg2",1);
            Print(s:"You got the Morph Ovum!");
        }
        else
        {
            if (GameType () == GAME_SINGLE_PLAYER)
            {
                GiveInventory ("ArtiEgg2",1);
                Print(s:"You got the Morph Ovum!");
            }
            else
            {
                GiveInventory ("ArtiEgg3",1);
                Print(s:"You got the Morph Ovum!");
            }
        }
        break;
    }
}

script 212 (int textshit) // This is the shit for different text messages.
{
    SetFont("SMALLFONT");
    switch(textshit)
    {
      case 1:
        Print(s:"You cannot use this unless injured.");
        break;
        
      case 2:
        Print(s:"                  ");
        break;
        
      case 3:
        SetHudSize(400, 300, 0);
        Hudmessage(s:"MIGHTY BOOT ENGAGED";
        HUDMSG_PLAIN,1,CR_LIGHTBLUE,200.4,9.1,1.0);
        break;
        
      case 4:
        if (GameType () != GAME_SINGLE_PLAYER)
        {
            SetHudSize(400, 300, 0);
            Hudmessage(s:"Press any button to respawn.";
            HUDMSG_PLAIN,1,CR_LIGHTBLUE,200.4,9.1,1.75);
            delay(15);
            LocalAmbientSound("duke/mpdeath",127);
            GiveInventory("DukeTauntCooldown",5);
            ACS_ExecuteAlways(205,0,0);
        }
        break;
        
      case 5:
        Print(s:"You do not have enough fuel!");
        break;
        
      case 6:
        Print(s:"You do not have enough power!");
        break;
        
      case 7:
        Print(s:"You are already flying!");
        break;
    }
}

/////////////////
// BOSS STUFF
//////////////

script 204 (int bossmonologueshit)
{
    switch(bossmonologueshit)
    {
      case 1: // KORAX
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"So, you have come."; HUDMSG_FADEOUT,15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"I have personally watched every step of your path, mortal."; HUDMSG_FADEOUT, 15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Your journey has given you such raw, destructive power..."; HUDMSG_FADEOUT, 15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Endless mountains of corpses and blood pollute your wake..."; HUDMSG_FADEOUT, 15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"I can't help but see myself when I look at you."; HUDMSG_FADEOUT, 15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"You've done me a great favor by coming here..."; HUDMSG_FADEOUT,15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"...now there are so many more worlds open to me."; HUDMSG_FADEOUT, 15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Don't feel too sad about never seeing home again."; HUDMSG_FADEOUT, 15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"I'll be sure to give my regards when I visit on your behalf."; HUDMSG_FADEOUT, 15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Greetings, mortal..."; HUDMSG_FADEOUT, 15, CR_RED,320.4, 150.0, 5.5, 1.0);
        delay(100);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"...Are you ready to die?"; HUDMSG_FADEOUT, 15, CR_RED,320.4, 150.0, 5.5, 1.0);
        break;
        
      case 2: // LORD SNOTFOLUS
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Glorp...glorp...glorp..."; HUDMSG_FADEOUT,15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Isn't it beautiful?"; HUDMSG_FADEOUT, 15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Look at it all. Look at all the goop that surrounds you."; HUDMSG_FADEOUT, 15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Your people call it repulsive. Revolting. Disgusting."; HUDMSG_FADEOUT, 15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"For me...it's life."; HUDMSG_FADEOUT, 15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Would you deny a man food? Would you deny a man life?"; HUDMSG_FADEOUT,15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Your food is made for only one purpose--to consume, as we have."; HUDMSG_FADEOUT, 15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Would you blame me for leading my people to prosperous grounds?"; HUDMSG_FADEOUT, 15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"If I'm to play the villain simply for eating, then so be it."; HUDMSG_FADEOUT, 15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"I will bury you in slime."; HUDMSG_FADEOUT, 15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        delay(100);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Glorp...glorp...glorp..."; HUDMSG_FADEOUT, 15, CR_GREEN,320.4, 150.0, 5.5, 1.0);
        break;
        
      case 3: // D'SPARIL
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"I suppose congratulations are in order."; HUDMSG_FADEOUT,15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"You've come all this way to the doors of my sanctum."; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"How many times have you narrowly dodged the clutch of death?"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"It's such a shame that it was all ultimately pointless."; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Do you realize you are the only living mortal left on this planet?"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"I command the vast power of the cosmos, the stars above you."; HUDMSG_FADEOUT,15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"I command the vast power of the hells, the nethers below you."; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"I command the vast power of the dead, the corpses around you."; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"I can effortlessly rebuild anything you have broken with just a thought."; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Gape at your own insignificance in the greater workings of the universe."; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(140);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Surrender to D'Sparil."; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        break;
    }
}

script 205 (void)
{
    while (CheckInventory("DukeTauntCooldown") > 0)
    {
        Delay(35);
        TakeInventory("DukeTauntCooldown", 1);
    }
}

script 583 (int x, int y, int z)
{
    if (GetActorZ(0) - GetActorFloorZ(0) > 4.0)
    {
        SetResultValue(1);
    }
    else
    {
        SetResultValue(sqrt(x*x + y*y + z*z) );
    }
}

script 586 (int divI, int divF, int divF1)
{
    int div = percFloat2(divI, divF, divF1);

    int x = GetActorVelX(0);
    int y = GetActorVelY(0);
    int z = GetActorVelZ(0);

    SetActorVelocity(0, FixedMul(x, div), FixedMul(y, div), FixedMul(z, div), 0, 1);
}
