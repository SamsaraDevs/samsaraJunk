#include "zcommon.acs"
#library "SAMSARA"
#define DEBUG 0

int DoomguyGame;
int ChexterGame;
int CorvusGame;
int WolfenGame;
int HexenGame;
int DukeGame;
int MarathonGame;
Global int 0 : FlagsSet;

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

function int isCooperative(void)
{
    int ret = (GetCVar("teamplay") == false) && (GetCVar("ctf") == false) &&
    (GetCVar("deathmatch") == false);
    
    if (DEBUG)
    {
        Log(s:"isCooperative returns ", d:ret);
    }
    
    return ret;
}

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


script 274 (void)
{
    if (CheckInventory("WolfenMovement") == 1)
    {
        SetActorState(0, "Spawn");
    }
}

script 275 (void) NET
{
    if (CheckInventory("WolfenClass") == 1)
    {
        SetActorProperty(0, APROP_Speed, 1.0);
    }
    else
    {
    }
}

///////////////
// STATUS STUFF
//////////////
//if (GameType () == GAME_NET_COOPERATIVE)
//if (GameType () == GAME_NET_TEAMGAME)
//if (GameType () == GAME_NET_DEATHMATCH)
//if (GameType () == GAME_SINGLE_PLAYER)
script 219 (void)
{
    if(GetCvar("sv_permault") == 1)//if(GameType () == GAME_NET_COOPERATIVE)
    setresultvalue(1);
    else setresultvalue(0);
}

script 224 (int Cvar) NET
{
    if (CheckInventory("DoomguyClass"))
    if (CVar)
    {
        TakeInventory("VanillaDoom", 1);
        Print(s:"Smooth animations off.");
    }
    else
    {
        GiveInventory("VanillaDoom", 1);
        Print(s:"Vanilla animations on.");
    }
    else
    Print(s:"Only Doomguy can use this.");
}

script 225 (int Cvar) NET
{
    if (CVar)
    {
        TakeInventory("ExpandedHud", 1);
        Print(s:"Weapon bar off.");
    }
    else
    {
        GiveInventory("ExpandedHud", 1);
        Print(s:"Weapon bar on.");
    }
}

script 217 (void) NET CLIENTSIDE
{
    ConsoleCommand(StrParam(s:"puke 225 ",d:GetCvar("cl_weaponhud")));
    if (GetCvar("cl_weaponhud"))
    ConsoleCommand("set cl_weaponhud 0;");
    else
    ConsoleCommand("set cl_weaponhud 1");
    ConsoleCommand("archivecvar cl_weaponhud");
}

script 211 ENTER
{
    if (GameType () == GAME_NET_COOPERATIVE)
    {
        GiveInventory("CoopModeOn", 1);
        SetActorState(0,"CoOpModeOn");
    }
    else
    {
        if (GameType () == GAME_SINGLE_PLAYER)
        {
            GiveInventory("SPModeOn", 1);
            if(checkInventory("DoomguyClass")) DoomguyGame = 1;
            if(checkInventory("ChexClass")) ChexterGame = 1;
            if(checkInventory("CorvusClass")) CorvusGame = 1;
            if(checkInventory("WolfenClass")) WolfenGame = 1;
            if(checkInventory("HexenClass")) HexenGame = 1;
            if(checkInventory("DukeClass")) DukeGame = 1;
            if(checkInventory("MarathonClass")) MarathonGame = 1;
        }
    }
}


///////////////
// ITEM STUFF
//////////////

script 220 (void)
{
    if (GetCvar("sv_banjetpack") == 1)
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

script 205 (void)
{
    if (CheckInventory("DukeTauntCooldown") > 0)
    {
        delay(35);
        TakeInventory("DukeTauntCooldown",1);
        restart;
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
