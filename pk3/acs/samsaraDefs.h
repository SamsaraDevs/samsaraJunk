#define SAMSARA_ENTER               213
#define SAMSARA_RESPAWN             218
#define SAMSARA_DEATH               219
#define SAMSARA_OPEN                223
#define SAMSARA_SPAWN               224
#define SAMSARA_DISCONNECT          217
#define SAMSARA_WOLFMOVE            273

#define SAMSARA_CONFIRMCLASS        206
#define SAMSARA_DECORATE            215
#define SAMSARA_GIVEWEAPON          229
#define SAMSARA_CLIENT_WEAPONPICKUP 216
#define SAMSARA_MARATHON            209

#define SAMSARA_PUKE                226
#define SAMSARA_OPEN_CLIENT         205
#define SAMSARA_ENTER_CLIENT        221
#define SAMSARA_DISCONNECT_CLIENT   222
#define SAMSARA_CLIENT_CLASS        228

#define LMSMODES            6

#define CLASSCOUNT          7
#define SLOTCOUNT           9
#define CHOICECOUNT         6
#define MSGCOUNT            1

#define CLASS_DOOMGUY   0
#define CLASS_CHEX      1
#define CLASS_CORVUS    2
#define CLASS_WOLFEN    3
#define CLASS_HEXEN     4
#define CLASS_DUKE      5
#define CLASS_MARATHON  6

#define S_WEP               0
#define S_AMMO1             1
#define S_AMMO2             2
#define S_CHECKITEM         3
#define S_CHECKFAILITEM     4
#define S_GIVESCRIPT        5

#define SPEED_FORWARD       15
#define SPEED_SIDE          13

#define WEPFLAGS_GOTWEAPON  1
#define WEPFLAGS_WEAPONSTAY 2

int HELPSTR = 
"Welcome to the Wheel of Samsara! There are a few optional RCon commands you may want to consider.\n\
samsara_banjetpack 0/1: Toggles whether Duke is allowed to pick up his jetpack or not.\n\
samsara_sogravity 0/1: Toggles whether the Security Officer operates off Marathon or Doom gravity.\n\
samsara_permault 0/1: Toggles whether the Weapon VII is persistent or vanishes on pickup.\n\
samsara_lmslife 0/1/2/3/4/5: Affects how much health/armor people have on LMS spawn/respawn.\n\
samsara_lmsult 0/1: Toggles whether or not players get their VII in LMS mode.";

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
};

int PickupStates[CLASSCOUNT][3] = 
{
    {"Doomguy", "DoomFull",     "DoomEmpty"},
    {"Chexguy", "ChexFull",     "ChexEmpty"},
    {"Heretic", "HereticFull",  "HereticEmpty"},
    {"Wolfguy", "WolfFull",     "WolfEmpty"},
    {"Hexen",   "HexenFull",    "HexenEmpty"},
    {"Dukeguy", "DukeFull",     "DukeEmpty"},
    {"Marathon","MarathonFull", "MarathonEmpty"},
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

int ClassWeapons[CLASSCOUNT][SLOTCOUNT][CHOICECOUNT] = 
{
    {   // Doomguy
        {" Fist ",                          "",             "",             "", "", 0},
        {" Chainsaw ",                      "",             "",             "", "", 0},
        {" Pistol ",                        "",             "",             "", "", 0},
        {" Shotgun ",                       "AmmoShell",    "",             "", "", 0},
        {"Super Shotgun",                   "AmmoShell",    "",             "", "", 0},
        {" Chaingun ",                      "Clip",         "",             "", "", 0},
        {"Rocket Launcher",                 "RocketAmmo",   "",             "", "", 0},
        {"Plasma Rifle",                    "Cell",         "",             "", "", 0},
        {"B.F.G. 9000",                     "Cell",         "",             "", "", 0},
    },

    {   // Chexguy
        {" Bootspoon ",                     "",             "",             "", "", 0},
        {"Super Bootspork",                 "",             "",             "", "", 0},
        {"Mini-Zorcher",                    "",             "",             "", "", 0},
        {"Large Zorcher",                   "AmmoShell",    "",             "", "", 0},
        {"Super Large Zorcher",             "AmmoShell",    "",             "", "", 0},
        {"Rapid Zorcher",                   "Clip",         "",             "", "", 0},
        {"Zorch Propulsor",                 "RocketAmmo",   "",             "", "", 0},
        {"Phasing Zorcher",                 "Cell",         "",             "", "", 0},
        {"LAZ Device",                      "Cell",         "",             "", "", 0},
    },

    {   // Corvus
        {" Staff ",                         "",             "",             "", "", 0},
        {"Gauntlets of the Necromancer",    "",             "",             "", "", 0},
        {"Gold Wand",                       "",             "",             "", "", 0},
        {" Firemace ",                      "AmmoShell",    "",             "", "", 0},
        {"Ethereal Crossbow",               "AmmoShell",    "",             "", "", 0},
        {"Dragon Claw",                     "Clip",         "",             "", "", 0},
        {"Phoenix Rod",                     "RocketAmmo",   "",             "", "", 0},
        {"Hellstaff",                       "Cell",         "",             "", "", 0},
        {"PortTomeCoop",                    "",             "",             "", "", 0},
    },

    {   // B.J.
        {"Knife",                           "",             "",             "", "", 0},
        {"GotWeapon0",                      "",             "",             "", "", 0},
        {"Luger",                           "",             "",             "", "", 0},
        {"Machine Gun",                     "Clip",         "",             "", "", 0},
        {"Machine Gun",                     "Clip",         "",             "", "", 0},
        {"  Chaingun  ",                    "Clip",         "",             "", "", 0},
        {" Rocket Launcher ",               "RocketAmmo",   "",             "", "", 0},
        {" Flamethrower ",                  "Cell",         "",             "", "", 0},
        {"Spear of Destiny",                "Cell",         "",             "", "", 0},
    },

    {   // Parias
        {"Mace of Contrition",              "",             "",             "", "", 0},
        {"PortFlechette",                   "",             "",             "", "", 0},
        {"Sapphire Wand",                   "",             "",             "", "", 0},
        {"Frost Shards",                    "AmmoShell",    "",             "", "", 0},
        {"Timon's Axe",                     "AmmoShell",    "",             "", "", 0},
        {"Serpent Staff",                   "Clip",         "",             "", "", 0},
        {"Hammer of Retribution",           "RocketAmmo",   "",             "", "", 0},
        {"Firestorm",                       "Cell",         "",             "", "", 0},
        {"Wraithverge",                     "Cell",         "",             "", "", 0},
    },

    {   // Duke
        {"Mighty Boot",                     "",             "",             "", "", 0},
        {"Pipebombs",                       "RocketAmmo",   "",             "", "", 0},
        {"M1911",                           "",             "",             "", "", 0},
        {"  Shotgun  ",                     "AmmoShell",    "",             "", "", 0},
        {"Explosive Shotgun",               "RocketAmmo",   "",             "", "", 0},
        {"Chaingun Cannon",                 "Clip",         "",             "", "", 0},
        {"RPG",                             "RocketAmmo",   "",             "", "", 0},
        {"Freezethrower",                   "Cell",         "",             "", "", 0},
        {"Devastator",                      "Cell",         "",             "", "", 0},
    },

    {   // Security Officer
        {"Steel Knuckles",                  "",             "",             "",                 "",             0},
        {"KKV-7 SMG Flechette",             "Clip",         "",             "CanDualPistols",   "",             SAMSARA_MARATHON},
        {".44 Magnum Mega-Class A1",        "",             "",             "",                 "",             0},
        {"WSTE-M5 Combat Shotgun",          "AmmoShell",    "",             "CanDualShotties",  "LevelLimiter", SAMSARA_MARATHON},
        {"Fusion Pistol",                   "FusionBullet", "Cell",         "",                 "",             0},
        {"MA-75B Assault Rifle",            "RifleBullet",  "Clip",         "",                 "",             0},
        {"SPNKR-XP SSM Launcher",           "SpankerAmmo",  "RocketAmmo",   "",                 "",             0},
        {"TOZT-7 Napalm Unit",              "NapalmInTank", "Cell",         "",                 "",             0},
        {"ONI-71 Wave Motion Cannon",       "Cell",         "RocketAmmo",   "",                 "",             0},
    }
};

int ClassPickupMessages[CLASSCOUNT][SLOTCOUNT][MSGCOUNT] = 
{
        {
         {"I PUNCH YOO"},
         {"A chainsaw! Find some meat!"},
         {"Picked up a pistol."},
         {"You got the Shotgun!"},
         {"You got the Super Shotgun!"},
         {"You got the Chaingun!"},
         {"You got the Rocket Launcher!"},
         {"You got the Plasma Rifle!"},
         {"You got the BFG9000! Oh, yes."},
    },
    {
         {"I SCOOP YOO"},
         {"A Super Bootspork! Find some milk!"},
         {"Picked up a Mini Zorcher."},
         {"You got the Large Zorcher!"},
         {"You got the Super Large Zorcher!"},
         {"You got the Rapid Zorcher!"},
         {"You got the Zorch Propulsor!"},
         {"You got the Phasing Zorcher!"},
         {"You got the LAZ Device! Woot!"},
    },
    {
         {"I JAB YOO"},
         {"Gauntlets of the Necromancer! Find some disciples!"},
         {"Picked up a gold wand."},
         {"You got the Firemace!"},
         {"You got the Ethereal Crossbow!"},
         {"You got the Dragon Claw!"},
         {"You got the Phoenix Rod!"},
         {"You got the Hellstaff!"},
         {"You got the Tome of Power! Ahahaha..."},
    },
    {
         {"I STAB YOO"},
         {"Your knife developed teeth! Find some Nazis!"},
         {"Picked up a Luger."},
         {"You got the Machine Gun!"},
         {"You got the Machine Gun!"},
         {"You got the Chaingun!"},
         {"You got the Rocket Launcher!"},
         {"You got the Flamethrower!"},
         {"You got the Spear of Destiny! Heaven awaits."},
    },
    {
         {"I WAK YOO"},
         {"Flechettes! Find some centaurs!"},
         {"Picked up a sapphire wand."},
         {"You got the Frost Shards!"},
         {"You got Timon's Axe!"},
         {"You got the Serpent Staff!"},
         {"You got the Hammer of Retribution!"},
         {"You got the Firestorm!"},
         {"You got the Wraithverge! Scream for me!"},
    },
    {
         {"I KICK YOO"},
         {"Pipe bombs! Find some unsuspecting saps!"},
         {"Picked up an M1911."},
         {"You got the Riot Shotgun!"},
         {"You got the Explosive Shotgun!"},
         {"You got the Chaingun Cannon!"},
         {"You got the RPG Launcher!"},
         {"You got the Freezethrower!"},
         {"You got the Devastators! Hail to the King."},
    },
    {
         {"I PUNCH YOO TOO"},
         {"A KKV-7 SMG and an extra .44 Magnum! Find some aliens!"},
         {"Picked up a magnum."},
         {"You got the WSTE-M5 Combat Shotgun!"},
         {"You got the Zeus Class Fusion Pistol!"},
         {"You got the MA-75B Assault Rifle!"},
         {"You got the SPNKR-XP SSM Launcher!"},
         {"You got the TOZT-7 Napalm Unit!"},
         {"You got the ONI-71 Wave Motion Cannon! What's that spell? Trouble."}, // since apparently this quote ain't good enuf
    },
};

int ClassPickupSounds[CLASSCOUNT][SLOTCOUNT] = 
{
    {
        "doomguy/weaponget",
        "doomguy/weaponget",
        "doomguy/weaponget",
        "doomguy/weaponget",
        "doomguy/weaponget",
        "doomguy/weaponget",
        "doomguy/weaponget",
        "doomguy/weaponget",
        "doomguy/weaponget",
    },
    {
        "chex/weaponget",
        "chex/weaponget",
        "chex/weaponget",
        "chex/weaponget",
        "chex/weaponget",
        "chex/weaponget",
        "chex/weaponget",
        "chex/weaponget",
        "chex/weaponget",
    },
    {
        "heretic/weaponget",
        "heretic/weaponget",
        "heretic/weaponget",
        "heretic/weaponget",
        "heretic/weaponget",
        "heretic/weaponget",
        "heretic/weaponget",
        "heretic/weaponget",
        "heretic/weaponget",
    },
    {
        "wolfen/knifeget",
        "wolfen/knifeget",
        "wolfen/itemget",
        "wolfen/mgunget",
        "wolfen/mgunget",
        "wolfen/weaponget",
        "wolfen/rocketget",
        "wolfen/flameget",
        "wolfen/spearget",
    },
    {
        "hexen/weaponget",
        "hexen/weaponget",
        "hexen/weaponget",
        "hexen/weaponget",
        "hexen/weaponget",
        "hexen/weaponget",
        "hexen/weaponget",
        "hexen/weaponget",
        "hexen/weaponget",
    },
    {
        "duke/weaponget",
        "duke/itemget",
        "duke/weaponget",
        "duke/mosspump2",
        "duke/xshotgunpump",
        "duke/weaponget",
        "duke/weaponget",
        "duke/weaponget",
        "duke/weaponget",
    },
    {
        "marathon/itemget",
        "marathon/itemget",
        "marathon/itemget",
        "marathon/itemget",
        "marathon/itemget",
        "marathon/itemget",
        "marathon/itemget",
        "marathon/itemget",
        "marathon/bigitemget",
    },
};
