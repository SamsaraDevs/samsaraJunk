#define SAMSARA_ENTER               611
#define SAMSARA_RETURN              613
#define SAMSARA_RESPAWN             618
#define SAMSARA_DEATH               619
#define SAMSARA_OPEN                623
#define SAMSARA_SPAWN               624
#define SAMSARA_DISCONNECT          617
#define SAMSARA_WOLFMOVE            673
#define SAMSARA_SCHEDULED           674
#define SAMSARA_PDWTAK              675
#define SAMSARA_GETSETTINGS         676

#define SAMSARA_CONFIRMCLASS        206
#define SAMSARA_DECORATE            215
#define SAMSARA_GIVEWEAPON          229
#define SAMSARA_GIVEUNIQUE          231
#define SAMSARA_CLIENT_WEAPONPICKUP 616
#define SAMSARA_CLIENT_UNIQUEPICKUP 633
#define SAMSARA_MARATHON            609
#define SAMSARA_RESONATE            608
#define SAMSARA_SYNTHFIRE           607
#define SAMSARA_MEGAHEALTH          249
#define SAMSARA_QPOWERS             636

#define SAMSARA_TIPBOX              300
#define SAMSARA_TIPBOX_CLIENT       301

#define SAMSARA_PUKE                226
#define SAMSARA_PUKE_CLIENT         227
#define SAMSARA_OPEN_CLIENT         548
#define SAMSARA_ENTER_CLIENT        521
#define SAMSARA_DISCONNECT_CLIENT   522
#define SAMSARA_CLIENT_CLASS        228
#define SAMSARA_RECOIL              312
#define SAMSARA_CLIENT_DECORATE     212

#define SAMSARA_STRIFEACCURACY      313
#define SAMSARA_CLIENT_ACCURACY     314
#define SAMSARA_BEACON              315
#define SAMSARA_SPECTRES            316
#define SAMSARA_SIGIL               317

#define SAMSARA_CL_VERSION          2601

#define CLASSCOUNT          8
#define UNIQUECOUNT         2
#define SLOTCOUNT           9
#define TIPCOUNT            2

#define LMSMODES            6
#define ARMORMODES          5

#define CLASS_DOOM      0
#define CLASS_CHEX      1
#define CLASS_HERETIC   2
#define CLASS_WOLFEN    3
#define CLASS_HEXEN     4
#define CLASS_DUKE      5
#define CLASS_MARATHON  6
#define CLASS_QUAKE     7

#define SPEED_FORWARD       15
#define SPEED_SIDE          13

#define UNLOADCOUNT 36

#define P_COUNT 2
#define P_QUAD  0
#define P_REGEN 1

#define QUAD_THRESHOLD      1050

#define REGEN_CENTER_X      0.953125    // 610 / 640
#define REGEN_CENTER_Y      0.708333    // 340 / 480

#define TIP_SCROLLRATE      8

#define GLOBAL_DONEBITCHING     0
#define GLOBAL_SIGILBASE        1

int HELPSTR = 
"Welcome to the Wheel of Samsara! There are a few optional RCon commands you may want to consider.\n\
samsara_banjetpack 0/1: Toggles whether Duke is allowed to pick up his jetpack or not.\n\
samsara_banwolfmove 0/1: Toggles whether Wolfenstein movement is allowed or not.\n\
samsara_nocustomgravity 0/1: Toggles whether the heroes have custom gravity or use only Doom gravity.\n\
samsara_jumpmod: Gives the indicated multiplier to jump height. 9 for HeXen, -8 for Strife.\n\
samsara_permault 0/1: Toggles whether the Weapon VII is persistent or vanishes on pickup.\n\
samsara_lmslife [0-5]: Affects how much health/armor people have on LMS spawn/respawn.\n\
samsara_lmsult 0/1: Toggles whether players get their VII in LMS.\n\
samsara_uniquestart [0-4]: Toggles whether players get their unique on enter or spawn.\n\
samsara_armormode [0-4]: Toggles the type of armor that spawns in-game.\n\
samsara_chainsawstart [0-2]: Toggles whether players get their I on enter or spawn.\n\
samsara_nohealthcap 0/1: Toggles whether 100/200 is the health cap for players, or infinity.";

int LMSArmors[LMSMODES] = 
{
    "",
    "GreenArmor",
    "BlueArmor",
    "LMSArmor", 
    "LMSArmor2",
    "LMSArmor3",
};

int ClassItems[CLASSCOUNT] =
{
    "DoomguyClass",
    "ChexClass",
    "CorvusClass",
    "WolfenClass",
    "HexenClass",
    "DukeClass",
    "MarathonClass",
    "QuakeClass",
};

int PickupStates[CLASSCOUNT][5] = 
{
    {"Doomguy", "DoomFull",     "DoomEmpty",        "DoomSpecial",      "DoomSpecial2"},
    {"Chexguy", "ChexFull",     "ChexEmpty",        "ChexSpecial",      "ChexSpecial2"},
    {"Heretic", "HereticFull",  "HereticEmpty",     "HereticSpecial",   "HereticSpecial2"},
    {"Wolfguy", "WolfFull",     "WolfEmpty",        "WolfSpecial",      "WolfSpecial2"},
    {"Hexen",   "HexenFull",    "HexenEmpty",       "HexenSpecial",     "HexenSpecial2"},
    {"Dukeguy", "DukeFull",     "DukeEmpty",        "DukeSpecial",      "DukeSpecial2"},
    {"Marathon","MarathonFull", "MarathonEmpty",    "MarathonSpecial",  "MarathonSpecial2"},
    {"Quake",   "QuakeFull",    "QuakeEmpty",       "QuakeSpecial",     "QuakeSpecial2"},   
};

int ItoSArray[7] = {1, 3, 4, 5, 6, 7, 8};
int StoIArray[SLOTCOUNT] = {-1, 1, -1, 2, 3, 4, 5, 6, 7};

int ClassFades[CLASSCOUNT][5] =
{
    {255, 255, 0,   0.1, 5},
    {255, 255, 0,   0.1, 5},
    {255, 255, 0,   0.1, 5},
    {255, 255, 0,   0.1, 5},
    {255, 255, 0,   0.1, 5},
    {255, 255, 0,   0.1, 5},
    {0,   255, 0,   0.4, 8},
    {255, 255, 0,   0.1, 5},
};

int SlotItems[SLOTCOUNT] = 
{
    "",
    "GotWeapon0",
    "",
    "GotWeapon2",
    "GotWeapon3",
    "GotWeapon4",
    "GotWeapon5",
    "GotWeapon6",
    "GotWeapon7",
};

int LMSItems[CLASSCOUNT] =
{
    "",
    "",
    "PortGhostly",
    "WolfenAmmoCrate",
    "",
    "",
    "",
    "",
};

int UnloadRemove[UNLOADCOUNT] =
{
    "TomeOfPowerCooldown", "DiscOfRepulsionCooldown", "RainTracker", 
    "DukeTauntCooldown", "MaceAmmo", "MacePowered", "Mace", "DukeFlying", 
    "DukePetJack", "DukeVision", "DukeReady", "DukeWaiting", "Infrared", 
    "SynthFireActive", "SynthFireLeft", "SynthFireRight", "LevelLimiter", 
    "LeftReload", "RightReload", "UsingSMG", "UsingFists", "UsingPistols", 
    "UsingShotguns", "UsingFusionPistol", "UsingAssaultRifle", "UsingSpanker", 
    "UsingToasty", "UsingAlienWeapon", "UsingAlienWeapon2", "UsingWMC", 
    "UsingDualPistols", "UsingDualShotguns", "InvasionDualShottyCheck",
    "QuakeQuadTimer", "QuakeRegenTimer", "QuakeInvisTimer",
};

int PowerOutVols[5] = {96, 104, 112, 120, 127};

int Tipboxes[CLASSCOUNT][TIPCOUNT] =
{
    {"DOOMTIP1", "DOOMTIP2"},
    {"CHEXTIP1", "CHEXTIP2"},
    {"HERETIP1", "HERETIP2"},
    {"WOLFTIP1", "WOLFTIP2"},
    {"HEXNTIP1", "HEXNTIP2"},
    {"DUKETIP1", "DUKETIP2"},
    {"MARATIP1", "MARATIP2"},
    {"QUAKTIP1", "QUAKTIP2"},
};


int DMTipboxes[CLASSCOUNT][TIPCOUNT] =
{
    {"", ""},
    {"", "CHEXTIP3"},
    {"", "HERETIP3"},
    {"", "WOLFTIP3"},
    {"", "HEXNTIP3"},
    {"", "DUKETIP3"},
    {"", "MARATIP3"},
    {"", "QUAKTIP3"},
};

#define RESCOUNT 2
int ResonantItems[RESCOUNT][3] =
{
    {"UnknownResonator1", 0, 100},
    {"UnknownResonator2", 0, 100},
};


#define PDWTAKCOUNT 3

int PeopleDieParticles[CLASSCOUNT] = 
{
    "DoomguysDieParticle",
    "DoomguysDieParticle",
    "DoomguysDieParticle",
    "DoomguysDieParticle",
    "DoomguysDieParticle",
    "DoomguysDieParticle",
    "DoomguysDieParticle",
    "DoomguysDieParticle",
    /*
    "ChexguysDieParticle",
    "CorvusDiesParticle",
    "BlazkoDiesParticle",
    "PariasDiesParticle",
    "DukeguysDieParticle",
    "VidBoisDieParticle",
    "RangersDieParticle",
    */
};

int PeopleDiePulses[CLASSCOUNT] = 
{
    "DoomguysDiePulse",
    "DoomguysDiePulse",
    "DoomguysDiePulse",
    "DoomguysDiePulse",
    "DoomguysDiePulse",
    "DoomguysDiePulse",
    "DoomguysDiePulse",
    "DoomguysDiePulse",
    /*
    "ChexguysDiePulse",
    "CorvusDiesPulse",
    "BlazkoDiesPulse",
    "PariasDiesPulse",
    "DukeguysDiePulse",
    "VidBoisDiePulse",
    "RangersDiePulse",
    */
};

int PeopleDieBlasts[CLASSCOUNT] = 
{
    "DoomguyDiesWhenHeIsKilled",
    "ChexguyDiesWhenHeIsKilled",
    "CorvusDiesWhenHeIsKilled",
    "BlazkoDiesWhenHeIsKilled",
    "PariasDiesWhenHeIsKilled",
    "DukeDiesWhenHeIsKilled",
    "ManathorDiesWhenHeIsKilled",
    "RangerDiesWhenHeIsKilled",
};

int PDWTAKItems[PDWTAKCOUNT][2] = 
{
    {"InvPeopleDie",  "InvPeopleZorch"},
    {"InvPeopleDie2", "InvPeopleZorch2"},
    {"InvPeopleDie3", "InvPeopleZorch3"},
};

#define ARMORCOUNT  5

#define ARMOR_GREEN             0
#define ARMOR_YELLOW            1
#define ARMOR_BLUE              2
#define ARMOR_BLUE_WITHYELLOW   3
#define ARMOR_RED               4

int ArmorPickups[ARMORCOUNT] = 
{
    "GreenArmorPickup",
    "YellowArmorPickup",
    "BlueArmorPickup",
    "RedArmor",
};

int ArmorItems[ARMORMODES][ARMORCOUNT][2] = 
{
    {
        {"ArmorPack1",          100},
        {"SamsaraYellowArmor",  150},
        {"ArmorPack2",          200},
        {"ArmorPack2",          200},
        {"ArmorPack3",          250},
    },
    {
        {"QuakeGreenArmor",     100},
        {"QuakeYellowArmor",    150},
        {"QuakeYellowArmor",    150},
        {"QuakeRedArmor",       200},
        {"SamsaraSilverArmor",  250},
    },
    {
        {"QuakeYellowArmor",    150},
        {"QuakeYellowArmor",    150},
        {"QuakeRedArmor",       200},
        {"QuakeRedArmor",       200},
        {"SamsaraSilverArmor",  250},
    },
    {
        {"MarathonGreenArmor",  100},
        {"MarathonYellowArmor", 150},
        {"MarathonBlueArmor",   200},
        {"MarathonRedArmor",    300},
        {"MarathonRedArmor",    300},
    },
    {
        {"NoArmor",             0},
        {"NoArmor",             0},
        {"NoArmor",             0},
        {"NoArmor",             0},
        {"NoArmor",             0},
    }
};

int ArmorModeStates[ARMORMODES][4] = 
{
    {"Armor1", "Armor1_Drop", "Armor1_WithYellow", "Armor1_YellowDrop"},
    {"Armor2", "Armor2_Drop", "Armor2_WithYellow", "Armor2_YellowDrop"},
    {"Armor3", "Armor3_Drop", "Armor3_WithYellow", "Armor3_YellowDrop"},
    {"Armor4", "Armor4_Drop", "Armor4_WithYellow", "Armor4_YellowDrop"},
    {"Armor5", "Armor5_Drop", "Armor5_WithYellow", "Armor5_YellowDrop"},
};

int ArmorModeNames[ARMORMODES] = 
{
    "Normal",
    "Quake",
    "Quake (old)",
    "Marathon",
    "None",
};

int PunchDrunkItems[CLASSCOUNT][3] = 
{
    {"DoomguyPunchDrunk",   "DoomguyPunchSaw",  "DoomguyPunchUnique"},
    {"ChexguyPunchDrunk",   "ChexguyPunchSaw",  "ChexguyPunchUnique"},
    {"CorvusPunchDrunk",    "CorvusPunchSaw",   "CorvusPunchUnique"},
    {"BlazkoPunchDrunk",    "BlazkoPunchSaw",   "BlazkoPunchUnique"},
    {"PariasPunchDrunk",    "PariasPunchSaw",   "PariasPunchUnique"},
    {"DukePunchDrunk",      "DukePunchSaw",     "DukePunchUnique"},
    {"ManathorPunchDrunk",  "ManathorPunchSaw", "ManathorPunchUnique"},
    {"RangerPunchDrunk",    "RangerPunchSaw",   "RangerPunchUnique"},
};


int ChangelogString = 
" FROM 0.28 TO 0.29\n\
========================\n\
\cfGAMEPLAY:\c-\n\
- Double-tapping Duke's jetpack to try and extend the lifespan of it now actually dramatically decreases its lifespan.\n\
- sv_shotgunstart is now respected.\n\
- The Fusion Pistol overcharge now explodes again.\n\
- The first Fusion Pistol beep is now silent, and you can now charge for one more second.\n\
- Ranger now has invisibility to Quake's Ring of Shadows! He's much less visible when picking up a blursphere (10% visibility), and his weapons don't alert monsters.\n\
- Everyone Frozen states has been completely reworked. This should fix all of the strange ice-related bugs that keep popping up in DM.\n\
- samsara_peoplediewhentheyarekilled has been added as an optional cvar. What does it do? Well, call it a Party Mode...\n\
- Parias' Wraithverge has been converted to Decorate. This allows for different behavior in co-op and DM, at the expense of the ghosts now acting like complete idiots. In DM, the ghosts do dramatically more damage, move faster as long as the player is in sight, and spawn with a large explosion.\n\
- samsara_chainsawstart has been added. If 1, gives you your chainsaw weapon. If 2, gives you your chainsaw weapon, plus full ammo.\n\
- samsara_uniquestart has been added. See the wiki for details.\n\
- A melee-only mode has been added! If it's on, all your non-fist weapons are taken, you can't pick up weapons, and uniques change to effects more useful in Punchdrunk. Use 'samsara_punchdrunk 1' to turn it on.\n\
- Ranger's Dissolution of Eternity ammo has been completely reworked. One of the biggest problems Ranger had was that sheer luck determined whether or not he got to use Cells or not--now, he practically lives off them.\n\
- Dark Imps, Hectebi, and SSGGuys now shoot through spooky ghosts.\n\
- Corvus' cooldown for the time bombs has been reduced from 70 tics to 15.\n\
- The Fusion Pistol can now actually bypass player invulnerability, as it should.\n\
- B.J. has had his Clip ammo doubled upon picking up Ammo 2.\n\
- B.J. Blazkowic's Machine Gun now gives 20 of ammo 1 on pickup, rather than 6.\n\
- samsara_armormode has been added. The modes available are Normal, Quake, Quake (old), Marathon, and None.\n\
- You can now see what settings the server is using!\n\
- In DM, the Tomed Firemace has traded in the ability to home on people for the ability to bounce off walls.\n\
- Thanks to TehVappy50, Corvus now has the ability to pick up and carry around spheres of various kinds in his inventory. A massive buff on Skulltag maps!\n\
- Chexter's screen now flashes green when slimed or flemmed.\n\
- Duke's Riot Shotgun has been given a cleaned-up spread, more reminiscent of Duke 3D.\n\
- Duke Nukem's Mighty Foot now affects players! Slam the boot into them, they go careening backwards wildly.\n\
\cfBUGFIXES:\c-\n\
- Ranger's Spectral weapons no longer use the old DoE ammo switching style.\n\
- Spectral Laser Cannon no longer flickers.\n\
- Spectral Thunderbolt no longer uses cells.\n\
- Spectral LAZ Device zorches properly.\n\
- Ranger no longer keeps his quadded face when he dies.\n\
- (ZDoom) The SO's lightamp now does BlueMap, as expected. BlueMap still broken in Zandroland.\n\
- The Thunderbolt discharge now actually escapes the sector it's in. Apparently low-radius projectiles don't explode properly.\n\
- sv_degeneration disables Ranger's degeneration, so he isn't hit with both of them.\n\
- Corvus' Tomed Phoenix Rod was getting confused with the Spectral Tomed Phoenix Rod. This was fixed.\n\
- The Quad Damage no longer plays weapons/sigil when powering down.\n\
- The SO's Marathon 2 Alien Weapon can no longer be dropped.\n\
- The Hectebus no longer gets locked in firing.\n\
- Spectre 2 and 4 in Strife are no longer immune to every non-Spectral weapon.\n\
- Duke burning and freezing now causes him to lose his inventory stuff and triggers the 'press any button to respawn' message.\n\
- Duke's ballgag now respects his death quip.\n\
- The SSG zombie's drop now actually exists.\n\
- The TOZT no longer lifts up instantly and gains an ammo from nowhere on lift-up.\n\
- Unique items no longer spawn in LMS and TLMS.\n\
- Duke passing the pipe no longer results in a clack.\n\
- The Barons of Hell, Mancubi, and Arachnotrons no longer forget to trigger A_BossDeath when frozen in Survival mode. What a specific bug!\n\
- The Wings of Wrath have had their gold tint removed, as Hexen made it permanent.\n\
- Freezing the Programmer no longer breaks Strife.\n\
- The Programmer now gives a dummy Sigil on death, which should fix Strife's map progression.\n\
\cfPOLISH:\c-\n\
- The changelog has been changed from a graphic to a textdump.\n\
- Further separated Samsara character scripts from other non-Samsara chars.\n\
- When using vanilla animations, Doomguy's pistol muzzle flash is no longer wildly to the side.\n\
- Hexen armor is now brown, and red armor is actually red again.\n\
- You can now see what armor type you have when using hud_althud, as you would expect.\n\
- samsara_cl_noadditivepickups has been added, for those whose bloom makes it near impossible to see the things.\n\
- Duke pipebombs and devastators now make two explosion sounds; one local and loud, one global and quiet (with the pipebomb global sound limited).\n\
- The Quad Damage now makes the 'no item' sound from Quake 3 if it's used when it's recharging.\n\
- Duke's Freezethrower sprites have been adjusted for better Widescreen compatibility.\n\
- The SO has been given generic color painflashes for easier add-on compatibility.\n\
- BlackFish has provided cleaner rocketlauncher sprites for Quakeguy.\n\
- Weapon pickup messages now take into account msg0color.\n\
- Duke's RPG has been tweaked in widescreen, thanks to Turbo.\n\
- Thanks to BlackFish, the Firestorm now has widescreen sprites!\n\
- Whoops! Turns out the proper message was MIGHTY FOOT ENGAGED, not Mighty Boot. Fixed.\n\
- Skulltag monsters now bleed/gib their proper colors.\n\
- The Security Officer's fists now properly alternate when holding both fire buttons.\n\
- B.J. Blazkowicz now has a Dukegib sequence, thanks to Peter Bark's sprites.\n\
- Due to popular request, the Chicken player now has its own unique HUD.\n\
- The Icon of Sin now has a boss speech.\n\
- For those who don't like the boss speeches, samsara_nomonologues has been implemented as a cvar.\n\
- Sounds have been arranged and sorted into neat little folders.\n\
- Ranger now has a sound for hitting things with the axe, due to popular request.\n\
- Ranger now has a burndeath sequence based off the simplistic burndeaths from Quake mods.";
