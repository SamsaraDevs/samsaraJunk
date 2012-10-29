#include "zcommon.acs"
#library "samsara_tichy"

#include "commonFuncs.h"

#include "samsaraDefs.h"
#include "samsaraFuncs.h"

int array_wolfmove[PLAYERMAX];
int array_vanillaAnim[PLAYERMAX];
int array_ballgag[PLAYERMAX];

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

        if (!GetCvar("sv_banjetpack"))
        {   ConsoleCommand("set sv_banjetpack 0");
            ConsoleCommand("archivecvar sv_banjetpack"); }
        
        if (!GetCvar("sv_lmslife"))
        {   ConsoleCommand("set sv_lmslife 0");
            ConsoleCommand("archivecvar sv_lmslife"); }
        
        if (!GetCvar("sv_lmsult"))
        {   ConsoleCommand("set sv_lmsult 0");
            ConsoleCommand("archivecvar sv_lmsult"); }
        
        if (!GetCvar("sv_sogravity"))
        {   ConsoleCommand("set sv_sogravity 0");
            ConsoleCommand("archivecvar sv_sogravity"); }
        
        if (!GetCvar("sv_permault"))
        {   ConsoleCommand("set sv_permault 0");
            ConsoleCommand("archivecvar sv_permault"); }

        if (!GetCVar("compat_clientssendfullbuttoninfo"))
        {   ConsoleCommand("compat_clientssendfullbuttoninfo 1"); }

        Delay(1);
    }
}


script SAMSARA_ENTER enter { ACS_ExecuteAlways(SAMSARA_SPAWN, 0, 0,0,0); }
script SAMSARA_RESPAWN respawn { ACS_ExecuteAlways(SAMSARA_SPAWN, 0, 1,0,0); }

script SAMSARA_SPAWN (int respawning)
{
    int pln = PlayerNumber();

    if (isLMS()) { ApplyLMS(); }

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
        SetInventory("SynthFireLeft", keyDown(BT_ATTACK));
        SetInventory("SynthFireRight", keyDown(BT_ALTATTACK));

        if (CheckInventory("MarathonClass"))
        {
            if (GetCVar("sv_sogravity"))
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

        Print(d:array_wolfmove[pln], s:" ", d:array_vanillaAnim[pln], s:" ", d:array_ballgag[pln]);

        Delay(1);

        if (isDead(0)) { break; }
    }
}

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

    array_wolfmove[pln]     = wolfmove;
    array_vanillaAnim[pln]  = vanilla;
    array_ballgag[pln]      = ballgag;
}


script SAMSARA_ENTER_CLIENT enter clientside
{
    int execInt, oExecInt, execStr;
    int pln = PlayerNumber();

    execInt = 0; oExecInt = 0;
    
    if (!GetCvar("cl_wolfmove"))
    {   ConsoleCommand("set cl_wolfmove 0");
        ConsoleCommand("archivecvar cl_wolfmove"); }
    
    if (!GetCvar("cl_vanilladoom"))
    {   ConsoleCommand("set cl_vanilladoom 0");
        ConsoleCommand("archivecvar cl_vanilladoom"); }
    
    if (!GetCvar("cl_ballgag"))
    {   ConsoleCommand("set cl_ballgag 0");
        ConsoleCommand("archivecvar cl_ballgag"); }

    while (PlayerInGame(pln))
    {
        if (GameType() == GAME_SINGLE_PLAYER)
        {
            array_wolfmove[pln]     = !!GetCvar("cl_wolfmove");
            array_vanillaAnim[pln]  = !!GetCvar("cl_vanilladoom");
            array_ballgag[pln]      = !!GetCvar("cl_ballgag");
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
