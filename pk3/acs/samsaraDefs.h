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
#define SAMSARA_QUAKE               606
#define SAMSARA_RESONATE            608
#define SAMSARA_SYNTHFIRE           607
#define SAMSARA_MEGAHEALTH          259
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
#define SLOTCOUNT           10
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

#define UNLOADCOUNT 37

#define P_COUNT 2
#define P_QUAD  0
#define P_REGEN 1

#define QUAD_THRESHOLD      2100   // one minute

#define PY640   380.0
#define PY320   (PY640/2)

#define POFF640 40.0
#define POFF320 20.0

#define REGEN_CENTER_X      0.953125    // 610 / 640
#define REGEN_CENTER_Y      (PY640 / 480)
#define REGEN_OFFSET        0.083333    // 40 / 480

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
samsara_chainsawstart [0-2]: Toggles whether players get their I on enter or spawn.\n\
samsara_peoplediewhentheyarekilled [0-3]: Players explode on death.\n\
samsara_punchdrunk 0/1: Toggles melee-only mode. Only works in deathmatch or coop.\n\
samsara_punchdrunkuniques 0/1: Toggles whether Punchdrunk uniques are present in normal play.\n\
samsara_punchdrunksaws 0/1: Toggles whether Punchdrunk Chainsaws are present in normal play.\n\
samsara_armormode [0-4]: Toggles the type of armor that spawns in-game.\n\
samsara_nohealthcap 0/1: Toggles whether 100/200 is the health cap for players, or infinity.\n\
samsara_noult 0/1: Toggles whether the VII comes into play or never spawns.\n\
samsara_nomonologues 0/1: Toggles whether the bosses speak on spawn or not.\n\
samsara_backpackstart 0/1: Toggles whether players spawn normally or with backpacks.\n\
samsara_classiclaz 0/1: Toggles whether the LAZ Device has Samsara behavior or original behavior.\n\
samsara_allcanrj 0/1: Every character with non-self-damaging rockets will be able to RJ with them.\n\
samsara_nounique 0/1: Toggles whether Unique Item spawns.\n\
samsara_noinvuln 0/1: Toggles whether Invulnerability spawns.\n\
samsara_ban[class] 0/1: Forbids players from playing certain classes. bandoomguy for Doomguy, banchex for Chex Warrior, etc.\n\
samsara_lmsrules 0/1: For players who want Rocket Arena-style showdowns or just to spawn with all weapons.\n\
samsara_[class]damage -10-30: Adjusts the character's damage multiplier.\n\
samsara_[class]defense -10-20: Adjusts the character's defense multiplier.";

int HELPSTR_CL = 
"Likewise, there are several clientside console variables, including:\n\
samsara_cl_printpickup 0/1: 1 = Print, 0 = Log.\n\
samsara_cl_expparticles [1-100000]: How many particles does Ranger's explosions emit? 0 is default, -1 to disable.\n\
samsara_cl_norecoil 0/1: Controls whether Ranger's weapons recoil.\n\
samsara_cl_sinerecoil 0/1: Toggles whether Ranger's recoil movement uses a sine wave.\n\
samsara_cl_weaponhud 0/1: Controls whether the weapon bar at the top of your screen appears.\n\
samsara_cl_vanilladoom 0/1: Toggles the vanilla weapon animations for Doomguy.\n\
samsara_cl_wolfmove 0/1: Toggles classic Wolfenstein movement on B.J. Blazkowicz.";

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

int PickupStates[CLASSCOUNT][8] = 
{
    {"Doomguy", "DoomFull",     "DoomEmpty",        "DoomSpecial",      "DoomPunchdrunk",      "DoomPDFull",       "DoomPDEmpty",      "DoomPDSpecial"},
    {"Chexguy", "ChexFull",     "ChexEmpty",        "ChexSpecial",      "ChexPunchdrunk",      "ChexPDFull",       "ChexPDEmpty",      "ChexPDSpecial"},
    {"Heretic", "HereticFull",  "HereticEmpty",     "HereticSpecial",   "HereticPunchdrunk",   "HereticPDFull",    "HereticPDEmpty",   "HereticPDSpecial"},
    {"Wolfguy", "WolfFull",     "WolfEmpty",        "WolfSpecial",      "WolfPunchdrunk",      "WolfPDFull",       "WolfPDEmpty",      "WolfPDSpecial"},
    {"Hexen",   "HexenFull",    "HexenEmpty",       "HexenSpecial",     "HexenPunchdrunk",     "HexenPDFull",      "HexenPDEmpty",     "HexenPDSpecial"},
    {"Dukeguy", "DukeFull",     "DukeEmpty",        "DukeSpecial",      "DukePunchdrunk",      "DukePDFull",       "DukePDEmpty",      "DukePDSpecial"},
    {"Marathon","MarathonFull", "MarathonEmpty",    "MarathonSpecial",  "MarathonPunchdrunk",  "MarathonPDFull",   "MarathonPDEmpty",  "MarathonPDSpecial"},
    {"Quake",   "QuakeFull",    "QuakeEmpty",       "QuakeSpecial",     "QuakePunchdrunk",     "QuakePDFull",      "QuakePDEmpty",     "QuakePDSpecial"},   
};

int ItoSArray[7] = {1, 3, 4, 5, 6, 7, 8};
int StoIArray[SLOTCOUNT] = {-1, 1, -1, 2, 3, 4, 5, 6, 7, -1};

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
    "",
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
    "QuakeQuadTimer", "QuakeRegenTimer", "QuakeInvisTimer", "ChickenKillCount",
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

#define ARMORCOUNT  6

#define ARMOR_GREEN             0
#define ARMOR_YELLOW            1
#define ARMOR_BLUE              2
#define ARMOR_BLUE_WITHYELLOW   3
#define ARMOR_RED               4
#define ARMOR_MEGASPHERE        5

int ArmorPickups[ARMORCOUNT] = 
{
    "GreenArmorPickup",
    "YellowArmorPickup",
    "BlueArmorPickup",
    "BlueArmorPickup",
    "RedArmor",
    "MegaPack",
};

int ArmorItems[ARMORMODES][ARMORCOUNT][3] = 
{
    {
        {"ArmorPack1",          100, "GreenArmorNoDrop"},
        {"SamsaraYellowArmor",  150, "YellowArmorNoDrop"},
        {"ArmorPack2",          200, "BlueArmorNoDrop"},
        {"ArmorPack2",          200, "BlueArmorNoDrop"},
        {"ArmorPack3",          250, "RedArmorNoDrop"},
        {"ArmorPack2",          200, "BlueArmorNoDrop"},
    },
    {
        {"QuakeGreenArmor",     100, "QGreenArmorNoDrop"},
        {"QuakeYellowArmor",    150, "QYellowArmorNoDrop"},
        {"QuakeYellowArmor",    150, "QYellowArmorNoDrop"},
        {"QuakeRedArmor",       200, "QRedArmorNoDrop"},
        {"SamsaraSilverArmor",  250, "SilverArmorNoDrop"},
        {"QuakeRedArmor",       200, "QRedArmorNoDrop"},
    },
    {
        {"QuakeYellowArmor",    150, "QYellowArmorNoDrop"},
        {"QuakeYellowArmor",    150, "QYellowArmorNoDrop"},
        {"QuakeRedArmor",       200, "QRedArmorNoDrop"},
        {"QuakeRedArmor",       200, "QRedArmorNoDrop"},
        {"SamsaraSilverArmor",  250, "SilverArmorNoDrop"},
        {"QuakeRedArmor",       200, "QRedArmorNoDrop"},
    },
    {
        {"MarathonGreenArmor",  100, "MGreenArmorNoDrop"},
        {"MarathonYellowArmor", 150, "MYellowArmorNoDrop"},
        {"MarathonBlueArmor",   200, "MBlueArmorNoDrop"},
        {"MarathonBlueArmor",   200, "MBlueArmorNoDrop"},
        {"MarathonRedArmor",    300, "MRedArmorNoDrop"},
        {"MarathonRedArmor",    300, "MRedArmorNoDrop"},
    },
    {
        {"NoArmor",             0, "NoArmor"},
        {"NoArmor",             0, "NoArmor"},
        {"NoArmor",             0, "NoArmor"},
        {"NoArmor",             0, "NoArmor"},
        {"NoArmor",             0, "NoArmor"},
        {"NoArmor",             0, "NoArmor"},
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

int PunchDrunkItems[CLASSCOUNT][2] = 
{
    {"DoomguyPunchDrunk",   "DoomguyPunchUnique"},
    {"ChexguyPunchDrunk",   "ChexguyPunchUnique"},
    {"CorvusPunchDrunk",    "CorvusPunchUnique"},
    {"BlazkoPunchDrunk",    "BlazkoPunchUnique"},
    {"PariasPunchDrunk",    "PariasPunchUnique"},
    {"DukePunchDrunk",      "DukePunchUnique"},
    {"ManathorPunchDrunk",  "ManathorPunchUnique"},
    {"RangerPunchDrunk",    "RangerPunchUnique"},
};

int BanCVars[CLASSCOUNT] = 
{
    "samsara_bandoomguy",
    "samsara_banchex",
    "samsara_bancorvus",
    "samsara_banbj",
    "samsara_banparias",
    "samsara_banduke",
    "samsara_banso",
    "samsara_banranger",
};

int BanStrings[CLASSCOUNT] =
{
	"\c[m7]The admin has banned \cdDoomguy\c[m7] from play.",
	"\c[m7]The admin has banned \chChex Warrior\c[m7] from play.",
	"\c[m7]The admin has banned \cqCorvus\c[m7] from play.",
	"\c[m7]The admin has banned \cjB.J. Blazkowicz\c[m7] from play.",
	"\c[m7]The admin has banned \chParias\c[m7] from play.",
	"\c[m7]The admin has banned \cfDuke Nukem\c[m7] from play.",
	"\c[m7]The admin has banned \cnSecurity Officer\c[m7] from play.",
	"\c[m7]The admin has banned \csRanger\c[m7] from play.",
};


#define DAMAGEMODES 41
#define DAMAGEZEROINDEX 10

#define DEFENSEMODES 31
#define DEFENSEZEROINDEX 10

int DamageCVars[CLASSCOUNT] = 
{
    "samsara_doomguydamage",
    "samsara_chexdamage",
    "samsara_corvusdamage",
    "samsara_bjdamage",
    "samsara_pariasdamage",
    "samsara_dukedamage",
    "samsara_sodamage",
    "samsara_rangerdamage",
};

int DefenseCVars[CLASSCOUNT] = 
{
    "samsara_doomguydefense",
    "samsara_chexdefense",
    "samsara_corvusdefense",
    "samsara_bjdefense",
    "samsara_pariasdefense",
    "samsara_dukedefense",
    "samsara_sodefense",
    "samsara_rangerdefense",
};

int CVarDamageItems[DAMAGEMODES] = 
{
    "PowerDamageBuff-10",
    "PowerDamageBuff-9",
    "PowerDamageBuff-8",
    "PowerDamageBuff-7",
    "PowerDamageBuff-6",
    "PowerDamageBuff-5",
    "PowerDamageBuff-4",
    "PowerDamageBuff-3",
    "PowerDamageBuff-2",
    "PowerDamageBuff-1",
    "", // middle point
    "PowerDamageBuff1",
    "PowerDamageBuff2",
    "PowerDamageBuff3",
    "PowerDamageBuff4",
    "PowerDamageBuff5",
    "PowerDamageBuff6",
    "PowerDamageBuff7",
    "PowerDamageBuff8",
    "PowerDamageBuff9",
    "PowerDamageBuff10",
    "PowerDamageBuff11",
    "PowerDamageBuff12",
    "PowerDamageBuff13",
    "PowerDamageBuff14",
    "PowerDamageBuff15",
    "PowerDamageBuff16",
    "PowerDamageBuff17",
    "PowerDamageBuff18",
    "PowerDamageBuff19",
    "PowerDamageBuff20",
    "PowerDamageBuff21",
    "PowerDamageBuff22",
    "PowerDamageBuff23",
    "PowerDamageBuff24",
    "PowerDamageBuff25",
    "PowerDamageBuff26",
    "PowerDamageBuff27",
    "PowerDamageBuff28",
    "PowerDamageBuff29",
    "PowerDamageBuff30",
};

int CVarDefenseItems[DEFENSEMODES] =
{
    "PowerDefenseBuff-10",
    "PowerDefenseBuff-9",
    "PowerDefenseBuff-8",
    "PowerDefenseBuff-7",
    "PowerDefenseBuff-6",
    "PowerDefenseBuff-5",
    "PowerDefenseBuff-4",
    "PowerDefenseBuff-3",
    "PowerDefenseBuff-2",
    "PowerDefenseBuff-1",
    "", // middle point
    "PowerDefenseBuff1",
    "PowerDefenseBuff2",
    "PowerDefenseBuff3",
    "PowerDefenseBuff4",
    "PowerDefenseBuff5",
    "PowerDefenseBuff6",
    "PowerDefenseBuff7",
    "PowerDefenseBuff8",
    "PowerDefenseBuff9",
    "PowerDefenseBuff10",
    "PowerDefenseBuff11",
    "PowerDefenseBuff12",
    "PowerDefenseBuff13",
    "PowerDefenseBuff14",
    "PowerDefenseBuff15",
    "PowerDefenseBuff16",
    "PowerDefenseBuff17",
    "PowerDefenseBuff18",
    "PowerDefenseBuff19",
    "PowerDefenseBuff20",
};


int ChangelogString =
"FROM 0.29b TO 0.3\n\
========================\n\
GAMEPLAY:\n\
- B.J.'s chaingun gave less ammo on pickup than his Machine Gun. Fixed, now it gives 20 ammo like the Machine Gun does.\n\
- The Quake wiki reveals the lava nails were dealing too much damage. They were not more powerful in DM, they just pierced armor. And they only did 30% extra damage in co-op. Ranger was adjusted accordingly.\n\
- Someone had removed +THRUGHOST on Ranger's nails. They were put back in.\n\
- The Fusion Pistol's shots can no longer be deflected.\n\
- In lieu of Zandronum 1.1.1, Duke's and Ranger's bullets now use FBF_NORANDOM! This gives them wonderful Unlagged compatibility while still being, as in Duke Nukem 3D, completely static about their damage!\n\
- Splitting the Wraithverge into DM modes and coop modes gave it the side effect of being terribly, terribly overpowered. Its damage in deathmatch has been thirded and it speeds up only half as often.\n\
- Timon's Axe now actually uses the damagetype TimonsAxe. \n\
- In lieu of FBF_NORANDOM, Duke Nukem's Explosive Shotgun has been reworked. Damage emphasis has been placed less on the bullets and more on the explosives--the bullets do less damage, while the explosions do more to a wider radius. To accomodate the wider radius, the accuracy has been tightened up as well.\n\
- The Devastators have had their radius increased as well, from 32 to 64, with damage increased from 35 to 48. Duke Nukem's HEIRARCHY OF EXPLOSIVES now goes:\n\
 ^- Pipebombs: Largest radius\n\
 ^- Explosive Shotgun: Most damage\n\
 ^- RPG: Fastest firing\n\
 ^- Devastators: Ultimate\n\
- Parias now only resists 0.375 of Duke's explosives, rather than the 0.25 he does of everyone else.\n\
- Doomguy's, Duke's, Corvus', and Chexter's bullets have been reworked to utilize an frandom-esque spread of projectiles, rather than the static grid. SO's bullets have been kept in a grid pattern, for accuracy to Marathon.\n\
- Chexguy's bullets were originally more accurate to counteract lack of an SSG. Now he has an SSG-esque weapon, which is super-accurate. Why do the other bullets still need to be more accurate? Reverted to a Doom-esque spread level.\n\
- Corvus' staff went through ghosts in Heretic. Now it does here.\n\
- The difficulty settings now have ACSReturn values, for maps with ACS scripts that rely on difficulty settings.\n\
- Here's one that's been around for a long, long time. Possessed Troopers, Sergeants, and Chaingunners couldn't infight with each other. Now they can!\n\
- A new cvar, samsara_noult, has been implemented for DM/CTF/etc play. When enabled, no VIIs will spawn on the battlefield.\n\
- Corvus' Crossbow has been slightly buffed and the Firemace slightly nerfed, changing out the DamageFactor Mace on everything with DamageFactor CorvusCrossbow. The DamageFactor Mace increase was made long, long ago in the early builds when the Firemace was an alternative to the chaingun and the Crossbow was OP. This is no longer the case.\n\
- Tome of Power doubles dealt damage and halves recieved damage. Cooldown reduced further to 200.\n\
- The SP and DM modes of the Super Large Zorcher's spatters on hit were swapped with each other. This has been fixed.\n\
- When all of Duke's firing modes were integrated, the Explosive Shotgun had been sped up slightly, rendering it faster than the Super Shotgun. It has been slowed down back to 56 tics.\n\
- The KKV SMG has had its out-of-water damage increased. While nowhere near as powerful as it was before, lack of water in ZDoom maps made its niche terribly infrequent at best.\n\
- All of the enemies in Doom, sans the boss monsters, can now be stunned with the Tech.50 Pacifier.\n\
- BJ has been given a very hacky recreation of A_WolfAttack--specifically, how bullets did more damage up-close. While BJ is and always will be a long-range focused character, a lot of Doom 2 is corridor and close-ranged based, and BJ was essentially useless in these matchups. He recieves a (very minor) 2(1d3) damage boost to his bullets when within 120 map units, which while not degrading from his long range power should give him a minor fighting chance up close.\n\
- samsara_backpackstart is a new cvar that, well, has players spawn with backpacks.\n\
- Corvus' bombs now force radius damage.\n\
- The +FORCERADIUSDMG of the LAZ Device has been moved from the initial blast to the lingering wall. To compensate, the radius of the lingering explosions has been slightly increased by 32 units.\n\
- From the same stupid minds that brought you Punchdrunk, a new gamemode has been implemented--Super Turbo Turkey Puncher 3000! How long can you stand against an endless wave of crazy chickens out for YOUR flesh?! Activated via samsara_superturboturkeypuncher3000, it endlessly spawns either aggressive chickens (1), cowardly chickens (2), or friendly chickens (3). Whoever reaches the kill limit (perscribed by samsara_superturboturkeylimit) first is the winner. It is compatible in all gamemodes with all other existing cvars, and can be used in LMS, coop, deathmatch, survival, CTF, punchdrunk, or wherever.\n\
- Ranger's Lava Nails now use the QuakeFire damagetype rather than Fire.\n\
- The Super Large Zorcher's bolts have been changed from projectile to fastprojectile, so that they can actually /hit/ people at high speeds. They now travel at 155 speed to hit long-range opponents more easily (though die off in 25 speed with each bounce) and the spatters now travel at speed 15 for a slightly wider AoE. Being a fastprojectile, the 'bounces' now all target Chexter, careening back to him rather than bouncing in a random direction--which should make bounce-shots easier for players to predict, and easier for opponents to avoid.\n\
- For the Chex Quest purists, samsara_classiclaz has been implemented as a cvar. When enabled, it changes the LAZ Device back to the BFG9000 behavior. Keep in mind, the BFG spray effect still does not zorch people in Zandronum!\n\
- Instagib mode is now supported! Everyone gets new weapons that, well, instagib people.\n\
- I misinterpreted the PLAYERMISSILERANGE's range for hitscans to be the same as hitscans for monsters. This means that the Doom enemy shots did not travel 8192 units long (long enough to not matter) but to 2048 units (short enough to matter). The DoomEnemyBullet actor now only travels for 6 tics (1920 units) before dissipating.\n\
- Ranger's Thunderbolt has been converted from A_Explode being the source of damage to the A_FireBullets, thanks to FBF_NoRandom. Parias and Chexguy no longer resist it, nor do bosses.\n\
- samsara_allcanrj has been implemented as a new cvar. For the characters whose rockets do not damage themselves (Parias, Chex Warrior, BJ), this allows for the rockets to deliver self-damage and launch them up.\n\
- samsara_nounique has been implemented for those who want duels without the unique item messing things up.\n\
- samsara_noinvuln has been implemented for mapsets that use Invulnerability replacements as props. I'm looking at you, IDL maps.\n\
- samsara_ban[class] has been implemented primarily as a debugging cvar, but admins who feel a certain character wrecks balance or want to enforce one-character-only matches may find it handy as well. On use, it forbids players from playing as certain classes--bandoomguy to forbid Doomguy, banchex to forbid Chex Warrior, banso to forbid Security Officer, etc.\n\
- samsara_lmsrules has been implemented for those who want Rocket-Arena-esque matches or just to start with all weapons. As per the name, the game is treated as LMS--everyone starts with all weapons, and samsara_lmslife determines their starting health/armor.\n\
- BJ was not the only person suffering from ammo woes--Parias now has BJ's ammo conversion system for picking up shells/rockets to convert.\n\
- Parias' hammer might have had no self-damage, but it was dramatically slower than everyone else's and had a delayed explosion that allowed people to run away. The explosion damage has been restored to 128.\n\
- It's impossible to make character balance that everyone agrees with--there are now cvars so that admins can buff or nerf a character on the spot as they see fit. samsara_[class]damage and samsara_[class]defense gives the class a defense or damage multiplier that can either bolster or hinder them. Alternatively, set everything at max and run through slaughterwads, or set everything at min and get a challenge.\n\
- FireExplosive has been implemented as a damagetype for the Phoenix Rod and Hammer of Retribution.\n\
- Two more gib decorative actors are now available for B.J. to drink up.\n\
- The Security Officer's KKV-7 SMG and Wave Motion Cannon now have an auto-reload.\n\
- Chex Warrior's LAZ Device has gone from 'stupidly powerful explosion' to 'pretty damn powerful', reduced from 896/512 damage/radius to 512/384.\n\
- Chex Warrior's bullets originally all hit ghosts to counteract lack of an SSG. Now he doesn't need that anymore. Poor dude's getting nerfed all around today.\n\
\n\
BUGFIXES:\n\
- Duke's left foot kicks with the Mighty Boot no longer do double damage, and Atomic Boot right foot kicks don't do double damage.\n\
- In Strife, apparently the Loremaster door wouldn't open if you had all five splinters? At least, that's what the code said. Fixed.\n\
- The Thunderbolt could not be forced to not recoil. Fixed.\n\
- In rare instances, the S.O.'s pain frames would be replaced with a burning corpse. This has been fixed.\n\
- Pain Elementals can no longer be unzorched by Archviles.\n\
- You may now suicide when invulnerable again.\n\
- Ranger's powerups now properly deplete on death.\n\
- Players could not be telefragged while invulnerable. This has been fixed.\n\
- In rare instances, the IV pickup would be replaced with Duke's Ripper. This has been fixed.\n\
- Ranger should no longer be able to infinitely stack Quad Damage on pickup.\n\
- In crazy instances where you face Lord Snotfolus before he finishes his monologue (?!), getting him in a painstate will no longer permanently lock him in invulnerability.\n\
- Dumb things happened with the Gargoyles' Zorch sequence online. PresidentPeople has fixed all of the dumb things!\n\
- Armors no longer spawn on the field in Last Man Standing mode.\n\
- Ranger's freezedeath graphic was being overwritten by one of his firing frames. No more.\n\
- If a player has been killed, Skulltag monsters should no longer repeatedly fire at their corpse.\n\
- Respawning no longer breaks various scripts.\n\
- Picking up an Invulnerability as Doomguy, Chex, Corvus, BJ, Duke, and SO now gives the proper god mode hud face.\n\
- Duke no longer moans every time he picks up a Soulsphere--now he only moans when he picks it up at critical health.\n\
- The Security Officer can no longer fight from beyond death by holding down fire as he dies.\n\
- Here's an old bug. Duke now gets taunt cooldown proper from picking up a weapon, and thus can no longer run two quotes at once by picking up a weapon and then killing someone/dying.\n\
- Here's another old bug. Players no longer dance in place after spawning and triggering coop mode.\n\
- Timefreeze no longer affects the LAZ Device shield, keeping people from locking up computers.\n\
- Timefreeze also no longer affects the LAZ's particles.\n\
- sv_shotgunstart now works on respawning when in non-cooperative game modes.\n\
- The Security Officer no longer can get a LevelLimiter stuck in his inventory when picking up a dropped shotgun.\n\
- Picking up a WSTE-M5 gave double the ammo. Whoops.\n\
- Flemoids no longer react like normal damage to the PhasingZorch and ZorchKamikaze damagetype.\n\
- Common Flemoids and Bipedal Common Flemoids no longer get hurt by the Flem damagetype.\n\
- Defining the teams as Raktah and Nilah broke announcers in team-based games. Sorry. Teams reverted back to default, which also unfortunately means the colored winpics and losepics go.\n\
- Chexguy would occaisionally go completely silent when 'gibbed'. No longer.\n\
- Traductus no longer uses Parias' terrifying version of the Wraithverge.\n\
- At the cost of irrevocably breaking Skulltag mode (which nobody plays, so it isn't a big loss), keys can no longer be crushed by crushers.\n\
- Duke's pipebombs can no longer telefrag people.\n\
- Online, Duke's Devastators no longer spam the server console with 'NETWORK_AttenuationFloatToInt: Warning unknown attenuation value!'.\n\
- The Dark Imp had brown specs all over his body on death. Popsoap has fixed this with a new translation.\n\
- The Cyberdemon now chases players properly.\n\
\n\
POLISH:\n\
- Party Mode particles are now spawned less often and disappear faster.\n\
- Sounds and flashes are now done if you lose armor with Marathon-style armor, to indicate when you're hit.\n\
- All fake bullets now go at 320 tics per unit, a slight speed boost.\n\
- Rephrased a few obituaries to be gender-ambiguous.\n\
- The Hectebus' shots now use the HexGreenFire damagetype.\n\
- Ranger's shaft has been given an increase in girth.\n\
- Duke's ricochet sound happened far too often. Now it happens much less often, as proper of Duke Nukem 3D.\n\
- Vanilla animations for Doomguy's Pistol/Chaingun now properly light up the room.\n\
- Duke's Explosive Shotgun's explosions now has the proper Duke Nukem 64 sprites.\n\
- Thanks to Popsoap, all Skulltag monsters now have GLDefs.\n\
- Ranger now has the Quake announcer shout 'INVISIBILITY!' on picking up the Partial Invisibility.\n\
- The Freezethrower now has its official Widescreen sprite from the Duke Nukem 3D Megaton Edition, ripped by Kinsie. Thanks!\n\
- The Flembrane and Flembomination now have a proper non-zorch death sequence. Only Lord Snotfolus left to go!\n\
- Here's one I've known about for a while but was too lazy to fix. The Fusion Pistol did not have a reload sound, and thus it has been removed.\n\
- The Laser Cannon in Scourge of Armagon had some interesting behavior I'd forgotten about--every second shot was two bolts combined into one, and it wasn't a 100% guarantee to bounce. Whoops!\n\
- Security Officer's Partial Invisibility pickup now behaves more closely to Marathon.\n\
- The 'see console' message now only pops up on first playing Samsara for clients. For servers, it still lists all of the serverside cvars every time. To see all of the cvars available, type 'cvarinfo' into the console--it lists all cvars, both serverside and clientside.\n\
- FuzzballFox has contributed almost 70 new colors for players to use in their names! WOWZERS!";
