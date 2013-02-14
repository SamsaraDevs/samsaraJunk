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

#define SAMSARA_CONFIRMCLASS        206
#define SAMSARA_DECORATE            215
#define SAMSARA_GIVEWEAPON          229
#define SAMSARA_GIVEUNIQUE          231
#define SAMSARA_CLIENT_WEAPONPICKUP 616
#define SAMSARA_CLIENT_UNIQUEPICKUP 633
#define SAMSARA_MARATHON            609
#define SAMSARA_RESONATE            608
#define SAMSARA_MEGAHEALTH          249

#define SAMSARA_TIPBOX              300
#define SAMSARA_TIPBOX_CLIENT       301

#define SAMSARA_PUKE                226
#define SAMSARA_PUKE_CLIENT         227
#define SAMSARA_OPEN_CLIENT         548
#define SAMSARA_ENTER_CLIENT        521
#define SAMSARA_DISCONNECT_CLIENT   522
#define SAMSARA_CLIENT_CLASS        228
#define SAMSARA_RECOIL              312

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

#define UNLOADCOUNT 35

#define P_COUNT 2
#define P_QUAD  0
#define P_REGEN 1

#define QUAD_THRESHOLD      1050

#define REGEN_CENTER_X      0.953125    // 610 / 640
#define REGEN_CENTER_Y      0.6875      // 330 / 480

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
    "QuakeQuadTimer", "QuakeRegenTimer",
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
    "DoomguyDiesWhenHeIsKilled",
    "DoomguyDiesWhenHeIsKilled",
    "DoomguyDiesWhenHeIsKilled",
    "DoomguyDiesWhenHeIsKilled",
    "DoomguyDiesWhenHeIsKilled",
    "DoomguyDiesWhenHeIsKilled",
    "DoomguyDiesWhenHeIsKilled",
};
