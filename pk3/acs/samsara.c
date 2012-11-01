#include "zcommon.acs"
#library "samsara"

#include "commonFuncs.h"

#include "samsaraDefs.h"
#include "samsaraFuncs.h"

#define DEBUG 1

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
        {   ConsoleCommand("set compat_clientssendfullbuttoninfo 1");
            ConsoleCommand("archivecvar compat_clientssendfullbuttoninfo"); }

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

        TakeInventory("WeaponGetYaaaay",  1);
        TakeInventory("WeaponGetYaaaay2", 1);

        TakeInventory("Mace", 1);
        TakeInventory("MacePowered", 1);

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
    PrintBold(s:"Player \"", n:pln+1, s:"\c-\" puked script ", d:SAMSARA_PUKE, s:" (", d:values, s:", ", d:pln, s:")");
    array_wolfmove[pln]     = values & 1;
    array_vanillaAnim[pln]  = values & 2;
    array_ballgag[pln]      = values & 4;
    array_weaponBar[pln]    = values & 8;
    array_pickupswitch[pln] = values & 16;
}

script SAMSARA_OPEN_CLIENT open clientside
{
    int i;

    while (1)
    {
        for (i = 0; i < PLAYERMAX; i++)
        {
            DukeQuoteCooldown[i] = max(0, DukeQuoteCooldown[i]-1);
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

        if (oClass != class) { SamsaraItemFlash = Timer(); }

        for (i = 0; i < SLOTCOUNT; i++)
        {
            j = SamsaraClientWeps[i];
            SamsaraClientWeps[i] = HasClassWeapon(class, i);

            if (j != SamsaraClientWeps[i]) { SamsaraClientWepFlashes[i] = Timer(); }
        }

        if (GameType() == GAME_SINGLE_PLAYER)
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

script SAMSARA_DECORATE (int choice)
{
    switch (choice)
    {
      case 1:
        SetResultValue(GetActorProperty(0, APROP_Dropped));
        break;
      
      case 2:
        if (CheckInventory("WolfenMovement") == 1) { SetActorState(0, "Spawn"); }
        break;
    }
}

/*
 *      WARNING
 *  This script is over 20 variables. Check here first for segfaults.
 *  Also, keep an eye on this script for potential desync issues.
 */

script SAMSARA_GIVEWEAPON (int slot, int dropped)
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

    if (!hasWep) { weaponGet = 1; }      // do we even have this?
    else if (!weaponStay || dropped)     // does this not stay on the ground?
    {
        if (!(a1Full && a2Full)) { weaponGet = 1; }        // can we get ammo from this?
    }

    if (weaponGet && IsServer)
    {
        GiveInventory(weapon, 1);
        GiveInventory(SlotItems[slot], 1);

        if (array_pickupswitch[PlayerNumber()] &&
                (array_pickupswitch[PlayerNumber()] >= 2 || slot > ClassWeaponSlot()))
        {
            SetWeapon(ClassWeapons[pclass][slot][S_WEP]);
        }

        Spawn("WeaponGetYaaaay", GetActorX(0), GetActorY(0), GetActorZ(0));
        Spawn("WeaponGetYaaaay2", GetActorX(0), GetActorY(0), GetActorZ(0));
        ACS_ExecuteAlways(SAMSARA_CLIENT_WEAPONPICKUP, 0, slot,GetCVar("compat_silentpickup"),0);
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
    
    SetResultValue((weaponStay * WEPFLAGS_WEAPONSTAY) + (weaponGet * WEPFLAGS_GOTWEAPON));
}

script SAMSARA_CLIENT_WEAPONPICKUP (int slot, int soundmode) clientside
{
    int pln = PlayerNumber(), cpln = ConsolePlayerNumber();
    int pclass = samsaraClassNum();

    if (DEBUG) { Print(s:"running on local tic ", d:Timer()); }

    if (cpln == pln) { Log(s:ClassWeapons[pclass][slot][S_PICKUPMESSAGE]); }

    if (soundmode == 1) { LocalAmbientSound(ClassPickupSounds[pclass][slot], 127); }
    else { ActivatorSound(ClassPickupSounds[pclass][slot], 127); }

    FadeRange(ClassFades[pclass][0], ClassFades[pclass][1], ClassFades[pclass][2], ClassFades[pclass][3],
              ClassFades[pclass][0], ClassFades[pclass][1], ClassFades[pclass][2], 0.0, 5.0/35);

    if (pclass == CLASS_DUKE && !GetCVar("samsara_cl_ballgag"))
    {
        Delay(8);

        if (!DukeQuoteCooldown[pln])
        {
            if (soundmode == 1) { LocalAmbientSound("duke/weapontaunt", 127); }
            else { ActivatorSound("duke/weapontaunt", 127); }
            DukeQuoteCooldown[pln] = 140;
        }
    }
}


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

script 901 ENTER { // Give keys as needed, for people joining the game.
    while(PlayerInGame(PlayerNumber())) {
        for(int a = 0; a < 26; a++) {
            if(keys[0][a] == 1) {
                GiveInventory(keys[1][a], 1);
            }
        }
        delay(10);
    }
}

script 902 (int a) { // Picked up a key, broadcast that shit to the whole world!
    if(keys[0][a] == 0) {
        Log(n:0,s:"\c* has picked up the ",s:keys[2][a],s:"\c*."); // Let the server admins know.
        HudMessageBold(n:0,s:"\c* has picked up the ",s:keys[2][a],s:"\c*.";HUDMSG_FADEOUT, 900, CR_GOLD, 0.5, 0.1, 3.0, 0.5);
    }
    keys[0][a] = 1;
}

///////////////
// STATUS STUFF
//////////////
//if (GameType () == GAME_NET_COOPERATIVE)
//if (GameType () == GAME_NET_TEAMGAME)
//if (GameType () == GAME_NET_DEATHMATCH)
//if (GameType () == GAME_SINGLE_PLAYER)

script 211 ENTER
{
    if (GameType () == GAME_NET_COOPERATIVE)
    {
        GiveInventory("CoopModeOn", 1);
        SetActorState(0,"CoOpModeOn");
    }
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
    TakeInventory("TomeOfPowerCooldown",300);
    TakeInventory("DiscOfRepulsionCooldown",99);
    TakeInventory("RainTracker",997);
    TakeInventory("DukeTauntCooldown",997);
    TakeInventory("MaceAmmo",997);
    TakeInventory("MacePowered",997);
    TakeInventory("Mace",997);
    TakeInventory("DukeFlying",997);
    TakeInventory("DukePetJack",997);
    TakeInventory("DukeVision",997);
    TakeInventory("DukeReady",1);
    TakeInventory("DukeWaiting",15);
    TakeInventory("Infrared",997);
    TakeInventory("SynthFireActive",99);
    TakeInventory("SynthFireLeft",99);
    TakeInventory("SynthFireRight",99);
    TakeInventory("LevelLimiter",99);
    TakeInventory("LeftReload",99);
    TakeInventory("RightReload",99);
    TakeInventory("UsingSMG",1);
    TakeInventory("UsingFists",1);
    TakeInventory("UsingPistols",1);
    TakeInventory("UsingShotguns",1);
    TakeInventory("UsingFusionPistol",1);
    TakeInventory("UsingAssaultRifle",1);
    TakeInventory("UsingSpanker",1);
    TakeInventory("UsingToasty",1);
    TakeInventory("UsingAlienWeapon",1);
    TakeInventory("UsingAlienWeapon2",1);
    TakeInventory("UsingWMC",1);
    TakeInventory("UsingDualPistols",1);
    TakeInventory("UsingDualShotguns",1);
    TakeInventory("InvasionDualShottyCheck",1);
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
        //if (GameType () == GAME_SINGLE_PLAYER)
        //{
            //Log(s:"MIGHTY BOOT ENGAGED");
        //}
        //else
        //{
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
            
            // Weapon pickup messages!
            // Doomguy
          case 11:
            Print(s:"You got the Chainsaw! Find some meat!");
            break;
            
          case 12:
            Print(s:"You got the Shotgun!");
            break;
            
          case 13:
            Print(s:"You got the Super Shotgun!");
            break;
            
          case 14:
            Print(s:"You got the Chaingun!");
            break;
            
          case 15:
            Print(s:"You got the Rocket Launcher!");
            break;
            
          case 16:
            Print(s:"You got the Plasma Rifle!");
            break;
            
          case 17:
            Print(s:"You got the B.F.G. 9000! Oh, yes.");
            break;
            
          case 18:
            Print(s:"You got the Berserk! Rip and tear!");
            break;
            
            // Corvus
          case 21:
            Print(s:"You got the Gauntlets of the Necromancer!");
            break;
            
          case 22:
            Print(s:"You got the Firemace!");
            break;
            
          case 23:
            Print(s:"You got the Ethereal Crossbow!");
            break;
            
          case 24:
            Print(s:"You got the Dragon Claw!");
            break;
            
          case 25:
            Print(s:"You got the Phoenix Rod!");
            break;
            
          case 26:
            Print(s:"You got the Hellstaff!");
            break;
            
          case 27:
            Print(s:"Honor forbids you from using this until you are wounded again.");
            break;
            
            //case 27:
            // Tome of Power is already handled by ACS!
            
            // Chex Warrior
          case 31:
            Print(s:"You got the Super Bootspork! Find some milk!");
            break;
            
          case 32:
            Print(s:"You got the Large Zorcher!");
            break;
            
          case 33:
            Print(s:"You got the Super Large Zorcher!");
            break;
            
          case 34:
            Print(s:"You got the Rapid Zorcher!");
            break;
            
          case 35:
            Print(s:"You got the Zorch Propulsor!");
            break;
            
          case 36:
            Print(s:"You got the Phasing Zorcher!");
            break;
            
          case 37:
            Print(s:"You got the LAZ Device! Woot!");
            break;
            
          case 38:
            Print(s:"You got the Supercharge Breakfast!");
            break;
            
            // B.J. Blazkowicz
          case 41:
            Print(s:"That's not a knife. THIS is a knife!");
            break;
            
          case 42:
            Print(s:"You got the Machine Gun!");
            break;
            
            //case 43:
            //break;
            // Doesn't exist.
            
            //case 44:
            //Print(s:"You got the Chaingun!");
            //break;
            // Case 14 handles this.
            
            //case 45:
            //Print(s:"You got the Rocket Launcher!");
            //break;
            // Case 15 handles this.
            
          case 46:
            Print(s:"You got the Flamethrower!");
            break;
            
          case 47:
            Print(s:"You got the Spear of Destiny!");
            break;
            
          case 48:
            Print(s:"You got an Extra Life!");
            break;
            
            // Parias
          case 51:
            Print(s:"You got the Flechette!");
            break;
            
          case 52:
            Print(s:"You got the Frost Shards!");
            break;
            
          case 53:
            Print(s:"You got Timon's Axe!");
            break;
            
          case 54:
            Print(s:"You got the Serpent Staff!");
            break;
            
          case 55:
            Print(s:"You got the Hammer of Retribution!");
            break;
            
          case 56:
            Print(s:"You got the Firestorm!");
            break;
            
          case 57:
            Print(s:"You got the Wraithverge! Scream for me!");
            break;
            
          case 58:
            Print(s:"You got the Mystic Ambit Incant!");
            break;
            
            // Duke
          case 61:
            Print(s:"You got the Pipebombs!");
            break;
            
            //case 62:
            //Print(s:"You got the Shotgun!");
            //break;
            // Case 12 handles this
            
          case 63:
            Print(s:"You got the Explosive Shotgun!");
            break;
            
          case 64:
            Print(s:"You got the Ripper Chaingun Cannon!");
            break;
            
          case 65:
            Print(s:"You got the RPG!");
            break;
            
          case 66:
            Print(s:"You got the Freezethrower!");
            break;
            
          case 67:
            Print(s:"You got the Devastator!");
            break;
            
          case 68:
            Print(s:"You got the Jetpack!");
            break;
            
            // Security Officer
          case 71:
            HudMessage(s:"You got the KKV-7 SMG Flechette!\n",
            s:"You got a second .44 Magnum Mega Class A1!";
            HUDMSG_PLAIN, 0, CR_GOLD, 0.5, 0.4, 2.0);
            break;
            
          case 72:
            Print(s:"You got the WSTE-M5 Combat Shotgun!");
            break;
            
          case 73:
            Print(s:"You got the Zeus Class Fusion Pistol!");
            break;
            
          case 74:
            Print(s:"You got the MA-75B Assault Rifle!");
            break;
            
          case 75:
            Print(s:"You got the SPNKR-XP SSM Launcher!");
            break;
            
          case 76:
            Print(s:"You got the TOZT-7 Backpack Napalm Unit!");
            break;
            
          case 77:
            Print(s:"You got the ONI-71 Wave Motion Cannon!");
            break;
            
          case 78:
            HudMessage(s:"You got an\cina\cfaNN\ckAa\ciN \ccSYSTEM ERROR 0xfded";
            HUDMSG_PLAIN, 0, CR_GOLD, 0.5, 0.4, 2.0);
            break;
            
          case 79:
            Print(s:"You got a second WSTE-M5 Combat Shotgun!");
            break;
            
          case 80:
            Print(s:"You got a pair of WSTE-M5 Combat Shotguns!");
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
