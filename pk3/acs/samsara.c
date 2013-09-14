#include "zcommon.acs"
#library "samsara"

#include "commonFuncs.h"

#include "samsaraDefs.h"
#include "samsaraWeps.h"
#include "samsaraSounds.h"
#include "samsaraMsgs.h"
#include "samsaraStrife.h"

int array_wolfmove[PLAYERMAX];
int array_vanillaAnim[PLAYERMAX];
int array_ballgag[PLAYERMAX];
int array_weaponBar[PLAYERMAX];
int array_pickupswitch[PLAYERMAX];
int DukeQuoteCooldown[PLAYERMAX];
int ServerEnterTimes[PLAYERMAX];
int WolfenEnterTimes[PLAYERMAX];
int ClientEnterTimes[PLAYERMAX];
int ClientTipboxes[PLAYERMAX];
int ResonantTimes[PLAYERMAX][RESCOUNT];
int ScheduledTimes[PLAYERMAX][3];

int SamsaraWepType, SamsaraClientClass, SamsaraItemFlash;
int SamsaraClientWeps[SLOTCOUNT] = {0};
int SamsaraClientWepFlashes[SLOTCOUNT] = {0};
int IsServer = 0;
int LMSMessaged = 0;
int UnloadingNow = 0;
int ArmorMode = -1;
int IsPunchdrunk = 0;
int MapArmors[ARMORCOUNT] = {-1};
int ClientTipboxModifier, ClientTipClassModifier;

global int 0:SamsaraGlobal[];

#include "samsaraFuncs.h"

#include "script_auto.h"
#include "script_pickup.h"
#include "script_marathon.h"
#include "script_quake.h"
#include "script_tipbox.h"
#include "script_strife.h"
#include "script_scheduled.h"
#include "script_pdwtak.h"

script SAMSARA_DECORATE (int choice, int arg1, int arg2)
{
    int clipcount;
    int result;
    int i, j, k;
    int x, y, z;
    int armorIndex, armorToSet;
    int pln = PlayerNumber();
    
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
        break;

      case 5:
        SetActivatorToTarget(0);
        result = CheckInventory("Cell");
        if (arg1) { TakeInventory("Cell", result); }
        break;

      case 6:
        result = GetCVar("skulltag");
        break;
      
      case 7:
        if (arg2 != 1)
        {
            GiveQuad(arg1);
        }
        else
        {
            if (isLMS())
            {
                GiveInventory("QuadDamageItem", 1);
                break;
            }

            GiveQuad(arg1);

            if (isCoop())
            {
                GiveInventory("QuadDamageItem", 1);
            }
        }
        break;

      case 8:
        result = defaultCVar("samsara_cl_expparticles", 0);
        if (!result) { result = 100; }

        result = max(0, result);

        result *= max(arg1, 1);
        result /= max(arg2, 1);

        GiveInventory("QuakeExplosionCounter", result);
        break;

      case 9:
        clipcount = CheckInventory("Clip");

        if (clipcount < 50)
        {
            GiveInventory("Clip", 50 - clipcount);
            TakeInventory("Clip", CheckInventory("Clip") - 50);
            result = 1;
        }
        break;

      case 10:
        TakeInventory("QuakeExplosionCounter", arg1);
        result = CheckInventory("QuakeExplosionCounter");
        break;

      case 15:
        SetActorProperty(0, APROP_Speed, percFloat(arg1, arg2));
        break;
        
      case 16:
        if (GameType () != GAME_SINGLE_PLAYER)
        {
            SetHudSize(400, 300, 0);
            Hudmessage(s:"Press any button to respawn.";
            HUDMSG_PLAIN,1,CR_LIGHTBLUE,200.4,9.1,1.75);
            delay(15);

            if (!CheckInventory("DukeBallgag"))
            {
                LocalAmbientSound("duke/mpdeath",127);
                GiveInventory("DukeTauntCooldown",5);
                ACS_ExecuteAlways(205,0,0);
            }
        }
        break;

      case 17:
        if (arg1) { result = GetCVar("samsara_permault"); }
        else { result = GetCVar("sv_weaponstay"); }
        break;

      case 18:
        if (MapArmors[0] == -1) { CheckMapArmors(); }
        SetArmorMode();

        i = Timer() != 0;

        if (MapArmors[ARMOR_YELLOW] == 1) { i += 2; }

        SetActorState(0, ArmorModeStates[ArmorMode][i]);
        break;

      case 19:
        result = isLMS();
        break;

      case 20:
        SetArmorMode();

        armorIndex = -1;
        armorToSet = arg1;

        for (i = 0; i < ARMORCOUNT; i++)
        {
            if (GetArmorType(ArmorItems[ArmorMode][i][0], pln))
            {
                armorIndex = i;
                break;
            }
        }

        arg1 = middle(0, arg1, ARMORCOUNT-1);
        i = CheckInventory("Armor");
        j = ArmorItems[ArmorMode][arg1][1];

        if (j == 0) { result = 0; break; }


        /* If we're adding armor, always follow through
           Else, if the ending armor count is lower than the current armor count
           and we're not upgrading our armor, give up now */

        if (arg2 > 0)
        {
            if (arg1 <= armorIndex) { armorToSet = armorIndex; }
        }
        else if (((arg2 == 0 && i > j) || (arg2 < 0 && i > -arg2)) && (arg1 <= armorIndex))
        {
            result = 0;
            break;
        }

        if (arg2 <= 0)
        {
            TakeInventory("BasicArmor", i);
            GiveInventory(ArmorItems[ArmorMode][armorToSet][0], 1);

            k = CheckInventory("Armor");

            if (arg2 == 0) { break; }

            TakeInventory("BasicArmor", k-1);
            GiveInventory("InfiniteArmorBonus", -arg2 - 1);
        }
        else
        {
            TakeInventory("BasicArmor", i);
            GiveInventory(ArmorItems[ArmorMode][armorToSet][0], 1);

            k = CheckInventory("Armor");
            TakeInventory("BasicArmor", k-1);

            GiveInventory("InfiniteArmorBonus", (i + arg2) - 1);
        }

        result = 1;
        break;

      case 21:
        i = CheckInventory("Armor");
        if (i < arg1) { result = 0; break; }

        TakeInventory("BasicArmor", i-arg1);
        result = 1;
        break;

      case 22:
        result = GetCVar("samsara_nohealthcap");
        break;

      case 23:
        GiveInventory("TimeBombPause", 1);
        Delay(arg1);
        TakeInventory("TimeBombPause", 1);
        break;

      case 24:
        result = GetCVar("samsara_noult");
        break;

      case 25:
        if (GameType() == GAME_NET_COOPERATIVE)
		{ AmbientSound("quake/invisannouncer",127); }
		else
		{ LocalAmbientSound("quake/invisannouncer",127); }
        break;
    }
    
    SetResultValue(result);
}

script SAMSARA_CLIENT_DECORATE (int which, int a1, int a2) clientside // This is the shit for different text messages.
{
    int i, j, k;
    int x, y, z;

    SetFont("SMALLFONT");
    switch (which)
    {
      case 1:
        Print(s:"You cannot use this unless injured.");
        break;
        
      case 2:
        Print(s:"");
        break;
        
      case 3:
        SetHudSize(400, 300, 0);
        Hudmessage(s:"MIGHTY FOOT ENGAGED";
        HUDMSG_PLAIN,1,CR_LIGHTBLUE,200.4,9.1,1.0);
        break;

      case 4:
        while (1)
        {
            if (defaultCVar("samsara_cl_noadditivepickups", 0))
            {
                SetActorProperty(0, APROP_RenderStyle, STYLE_Normal);
                SetActorProperty(0, APROP_Alpha, itof(a1)/100);
            }
            else
            {
                SetActorProperty(0, APROP_RenderStyle, STYLE_Add);
                SetActorProperty(0, APROP_Alpha, itof(a2)/100);
            }

            Delay(35);
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

      case 8:
        if (IsServer) { terminate; }
        IsPunchdrunk = a1;
        break;

      case 9:
        x = GetActorX(0); y = GetActorY(0); z = GetActorZ(0);
        i = GetActorPitch(0);
        j = GetActorAngle(0);
        k = unusedTID(4000, 14000);

        z += itof(cond(keyDown(BT_CROUCH), random(10, 14), random(30, 34)));
        Spawn("GauntletSparks", x + FixedMul(cos(i), 16 * cos(j)), y + FixedMul(cos(i), 16 * sin(j)), z - (16 * sin(i)), k);
        SetActorVelocity(k, GetActorVelX(0), GetActorVelY(0), GetActorVelZ(0), 0,0);
        break;
    }
}

script SAMSARA_GETSETTINGS (void) net
{
    int lmsLevel = middle(0, GetCVar("samsara_lmslife"), LMSMODES-1);
    int lmsHP, lmsArmor;
    int lmsUlt, lmsUnique;
    int ultStay, highLow;

    if (lmsLevel) { lmsHP    = 100*lmsLevel; lmsArmor = 100*lmsLevel; }
    else { lmsHP = 100; lmsArmor = 0; }

    if (GetCVar("samsara_lmsunique")) { lmsUnique = "\cdwith"; }
    else { lmsUnique = "\cgwithout"; }

    if (GetCVar("samsara_lmsult")) { lmsUlt = "\cdwith"; }
    else { lmsUlt = "\cgwithout"; }

    if (GetCVar("samsara_permault")) { ultStay = "\cdstay"; }
    else { ultStay = "\cado not stay"; }

    if (GetCVar("samsara_jumpmod") < 0) { highLow = "\calower"; }
    else { highLow = "\cfhigher"; }

    SetHudSize(640, 480, 1);

    if (isLMS())
    {
        HudMessage(s:"Spawning with \ca", d:lmsHP, s:" health\c- and \cd", d:lmsArmor, s:" armor\c-";
            HUDMSG_FADEOUT, 6761, CR_WHITE, 50.1, 80.0, 3.0, 1.0);
        
        HudMessage(s:"You spawn ", s:lmsUnique, s:"\c- your unique and ", s:lmsUlt, s:"\c- your slot 7";
            HUDMSG_FADEOUT, 6762, CR_WHITE, 50.1, 104.0, 3.0, 1.0);
    }
    else
    {
        HudMessage(s:"Slot 7 pickups ", s:ultStay, s:"\c- on pickup";
            HUDMSG_FADEOUT, 6761, CR_WHITE, 50.1, 80.0, 3.0, 1.0);

        HudMessage(s:"Armor mode is \cf", s:ArmorModeNames[ArmorMode];
            HUDMSG_FADEOUT, 6762, CR_WHITE, 50.1, 96.0, 3.0, 1.0);
    }

    if (GetCVar("samsara_jumpmod"))
    {
        HudMessage(s:"You jump \cn", d:abs(GetCVar("samsara_jumpmod")), s:"\c- units ", s:highLow, s:"\c- than normal";
                HUDMSG_FADEOUT, 6763, CR_WHITE, 50.1, 112.0, 3.0, 1.0);
    }
    else
    {
        HudMessage(s:"Jumping is \cbnormal";
                HUDMSG_FADEOUT, 6763, CR_WHITE, 50.1, 112.0, 3.0, 1.0);
    }


    if (GetCVar("samsara_banjetpack"))
    {
        HudMessage(s:"Duke's jetpack is \cgBANNED.";
                HUDMSG_FADEOUT, 6764, CR_WHITE, 50.1, 128.0, 3.0, 1.0);
    }
    else
    {
        HudMessage(s:"Duke's jetpack is \cdALLOWED.";
                HUDMSG_FADEOUT, 6764, CR_WHITE, 50.1, 128.0, 3.0, 1.0);
    }

    if (GetCVar("samsara_banwolfmove"))
    {
        HudMessage(s:"Wolfenstein movement is \cgBANNED.";
                HUDMSG_FADEOUT, 6765, CR_WHITE, 50.1, 144.0, 3.0, 1.0);
    }
    else
    {
        HudMessage(s:"Wolfenstein movement is \cdALLOWED.";
                HUDMSG_FADEOUT, 6765, CR_WHITE, 50.1, 144.0, 3.0, 1.0);
    }

    if (GetCVar("samsara_nocustomgravity"))
    {
        HudMessage(s:"Custom gravities are \cadisabled.";
                HUDMSG_FADEOUT, 6766, CR_WHITE, 50.1, 160.0, 3.0, 1.0);
    }
    else
    {
        HudMessage(s:"Custom gravities are \cdenabled.";
                HUDMSG_FADEOUT, 6766, CR_WHITE, 50.1, 160.0, 3.0, 1.0);
    }

    HudMessage(s:"Party mode is ", s:cond(GetCVar("samsara_peoplediewhentheyarekilled"), "\cdON.", "\caOFF."), s:"\c- (", d:GetCVar("samsara_peoplediewhentheyarekilled"), s:")";
            HUDMSG_FADEOUT, 6767, CR_WHITE, 50.1, 176.0, 3.0, 1.0);

    HudMessage(s:"Gentleman mode is ", s:cond(GetCVar("samsara_punchdrunk"), "\cdON.", "\caOFF."), s:"\c- (", d:GetCVar("samsara_punchdrunk"), s:")";
            HUDMSG_FADEOUT, 6768, CR_WHITE, 50.1, 192.0, 3.0, 1.0);

    HudMessage(s:"Limitless health is ", s:cond(GetCVar("samsara_nohealthcap"), "\cdON.", "\caOFF.");
            HUDMSG_FADEOUT, 6769, CR_WHITE, 50.1, 208.0, 3.0, 1.0);

    HudMessage(s:"Chainsaw/unique start: \ca", d:GetCVar("samsara_chainsawstart"), s:"\c- / \cn", d:GetCvar("samsara_uniquestart");
            HUDMSG_FADEOUT, 6770, CR_WHITE, 50.1, 224.0, 3.0, 1.0);
}

/*
 *
 * This is still to be converted.
 *
 */

int keys[3][26] = {{0},
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
//212: Displaying text.
//214: Duke Jetpack/Visor fuel draining.
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
    if (!(IsSinglePlayer() || IsCoop())) { terminate; }

    while (1)
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
    if(keys[0][a] == 0 && isCoop() && !isSinglePlayer()) {
        Log(n:0,s:"\c* has picked up the ",s:keys[2][a],s:"\c*."); // Let the server admins know.
        HudMessageBold(n:0,s:"\c* has picked up the ",s:keys[2][a],s:"\c*.";HUDMSG_FADEOUT, 900, CR_GOLD, 0.5, 0.1, 3.0, 0.5);
    }
    keys[0][a] = 1;
}

///////////////
// ITEM STUFF
//////////////

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
    while (CheckInventory("TomeOfPowerCooldown") > 0)
    {
        Delay(35);

        if (CheckInventory("TomeOfPowerCooldown") > 240)
        {
            TakeInventory("TomeOfPowerCooldown",1);
        }
        else
        {
            TakeInventory("TomeOfPowerCooldown",1);
            TakeInventory("RainTracker",1);
        }

        if (CheckInventory("TomeOfPowerCooldown") == 160)  // To end the spectral effect when the tome ends
        {
            TakeInventory("SpectralFiring", 0x7FFFFFFF);
            ActivatorSound("weapons/sigil", 127);
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

script 203 unloading
{
    int i;
    UnloadingNow = 1;

    for (i = 0; i < UNLOADCOUNT; i++) { TakeInventory(UnloadRemove[i], 0x7FFFFFFF); }
}

/////////////////
// BOSS STUFF
//////////////

script 204 (int bossmonologueshit)
{
    switch(bossmonologueshit)
    {
      case 1: // KORAX
	  if (GetCVar("samsara_nomonologues"))
	  {
		SetActorState(0,"Idle");
	  }
	  else
	  {
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
		}
        break;
        
      case 2: // LORD SNOTFOLUS
	  if (GetCVar("samsara_nomonologues"))
	  {
		SetActorState(0,"Idle");
	  }
	  else
	  {
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
		}
        break;
        
      case 3: // D'SPARIL
	  if (GetCVar("samsara_nomonologues"))
	  {
		SetActorState(0,"Idle");
	  }
	  else
	  {
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
		}
        break;
        
      case 4: // THE ENTITY
	  if (GetCVar("samsara_nomonologues"))
	  {
		SetActorState(0,"Idle");
	  }
	  else
	  {
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"What is this, I spy?"; HUDMSG_FADEOUT,15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"A tiny speck of a mortal, crawling into the depths of my chambers?"; HUDMSG_FADEOUT, 15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Is this the one that has caused so much trouble?"; HUDMSG_FADEOUT, 15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Fool."; HUDMSG_FADEOUT, 15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"There is no way out, from here."; HUDMSG_FADEOUT, 15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"The doors close behind you. The path goes only ahead."; HUDMSG_FADEOUT,15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"In front of you lies my legions, all craving your blood."; HUDMSG_FADEOUT, 15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Even my own Sigil, which you have fought so hard for, pulls away at your very soul."; HUDMSG_FADEOUT, 15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Despair, for every step you take is a step further into my waiting maw."; HUDMSG_FADEOUT, 15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Despair, for even after death, I will never let you know peace."; HUDMSG_FADEOUT, 15, CR_WHITE,320.4, 150.0, 5.5, 1.0);
        delay(140);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Despair, for I am the One God."; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
		}
        break;
		
      case 5: // THE ICON OF SIN
	  if (GetCVar("samsara_nomonologues"))
	  {
		SetActorState(0,"Idle");
	  }
	  else
	  {
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"In all my aeons, I have never met one like you."; HUDMSG_FADEOUT,15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"I wonder what your people would call you."; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Mighty? Great? Savior? Hero?"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Who knows how many they would have to sort through to find one for you?"; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Some fitting word that could suit all the incredible deeds you've done."; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Do you know the title I have?"; HUDMSG_FADEOUT,15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"None."; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"In all the years of my life, they have never found anything suitable for my power."; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Call me Baphomet. Call me Gatekeeper. Satan. Demonlord. Hellfather. The Icon of Sin."; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(154);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Whichever name you choose for me, only one will truly fit."; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
        delay(140);
        SetHudSize(640, 400, 0);
        SetFont("BIGFONT");
        HudMessageBold(s:"Your doom."; HUDMSG_FADEOUT, 15, CR_GOLD,320.4, 150.0, 5.5, 1.0);
		}
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

script 678 (int which)
{
    int x, y, z;

    x = GetActorX(0); y = GetActorY(0); z = GetActorZ(0);
    SetActivatorToTarget(0);

    ACS_ExecuteAlways(679, 0, x, y, z);
}

script 679 (int tx, int ty, int tz) clientside
{
    int t, i, k = 0, l;
    int x, y, z;
    int vx, vy, vz, mag, magI;

    x  = GetActorX(0); y =  GetActorY(0);  z = GetActorZ(0) + 24.0;

    vx = tx-x; vy = ty-y; vz = tz-z; mag = magnitudeThree_f(vx, vy, vz);
    vx = FixedDiv(vx, mag); vy = FixedDiv(vy, mag); vz = FixedDiv(vz, mag);
    magI = ftoi(mag);

    for (i = 8; i < magI; i += 8)
    {
        Spawn("SpnkrBeam", x+(vx*i), y+(vy*i), z+(vz*i));
        
        l += (i - k);
        Delay(l / 512);
        l %= 512;
        k = i;
    }
}