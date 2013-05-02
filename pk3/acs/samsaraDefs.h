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
#define ARMORMODES          2

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
samsara_lmsunique 0/1: Toggles whether players get their unique in LMS.";

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

int ArmorModeStates[ARMORMODES][2] = 
{
    {"Armor1", "Armor1_Drop"},
    {"Armor2", "Armor2_Drop"},
};

int ArmorModeNames[ARMORMODES] = 
{
    "Normal",
    "Quake",
};

int ChangelogString = 
" FROM 0.28 TO 0.29\n\
========================\n\
- Further separated Samsara character scripts from other non-Samsara chars.\n\
- Ranger's Spectral weapons no longer use the old DoE ammo switching style.\n\
- Spectral Laser Cannon no longer flickers.\n\
- Spectral Thunderbolt no longer uses cells.\n\
- Spectral LAZ Device zorches properly.\n\
- Ranger no longer keeps his quadded face when he dies.\n\
- (ZDoom) The SO's lightamp now does BlueMap, as expected. BlueMap still broken in Zandroland.\n\
- The Thunderbolt discharge now actually escapes the sector it's in. Apparently low-radius projectiles don't explode properly.\n\
- sv_degeneration disables Ranger's degeneration, so he isn't hit with both of them.\n\
- Corvus' Tomed Phoenix Rod was getting confused with the Spectral Tomed Phoenix Rod. This was fixed.\n\
- Double-tapping Duke's jetpack to try and extend the lifespan of it now actually dramatically decreases its lifespan.\n\
- Spectre 2 and 4 are no longer immune to every non-Spectral weapon.\n\
- When using vanilla animations, Doomguy's pistol muzzle flash is no longer wildly to the side.\n\
- Hexen armor is now brown, and red armor is actually red again.\n\
- You can now see what armor type you have when using hud_althud, as you would expect.\n\
- Yellow armor is in for any people mapping for Samsara. It gives 150 AP at 40% protection, and its DoomEdNum is 28636.\n\
- samsara_cl_noadditivepickups has been added, for those whose bloom makes it near impossible to see the things.\n\
- sv_degeneration disables Ranger's degeneration, so he isn't hit with both of them.\n\
- sv_shotgunstart is now respected.\n\
- Duke pipebombs and devastators now make two explosion sounds; one local and loud, one global and quiet (with the pipebomb global sound limited).\n\
- The Fusion Pistol overcharge now explodes again.\n\
- The first Fusion Pistol beep is now silent, and you can now charge for one more second.\n\
- The Quad Damage no longer plays weapons/sigil when powering down.\n\
- The Quad Damage now makes the 'no item' sound from Quake 3 if it's used when it's recharging.\n\
- Ranger now has invisibility to Quake's Ring of Shadows! He's much less visible when picking up a blursphere (10% visibility), and his weapons don't alert monsters.\n\
- B.J. Blazkowic's Machine Gun now gives 20 of ammo 1 on pickup, rather than 6.\n\
- Duke's Freezethrower sprites have been adjusted for better Widescreen compatibility.\n\
- The changelog has been changed from a graphic to a textdump.\n\
- The SO has been given generic color painflashes for easier add-on compatibility.\n\
- The SO no longer gains one cell upon raising the TOZT again.\n\
- Duke burning and freezing now causes him to lose his inventory stuff and triggers the 'press any button to respawn' message.\n\
- The SO's Marathon 2 Alien Weapon is now undroppable.\n\
- The Hectebus no longer gets locked in firing.\n\
- Weapon pickup messages now take into account msg0color.\n\
- BlackFish has provided cleaner rocketlauncher sprites for Quakeguy.\n\
- B.J. has had his Clip ammo doubled upon picking up Ammo 2.\n\
- Duke's ballgag now respects his death quip.\n\
- The SSG zombie's drop now actually exists.\n\
- Duke's RPG has been tweaked in widescreen, thanks to Turbo.\n\
- The TOZT no longer lifts up instantly and gains an ammo from nowhere on lift-up.\n\
- Everyone Frozen states has been completely reworked. This should fix all of the strange ice-related bugs that keep popping up in DM.\n\
- samsara_peoplediewhentheyarekilled has been added as an optional cvar. What does it do? Well, call it a Party Mode...\n\
- Parias' Wraithverge has been converted to Decorate. This allows for different behavior in co-op and DM, at the expense of the ghosts now acting like complete idiots. In DM, the ghosts do dramatically more damage, move faster as long as the player is in sight, and spawn with a large explosion.\n\
- Thanks to BlackFish, the Firestorm now has widescreen sprites!\n\
- samsara_armormode has been added. 0 means nothing changes, 1 means you have more Quake style armor pickups.\n\
- You can now see what settings the server is using!\n\
- Unique items no longer spawn in LMS and TLMS.";
