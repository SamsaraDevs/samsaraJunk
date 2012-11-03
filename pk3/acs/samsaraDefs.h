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
#define CHOICECOUNT         5
#define MSGCOUNT            2

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

int LMSItems[CLASSCOUNT] =
{
    "",
    "",
    "PortGhostly",
    "WolfenAmmoCrate",
    "",
    "",
    "",
};

int ClassWeapons[CLASSCOUNT][SLOTCOUNT][CHOICECOUNT] = 
{
    {   // Doomguy
        {" Fist ",                          "",             "",             "", ""},
        {" Chainsaw ",                      "",             "",             "", ""},
        {" Pistol ",                        "",             "",             "", ""},
        {" Shotgun ",                       "AmmoShell",    "",             "", ""},
        {"Super Shotgun",                   "AmmoShell",    "",             "", ""},
        {" Chaingun ",                      "Clip",         "",             "", ""},
        {"Rocket Launcher",                 "RocketAmmo",   "",             "", ""},
        {"Plasma Rifle",                    "Cell",         "",             "", ""},
        {"B.F.G. 9000",                     "Cell",         "",             "", ""},
    },

    {   // Chexguy
        {" Bootspoon ",                     "",             "",             "", ""},
        {"Super Bootspork",                 "",             "",             "", ""},
        {"Mini-Zorcher",                    "",             "",             "", ""},
        {"Large Zorcher",                   "AmmoShell",    "",             "", ""},
        {"Super Large Zorcher",             "AmmoShell",    "",             "", ""},
        {"Rapid Zorcher",                   "Clip",         "",             "", ""},
        {"Zorch Propulsor",                 "RocketAmmo",   "",             "", ""},
        {"Phasing Zorcher",                 "Cell",         "",             "", ""},
        {"LAZ Device",                      "Cell",         "",             "", ""},
    },

    {   // Corvus
        {" Staff ",                         "",             "",             "", ""},
        {"Gauntlets of the Necromancer",    "",             "",             "", ""},
        {"Elven Wand",                      "",             "",             "", ""},
        {" Firemace ",                      "AmmoShell",    "",             "", ""},
        {"Ethereal Crossbow",               "AmmoShell",    "",             "", ""},
        {"Dragon Claw",                     "Clip",         "",             "", ""},
        {"Phoenix Rod",                     "RocketAmmo",   "",             "", ""},
        {"Hellstaff",                       "Cell",         "",             "", ""},
        {"PortTomeCoop",                    "",             "",             "", ""},
    },

    {   // B.J.
        {"Knife",                           "",             "",             "", ""},
        {"GotWeapon0",                      "",             "",             "", ""},
        {"Luger",                           "",             "",             "", ""},
        {"Machine Gun",                     "Clip",         "",             "", ""},
        {"Machine Gun",                     "Clip",         "",             "", ""},
        {"  Chaingun  ",                    "Clip",         "",             "", ""},
        {" Rocket Launcher ",               "RocketAmmo",   "",             "", ""},
        {" Flamethrower ",                  "Cell",         "",             "", ""},
        {"Spear of Destiny",                "Cell",         "",             "", ""},
    },

    {   // Parias
        {"Mace of Contrition",              "",             "",             "", ""},
        {"PortFlechette",                   "",             "",             "", ""},
        {"Sapphire Wand",                   "",             "",             "", ""},
        {"Frost Shards",                    "AmmoShell",    "",             "", ""},
        {"Timon's Axe",                     "AmmoShell",    "",             "", ""},
        {"Serpent Staff",                   "Clip",         "",             "", ""},
        {"Hammer of Retribution",           "RocketAmmo",   "",             "", ""},
        {"Firestorm",                       "Cell",         "",             "", ""},
        {"Wraithverge",                     "Cell",         "",             "", ""},
    },

    {   // Duke
        {"Mighty Boot",                     "",             "",             "", ""},
        {"Pipebombs",                       "RocketAmmo",   "",             "", ""},
        {"M1911",                           "",             "",             "", ""},
        {"  Shotgun  ",                     "AmmoShell",    "",             "", ""},
        {"Explosive Shotgun",               "RocketAmmo",   "",             "", ""},
        {"Chaingun Cannon",                 "Clip",         "",             "", ""},
        {"RPG",                             "RocketAmmo",   "",             "", ""},
        {"Freezethrower",                   "Cell",         "",             "", ""},
        {"Devastator",                      "Cell",         "",             "", ""},
    },

    {   // Security Officer
        {"Steel Knuckles",                  "",             "",             "",                 ""},
        {"KKV-7 SMG Flechette",             "Clip",         "",             "CanDualPistols",   ""},
        {".44 Magnum Mega Class A1",        "",             "",             "",                 ""},
        {"WSTE-M5 Combat Shotgun",          "AmmoShell",    "",             "CanDualShotties",  "LevelLimiter"},
        {"Fusion Pistol",                   "FusionBullet", "Cell",         "",                 ""},
        {"MA-75B Assault Rifle",            "RifleBullet",  "Clip",         "",                 ""},
        {"SPNKR-XP SSM Launcher",           "SpankerAmmo",  "RocketAmmo",   "",                 ""},
        {"TOZT-7 Napalm Unit",              "NapalmInTank", "Cell",         "",                 ""},
        {"ONI-71 Wave Motion Cannon",       "Cell",         "RocketAmmo",   "",                 ""},
    }
};

int ClassScripts[CLASSCOUNT][SLOTCOUNT] = 
{
    {0,0,                   0,  0,0,0,0,0,0},
    {0,0,                   0,  0,0,0,0,0,0},
    {0,0,                   0,  0,0,0,0,0,0},
    {0,0,                   0,  0,0,0,0,0,0},
    {0,0,                   0,  0,0,0,0,0,0},
    {0,0,                   0,  0,0,0,0,0,0},
    {0,SAMSARA_MARATHON,    0,  SAMSARA_MARATHON,0,0,0,0,0},
};

int ClassPickupMessages[CLASSCOUNT][SLOTCOUNT][MSGCOUNT] = 
{
    {
         {"I PUNCH YOO", "\cgRIP AND TEAR"},
         {"A chainsaw! Find some meat!", "The great communicator!"},
         {"Picked up a pistol.", "Hey, useless!"},
         {"You got the Shotgun!", "Might makes light!"},
         {"You got the Super Shotgun!", "And I feel mighty!"},
         {"You got the Chaingun!", "Hey, chaingun! The hell with respect!"},
         {"You got the Rocket Launcher!", "The sugar-sweet kiss of heavy ordinance!"},
         {"You got the Plasma Rifle!", "Sweet christmas! It's always something!"},
         {"You got the BFG9000! Oh, yes.", "Hail to the chief!"},
    },
    {   // yeah I really didn't have ideas for this one
         {"I SCOOP YOO", "\cdPOUR AND SHARE"},
         {"A Super Bootspork! Find some milk!", "I'm pretty sure this wasn't made for boots."},
         {"Picked up a Mini Zorcher.", "Hey, useless zappy thing!"},
         {"You got the Large Zorcher!", "Why do zappy things need spread again?"},
         {"You got the Super Large Zorcher!", "Bouncy death in a white box!"},
         {"You got the Rapid Zorcher!", "Wasn't this an eggbeater?"},
         {"You got the Zorch Propulsor!", "No rocket jumping for you."},
         {"You got the Phasing Zorcher!", "Zappy red things!"},
         {"You got the LAZ Device! Woot!", "The pinnacle of science. Giant red blobs of zap."},
    },
    {
         {"I JAB YOO", "\cfBEAT AND PARRY"},
         {"Gauntlets of the Necromancer! Find some disciples!", "The gloves your gloves wish they were."},
         {"Picked up a gold wand.", "Hey, useless crystal thing!"},
         {"You got the Firemace!", "And you thought this was Slot 7."},
         {"You got the Ethereal Crossbow!", "Aren't crossbows meant to be accurate?"},
         {"You got the Dragon Claw!", "So apparently dragons are walking miniguns?"},
         {"You got the Phoenix Rod!", "Also doubles as a shitty coat rack."},
         {"You got the Hellstaff!", "It's too close to a plasma rifle. UAC's gonna sue."},
         {"You got the Tome of Power! Ahahaha...", "Fuck BFG, we'll make \caeverything\c- overpowered."},
    },
    {
         {"I STAB YOO", "\cgSTAB AND SPAR"},
         {"Your knife developed teeth! Find some Nazis!", "Now \cathis\c- is a knife!"},
         {"Picked up a Luger.", "Hey, sniper useless!"},
         {"You got the Machine Gun!", "Shotgun? Fuck off, I've got a machine gun."},
         {"You got the Machine Gun!", "Super shotgun? Ahahaha no."},
         {"You got the Chaingun!", "If this were Wolf3D, you'd just have hit top tier."},
         {"You got the Rocket Launcher!", "You got the Zreep Gun!"},
         {"You got the Flamethrower!", "Because flamethrowers have infinite range."},
         {"You got the Spear of Destiny! Heaven awaits.", "Buttered popcorn, straight from Jesus."},
    },
    {
         {"I WAK YOO", "\cfPOUND AND SMEAR"},
         {"Flechettes! Find some centaurs!", "Mexican burritos would be more effective."},
         {"Picked up a sapphire wand.", "Hey, actually slightly useful!"},
         {"You got the Frost Shards!", "You got the Massive Weapon Frost-wait what."},
         {"You got Timon's Axe!", "Because whacking people with an axe needs lightning."},
         {"You got the Serpent Staff!", "CLERIC SAPPIN MAH CENTAUR"},
         {"You got the Hammer of Retribution!", "Infinite amounts of exploding hammers. Okay then."},
         {"You got the Firestorm!", "Literal baptism by fire."},
         {"You got the Wraithverge! Scream for me!", "Still less powerful than a fart cloud."},
    },
    {
         {"I KICK YOO", "Balls of steel, meet boots of steel."},
         {"Pipe bombs! Find some unsuspecting saps!", "Enjoy \cathis\c- present."},
         {"Picked up an M1911.", "Counterpoint number 1."},
         {"You got the Riot Shotgun!", "I hope you like buckshot for lunch."},
         {"You got the Explosive Shotgun!", "This'll satisfy my appetite for \cgDEATH\c-."},
         {"You got the Chaingun Cannon!", "Oh dear, you're coming apart at the seams!"},
         {"You got the RPG Launcher!", "When I'm through, the walls will be a memorial of your interiors."},
         {"You got the Freezethrower!", "Stopped you dead in your tracks, didn't I?"},
         {"You got the Devastators! Hail to the King.", "Game over, chumps."},
    },
    {
         {"I PUNCH YOO TOO", "Run sideways, oneshot imp. Got it."},
         {"A KKV-7 SMG and an extra .44 Magnum! Find some aliens!", "I'm gonna go cowboy on your ass."},
         {"Picked up a magnum.", "HOW I HOLD NOTHER PSITOL I DUNO LOL"},
         {"You got the WSTE-M5 Combat Shotgun!", "I'm gonna make an expressway to your heart."},
         {"You got the Zeus Class Fusion Pistol!", "Oh, you're invulnerable? That's cute."},
         {"You got the MA-75B Assault Rifle!", "I'll suppress you forever."},
         {"You got the SPNKR-XP SSM Launcher!", "The janitors will hate me today."},
         {"You got the TOZT-7 Napalm Unit!", "Who wants a barbeque?"},
         {"You got the ONI-71 Wave Motion Cannon! What's that spell? Trouble.", "I'm returning you to the store in a coffee tin."}, // since apparently this quote ain't good enuf
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
