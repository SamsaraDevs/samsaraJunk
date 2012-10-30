#include "zcommon.acs"
#library "samsara_tichy"

#include "commonFuncs.h"

#include "samsaraDefs.h"
#include "samsaraFuncs.h"

int array_wolfmove[PLAYERMAX];
int array_vanillaAnim[PLAYERMAX];
int array_ballgag[PLAYERMAX];
int array_weaponBar[PLAYERMAX];

int SamsaraWepType, SamsaraClientClass, SamsaraItemFlash;
int SamsaraClientWeps[SLOTCOUNT] = {0};
int IsServer = 0;

global int 0:CommandBitchingDone;

script SAMSARA_OPEN open
{
    IsServer = 1;

    if (CommandBitchingDone == 0)
    {
        if (GameType() == GAME_SINGLE_PLAYER)
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
        
        if (!GetCVar("samsara_lmsult"))
        {   ConsoleCommand("set samsara_lmsult 0");
            ConsoleCommand("archivecvar samsara_lmsult"); }
        
        if (!GetCVar("samsara_sogravity"))
        {   ConsoleCommand("set samsara_sogravity 0");
            ConsoleCommand("archivecvar samsara_sogravity"); }
        
        if (!GetCVar("samsara_permault"))
        {   ConsoleCommand("set samsara_permault 0");
            ConsoleCommand("archivecvar samsara_permault"); }

        if (!GetCVar("compat_clientssendfullbuttoninfo"))
        {   ConsoleCommand("compat_clientssendfullbuttoninfo 1"); }

        Delay(1);
    }
}


script SAMSARA_ENTER enter { ACS_ExecuteWithResult(SAMSARA_SPAWN, 0, 0,0,0); }
script SAMSARA_RESPAWN respawn { ACS_ExecuteWithResult(SAMSARA_SPAWN, 0, 1,0,0); }

script SAMSARA_SPAWN (int respawning)
{
    int pln = PlayerNumber();

    if (isLMS()) { ApplyLMS(); }

    if (GameType() == GAME_SINGLE_PLAYER)
    {
        SamsaraWepType = samsaraClassNum()+1;
    }

    SetInventory("CoopModeOn", isCoop());

    switch (samsaraClassNum())
    {
      case CLASS_DUKE:
        GiveInventory("DukeReady", 1);
        break;

      case CLASS_MARATHON:
        if (isInvasion()) { GiveInventory("InvasionDualShottyCheck", 1); }
        break;
    }

    while (1)
    {
        SetInventory("SynthFireLeft",  keyDown(BT_ATTACK));
        SetInventory("SynthFireRight", keyDown(BT_ALTATTACK));
        SetInventory("WolfenMovement", array_wolfmove[pln]);
        SetInventory("DukeBallgag",    array_ballgag[pln]);
        SetInventory("VanillaDoom",    array_vanillaAnim[pln]);
        SetInventory("ExpandedHud",    array_weaponBar[pln]);

        if (CheckInventory("MarathonClass"))
        {
            if (GetCVar("samsara_sogravity"))
            {
                SetActorProperty(0, APROP_Gravity, 1.0);
                SetActorProperty(0, APROP_JumpZ,   9.0);
            }
            else
            {
                SetActorProperty(0, APROP_Gravity, 0.15);
                SetActorProperty(0, APROP_JumpZ,   4.0);
            }
        }

        Delay(1);

        if (isDead(0)) { break; }
    }
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
    int wolfmove = values & 1;
    int vanilla  = values & 2;
    int ballgag  = values & 4;
    int wepbar   = values & 8;

    array_wolfmove[pln]     = wolfmove;
    array_vanillaAnim[pln]  = vanilla;
    array_ballgag[pln]      = ballgag;
    array_weaponBar[pln]    = wepbar;
}


script SAMSARA_ENTER_CLIENT enter clientside
{
    int execInt, oExecInt, execStr;
    int class, oClass;
    int pln = PlayerNumber();
    int i;

    // Comment out these lines for zdoom
    int cpln = ConsolePlayerNumber();
    if (cpln != pln) { terminate; }

    execInt = 0; oExecInt = 0;
    
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
    
    if (!GetCVar("samsara_cl_pickupmode"))
    {   ConsoleCommand("set samsara_cl_pickupmode 1");
        ConsoleCommand("archivecvar samsara_cl_pickupmode"); }

    class = samsaraClassNum() + 1;

    while (PlayerInGame(pln))
    {
        oClass = class;
        class  = samsaraClassNum();

        SamsaraClientClass = class+1;

        if (oClass != class)
        {
            SamsaraItemFlash = Timer();
        }

        /*
        execStr = "{";
        for (i = 0; i < SLOTCOUNT; i++)
        {
            SamsaraClientWeps[i] = HasClassWeapon(class, i);

            execStr = StrParam(s:execStr, d:SamsaraClientWeps[i]);

            if (i != SLOTCOUNT-1) { execStr = StrParam(s:execStr, s:", "); }
            else { execStr = StrParam(s:execStr, s:"}"); }
        }

        Print(s:execStr);
        */

        if (GameType() == GAME_SINGLE_PLAYER)
        {
            array_wolfmove[pln]     = !!GetCVar("samsara_cl_wolfmove");
            array_vanillaAnim[pln]  = !!GetCVar("samsara_cl_vanilladoom");
            array_ballgag[pln]      = !!GetCVar("samsara_cl_ballgag");
            array_weaponBar[pln]    = !!GetCVar("samsara_cl_weaponhud");
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
    int toClass = SamsaraClientClass;
    int displaymode = GetCVar("samsara_cl_pickupmode");
    slot = itemToSlot(slot);

    //PrintBold(d:IsServer, s:"   ", d:toClass);
    
    switch (displaymode)
    {
      case 0:
        SetActorState(0, "NoGuy");
        break;

      case 1:
      case 2:
        if ((SamsaraItemFlash >= (Timer() - 35)) && (Timer() >= 35))
        {
            Spawn("SamsaraChangeFlash", GetActorX(0), GetActorY(0), GetActorZ(0));
        }

        if (toClass == 0) { SetActorState(0, "NoGuy"); }
        else
        {
            SetActorState(0, PickupStates[toClass-1]);
        }
        break;
    }
}

script SAMSARA_GIVEWEAPON (int slot, int dropped)
{
    int weaponStay = !!GetCVar("sv_weaponstay");
    int weaponGet  = 0;
    int pclass = samsaraClassNum();
    int hasWep = HasClassWeapon(pclass, slot);


    int a1max  = 0, a2max = 0;
    int a1max2 = 0, a2max2 = 0;
    int a1Full = 0, a2Full = 0;
    int a1diff = 0, a2diff = 0;

    int weapon  = ClassWeapons[pclass][slot][S_WEP],    wepbool = !!StrLen(weapon); 
    int ammo1   = ClassWeapons[pclass][slot][S_AMMO1],  a1bool  = !!StrLen(ammo1);
    int ammo2   = ClassWeapons[pclass][slot][S_AMMO2],  a2bool  = !!StrLen(ammo2);

    if (!wepbool)
    {
        SetResultValue(weaponStay * WEPFLAGS_WEAPONSTAY);
        terminate;
    }

    if (a1bool)
    {
        a1max   = GetAmmoCapacity(ammo1);
        a1max2  = a1max * 4; // ya never know
    }

    if (a2bool)
    {
        a2max   = GetAmmoCapacity(ammo1);
        a2max2  = a1max * 4;
    }

    if (a1Bool) { a1Full = (CheckInventory(ammo1) == a1max); }
    if (a2Bool) { a2Full = (CheckInventory(ammo2) == a2max); }

    if (dropped)
    {
        if (a1bool) { SetAmmoCapacity(ammo1, a1max2); }
        if (a2bool) { SetAmmoCapacity(ammo2, a2max2); }
    }

    if (!hasWep) { weaponGet = 1; }      // do we even have this?
    else if (!weaponStay || dropped)     // does this not stay on the ground?
    {
        if (!(a1Full && a2Full)) { weaponGet = 1; }        // can we get ammo from this?
    }

    if (weaponGet)
    {
        GiveInventory(weapon, 1);
        ACS_ExecuteAlways(SAMSARA_CLIENT_WEAPONPICKUP, 0, slot,0,0);
    }

    if (dropped)
    {
        if (a1bool) { SetAmmoCapacity(ammo1, a1max); }
        if (a2bool) { SetAmmoCapacity(ammo2, a2max); }
    }

    
    SetResultValue((weaponStay * WEPFLAGS_WEAPONSTAY) + (weaponGet * WEPFLAGS_GOTWEAPON));
}

script SAMSARA_CLIENT_WEAPONPICKUP (int slot) clientside
{
    int pln = PlayerNumber(), cpln = ConsolePlayerNumber();
    int pclass = samsaraClassNum();

    if (cpln != pln) { terminate; }

    Log(s:ClassWeapons[pclass][slot][S_PICKUPMESSAGE]);
    LocalAmbientSound(ClassPickupSounds[pclass][slot], 127);
    FadeRange(ClassFades[pclass][0], ClassFades[pclass][1], ClassFades[pclass][2], ClassFades[pclass][3],
              ClassFades[pclass][0], ClassFades[pclass][1], ClassFades[pclass][2], 0.0, 5.0/35);
}
