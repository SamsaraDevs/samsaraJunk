#define SAMSARA_ENTER               213
#define SAMSARA_RESPAWN             218
#define SAMSARA_OPEN                223
#define SAMSARA_SPAWN               224
#define SAMSARA_WOLFMOVE            273

#define SAMSARA_CONFIRMCLASS        206
#define SAMSARA_DECORATE            215
#define SAMSARA_GIVEWEAPON          229
#define SAMSARA_CLIENT_WEAPONPICKUP 216

#define SAMSARA_PUKE                226
#define SAMSARA_ENTER_CLIENT        221
#define SAMSARA_DISCONNECT_CLIENT   222
#define SAMSARA_CLIENT_CLASS        228

#define LMSMODES            6

#define CLASSCOUNT          7
#define SLOTCOUNT           9
#define CHOICECOUNT         4

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
#define S_PICKUPMESSAGE     3

#define SPEED_FORWARD       15
#define SPEED_SIDE          13

#define WEPFLAGS_GOTWEAPON  1
#define WEPFLAGS_WEAPONSTAY 2

int HELPSTR = 
"Welcome to the Wheel of Samsara! There are a few optional RCon commands you may want to consider.\n\
sv_banjetpack 0/1: Toggles whether Duke is allowed to pick up his jetpack or not.\n\
sv_sogravity 0/1: Toggles whether the Security Officer operates off Marathon or Doom gravity.\n\
sv_permault 0/1: Toggles whether the Weapon VII is persistent or vanishes on pickup.\n\
sv_lmslife 0/1/2/3/4/5: Affects how much health/armor people have on LMS spawn/respawn.\n\
sv_lmsult 0/1: Toggles whether or not players get their VII in LMS mode.";

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

int ClassFades[CLASSCOUNT][4] =
{
    {1.0, 1.0, 0.0, 0.1},
    {1.0, 1.0, 0.0, 0.1},
    {1.0, 1.0, 0.0, 0.1},
    {1.0, 1.0, 0.0, 0.1},
    {1.0, 1.0, 0.0, 0.1},
    {1.0, 1.0, 0.0, 0.1},
    {0.0, 1.0, 0.0, 0.4},
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
        {" Fist ",                          "",             "",             "I PUNCH YOO"},
        {" Chainsaw ",                      "",             "",             "A chainsaw! Find some meat!"},
        {" Pistol ",                        "",             "",             "Picked up a pistol."},
        {" Shotgun ",                       "AmmoShell",    "",             "You got the shotgun!"},
        {"Super Shotgun",                   "AmmoShell",    "",             "You got the super shotgun!"},
        {" Chaingun ",                      "Clip",         "",             "You got the chaingun!"},
        {"Rocket Launcher",                 "RocketAmmo",   "",             "You got the rocket launcher!"},
        {"Plasma Rifle",                    "Cell",         "",             "You got the plasma rifle!"},
        {"B.F.G. 9000",                     "Cell",         "",             "You got the BFG9000! Oh, yes."},
    },

    {   // Chexguy
        {" Bootspoon ",                     "",             "",             "I SCOOP YOO"},
        {"Super Bootspork",                 "",             "",             "Super Bootspork! Find some slime!"},
        {"Mini-Zorcher",                    "",             "",             "Picked up a Mini Zorcher."},
        {"Large Zorcher",                   "AmmoShell",    "",             "You got the Large Zorcher!"},
        {"Super Large Zorcher",             "AmmoShell",    "",             "You got the Mega Zorcher!"},
        {"Rapid Zorcher",                   "Clip",         "",             "You got the Rapid Zorcher!"},
        {"Zorch Propulsor",                 "RocketAmmo",   "",             "You got the Zorch Propulsor!"},
        {"Phasing Zorcher",                 "Cell",         "",             "You got the Phasing Zorcher!"},
        {"LAZ Device",                      "Cell",         "",             "You got the LAZ Device! Woot!"},
    },

    {   // Corvus
        {" Staff ",                         "",             "",             "I JAB YOO"},
        {"Gauntlets of the Necromancer",    "",             "",             "Gauntlets of the Necromancer! Find some disciples!"},
        {"Gold Wand",                       "",             "",             "Picked up a gold wand."},
        {" Firemace ",                      "AmmoShell",    "",             "You got the firemace!"},
        {"Ethereal Crossbow",               "AmmoShell",    "",             "You got the ethereal crossbow!"},
        {"Dragon Claw",                     "Clip",         "",             "You got the blaster!"},
        {"Phoenix Rod",                     "RocketAmmo",   "",             "You got the Phoenix rod!"},
        {"Hellstaff",                       "Cell",         "",             "You got the hellstaff!"},
        {"PortTomeCoop",                    "",             "",             "You got the Tome of Power! Ahahaha..."},
    },

    {   // B.J.
        {"Knife",                           "",             "",             "I STAB YOO"},
        {"GotWeapon0",                      "",             "",             "Your knife developed teeth! Find some Nazis!"},
        {"Luger",                           "",             "",             "Picked up a Luger."},
        {"Machine Gun",                     "Clip",         "",             "You got the machine gun!"},
        {"Machine Gun",                     "Clip",         "",             "You got the machine gun!"},
        {"  Chaingun  ",                    "Clip",         "",             "You got the gatling gun!"},        // rename this chaingun to gatling gun :(
        {" Rocket Launcher ",               "RocketAmmo",   "",             "You got the rocket launcher!"},    // and this one to zreep gun
        {" Flamethrower ",                  "Cell",         "",             "You got the flamethrower!"},
        {"Spear of Destiny",                "Cell",         "",             "You got the Spear of Destiny! Heaven awaits."},
    },

    {   // Parias
        {"Mace of Contrition",              "",             "",             "I WAK YOO"},
        {"PortFlechette",                   "",             "",             "Flechettes! Find some centaurs!"},
        {"Sapphire Wand",                   "",             "",             "Picked up a sapphire wand."},
        {"Frost Shards",                    "AmmoShell",    "",             "You got the frost shards!"},
        {"Timon's Axe",                     "AmmoShell",    "",             "You got Timon's axe!"},
        {"Serpent Staff",                   "Clip",         "",             "You got the serpent staff!"},
        {"Hammer of Retribution",           "RocketAmmo",   "",             "You got the hammer of retribution!"},
        {"Firestorm",                       "Cell",         "",             "You got the Firestorm!"},
        {"Wraithverge",                     "Cell",         "",             "You got the Wraithverge! Smite with extreme prejudice."},
    },

    {   // Duke
        {"Mighty Boot",                     "",             "",             "I KICK YOO"},
        {"Pipebombs",                       "RocketAmmo",   "",             "Pipe bombs! Find some unsuspecting saps!"},
        {"M1911",                           "",             "",             "Picked up an M1911."},
        {"  Shotgun  ",                     "AmmoShell",    "",             "You got the shotgun!"},  // rename this to something like pump-shotty
        {"Explosive Shotgun",               "RocketAmmo",   "",             "You got the explosive shotgun!"},
        {"Chaingun Cannon",                 "Clip",         "",             "You got the chaingun cannon!"},
        {"RPG",                             "RocketAmmo",   "",             "You got the RPG launcher!"},
        {"Freezethrower",                   "Cell",         "",             "You got the freezethrower!"},
        {"Devastator",                      "Cell",         "",             "You got the Devastators! Hail to the king."},
    },

    {   // Security Officer
        {"Steel Knuckles",                  "",             "",             "I PUNCH YOO TOO"},
        {"SOMagnumSMGGiver",                "",             "",             "A KKV-7 SMG and an extra magnum! Find some aliens!"},
        {".44 Magnum Mega-Class A1",        "",             "",             "Picked up a magnum."},
        {"WSTE-M5 Combat Shotgun",          "AmmoShell",    "",             "You got the WSTE-M5 combat shotgun!"},
        {"Fusion Pistol",                   "FusionBullet", "Cell",         "You got the Zeus-class fusion pistol!"},
        {"MA-75B Assault Rifle",            "RifleBullet",  "Clip",         "You got the MA-75B assault rifle!"},
        {"SPNKR-XP SSM Launcher",           "SpankerAmmo",  "RocketAmmo",   "You got the SPNKR-XP SSM launcher!"},
        {"TOZT-7 Napalm Unit",              "NapalmInTank", "Cell",         "You got the TOZT-7 napalm unit!"},
        {"ONI-71 Wave Motion Cannon",       "Cell",         "RocketAmmo",   "You got the wave motion cannon! Blast 'em in half."},
    }
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
