#define WEPFLAGS_GOTWEAPON  1
#define WEPFLAGS_WEAPONSTAY 2

#define CHOICECOUNT         5
#define CHOICECOUNT_U       (UNIQUECOUNT*2)

#define S_WEP               0
#define S_AMMO1             1
#define S_AMMO2             2
#define S_CHECKITEM         3
#define S_CHECKFAILITEM     4

#define U_UNIQUE1           0
#define U_AMMO1             1
#define U_UNIQUE2           2
#define U_AMMO2             3

#define SLOT_UNIQUE        -1
#define SLOT_FIST           0
#define SLOT_CHAINSAW       1
#define SLOT_PISTOL         2
#define SLOT_SHOTGUN        3
#define SLOT_SUPERSHOTGUN   4
#define SLOT_SSG            4
#define SLOT_CHAINGUN       5
#define SLOT_ROCKETLAUNCHER 6
#define SLOT_PLASMARIFLE    7
#define SLOT_BFG9000        8
#define SLOT_BFG            8
#define SLOT_PUNCHDRUNKSAW  9

int ClassUniques[CLASSCOUNT][CHOICECOUNT_U] = 
{
    {"DoomguyBerserker",    "",                 "", ""},
    {"ChexSoulsphere",      "",                 "", ""},
    {"ArtiEgg2",            "",                 "", ""},
    {"WolfExtraLife",       "",                 "", ""},
    {"PortMysticAmbit",     "",                 "", ""},
    {"DukePortJetpack",     "DukeJetpackFuel",  "", ""},
    {"Alien Weapon",        "UnknownAmmo",      " Alien Weapon ", "UnknownAmmo2"},
    {"Laser Cannon",        "Cell",             "", ""},
};

int UniqueMaxes[CLASSCOUNT][CHOICECOUNT_U] = 
{
    {0,     0,      0,  0},
    {0,     0,      0,  0},
    {16,    0,      0,  0},
    {9,     0,      0,  0},
    {16,    0,      0,  0},
    {1,     100,    0,  0},
    {1,     50,     1,  50},
    {1,     0,      0,  0},
};

int UniqueCounts[CLASSCOUNT][CHOICECOUNT_U] =
{
    {-1,    -1,     -1, -1},
    {-1,    -1,     -1, -1},
    {-1,    -1,     -1, -1},
    {-1,    -1,     -1, -1},
    {-1,    -1,     -1, -1},
    {-1,    -1,     -1, -1},
    {-1,    -1,     -1, -1},
    {-1,     0,     -1, -1},
};

int UniqueScripts[CLASSCOUNT] = {0, 0, 0, 0, 0, 0, SAMSARA_MARATHON, 0};

int ClassWeapons[CLASSCOUNT][SLOTCOUNT][CHOICECOUNT] = 
{
    {   // Doomguy
        {" Fist ",                          "",             "",             "", ""},
        {" Chainsaw ",                      "",             "",             "", ""},
        {" Pistol ",                        "",             "",             "", ""},
        {" Shotgun ",                       "Shell",        "",             "", ""},
        {"Super Shotgun",                   "Shell",        "",             "", ""},
        {" Chaingun ",                      "Clip",         "",             "", ""},
        {"Rocket Launcher",                 "RocketAmmo",   "",             "", ""},
        {"Plasma Rifle",                    "Cell",         "",             "", ""},
        {"B.F.G. 9000",                     "Cell",         "",             "", ""},
        {" Chainsaw ",                      "",             "",             "", ""},
    },

    {   // Chexguy
        {" Bootspoon ",                     "",             "",             "", ""},
        {"Super Bootspork",                 "",             "",             "", ""},
        {"Mini-Zorcher",                    "",             "",             "", ""},
        {"Large Zorcher",                   "Shell",        "",             "", ""},
        {"Super Large Zorcher",             "Shell",        "",             "", ""},
        {"Rapid Zorcher",                   "Clip",         "",             "", ""},
        {"Zorch Propulsor",                 "RocketAmmo",   "",             "", ""},
        {"Phasing Zorcher",                 "Cell",         "",             "", ""},
        {"LAZ Device",                      "Cell",         "",             "", ""},
        {"Super Bootspork",                 "",             "",             "", ""},
    },

    {   // Corvus
        {" Staff ",                         "",             "",             "", ""},
        {"Gauntlets of the Necromancer",    "",             "",             "", ""},
        {"Elven Wand",                      "",             "",             "", ""},
        {" Firemace ",                      "Shell",        "",             "", ""},
        {"Ethereal Crossbow",               "Shell",        "",             "", ""},
        {"Dragon Claw",                     "Clip",         "",             "", ""},
        {"Phoenix Rod",                     "RocketAmmo",   "",             "", ""},
        {"Hellstaff",                       "Cell",         "",             "", ""},
        {"PortTomeCoop",                    "",             "",             "", ""},
        {"Gauntlets of the Necromancer",    "",             "",             "", ""},
    },

    {   // B.J.
        {"Knife",                           "",             "",             "", ""},
        {"BJSuperKnife",                    "",             "",             "", ""},
        {"Luger",                           "",             "",             "", ""},
        {"Machine Gun",                     "Clip",         "",             "", ""},
        {"Machine Gun",                     "Clip",         "",             "", ""},
        {"  Chaingun  ",                    "Clip",         "",             "", ""},
        {" Rocket Launcher ",               "RocketAmmo",   "",             "", ""},
        {" Flamethrower ",                  "Cell",         "",             "", ""},
        {"Spear of Destiny",                "Cell",         "",             "", ""},
        {"BJUberKnife",                     "",             "",             "", ""},
    },

    {   // Parias
        {"Mace of Contrition",              "",             "",             "", ""},
        {"PortFlechette",                   "",             "",             "", ""},
        {"Sapphire Wand",                   "",             "",             "", ""},
        {"Frost Shards",                    "Clip",         "",             "", ""},
        {"Timon's Axe",                     "Clip",         "",             "", ""},
        {"Serpent Staff",                   "Clip",         "",             "", ""},
        {"Hammer of Retribution",           "Cell",         "",             "", ""},
        {"Firestorm",                       "Cell",         "",             "", ""},
        {"Wraithverge",                     "Cell",         "",             "", ""},
        {"PortFlechette",                   "",             "",             "", ""},
    },

    {   // Duke
        {"Mighty Boot",                     "",             "",             "", ""},
        {"Pipebombs",                       "RocketAmmo",   "",             "", ""},
        {"Glock 17",                           "",             "",             "", ""},
        {"  Shotgun  ",                     "Shell",        "",             "", ""},
        {"Explosive Shotgun",               "RocketAmmo",   "",             "", ""},
        {"Chaingun Cannon",                 "Clip",         "",             "", ""},
        {"RPG",                             "RocketAmmo",   "",             "", ""},
        {"Freezethrower",                   "Cell",         "",             "", ""},
        {"Devastator",                      "Cell",         "",             "", ""},
        {"CalmYoLegsDuke",                  "",             "",             "", ""},
    },

    {   // Security Officer
        {"Steel Knuckles",                  "",             "",             "",                 ""},
        {"KKV-7 SMG Flechette",             "Clip",         "",             "CanDualPistols",   ""},
        {".44 Magnum Mega Class A1",        "",             "",             "",                 ""},
        {"WSTE-M5 Combat Shotgun",          "Shell",        "",             "CanDualShotties",  "LevelLimiter"},
        {"Fusion Pistol",                   "FusionBullet", "Cell",         "",                 ""},
        {"MA-75B Assault Rifle",            "RifleBullet",  "Clip",         "",                 ""},
        {"SPNKR-XP SSM Launcher",           "SpankerAmmo",  "RocketAmmo",   "",                 ""},
        {"TOZT-7 Napalm Unit",              "NapalmInTank", "Cell",         "",                 ""},
        {"ONI-71 Wave Motion Cannon",       "Cell",         "RocketAmmo",   "",                 ""},
        {"Tech.50 Pacifier",                "",             "",             "",                 ""},
    },
    {   // Ranger
        {"Axe",                             "",             "",             "", ""},
        {"Mjolnir",                         "",             "",             "", ""},
        {"Single Shotgun",                  "",             "",             "", ""},
        {"Double Shotgun",                  "Shell",        "",             "", ""},
        {"Grenade Launcher",                "RocketAmmo",   "",             "", ""},
        {"Nailgun",                         "Clip",         "",             "", ""},
        {"  Rocket Launcher  ",             "RocketAmmo",   "",             "", ""},
        {"Super Nailgun",                   "Clip",         "",             "", ""},
        {"Thunderbolt",                     "Cell",         "",             "ForceCheck", ""},
        {"Gauntlet",                        "",             "",             "", ""},
    },
};

int ClassScripts[CLASSCOUNT][SLOTCOUNT] = 
{
    {0,0,                   0,  0,0,0,0,0,0,0},
    {0,0,                   0,  0,0,0,0,0,0,0},
    {0,0,                   0,  0,0,0,0,0,0,0},
    {0,0,                   0,  0,0,0,0,0,0,0},
    {0,0,                   0,  0,0,0,0,0,0,0},
    {0,0,                   0,  0,0,0,0,0,0,0},
    {0,SAMSARA_MARATHON,    0,  SAMSARA_MARATHON,SAMSARA_MARATHON,SAMSARA_MARATHON,SAMSARA_MARATHON,SAMSARA_MARATHON,SAMSARA_MARATHON,0},
    {0,0,                   0,  0,0,0,0,0,SAMSARA_QUAKE,0},
};
